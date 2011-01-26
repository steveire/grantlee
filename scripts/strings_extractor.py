#! /usr/bin/env python
# -*- coding: utf-8 -*-

##
# Copyright 2010,2011 Stephen Kelly <steveire@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

## Parts of this file are reproduced from the Django framework. The Django licence appears below.

##
# Copyright (c) Django Software Foundation and individual contributors.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#     1. Redistributions of source code must retain the above copyright notice,
#        this list of conditions and the following disclaimer.
#
#     2. Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#
#     3. Neither the name of Django nor the names of its contributors may be used
#        to endorse or promote products derived from this software without
#        specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

import re
import os.path

# == Introduction to the template syntax ==
#
# The template syntax looks like this:
# (For more see here: http://grantlee.org/apidox/for_themers.html )
#
# This is plain text
# This is text with a {{ value }} substitution
# This is {% if condition_is_met %}a conditional{% endif %}
# {# This is a comment #}
# This is a {% comment %} multi-line
# comment
# {% endcomment %}
#
# That is, we have plain text.
# We have value substitution with {{ }}
# We have comments with {# #}
# We have control tags with {% %}
#
# The first token inside {% %} syntax is called a tag name. Above, we have
# an if tag and a comment tag.
#
# The 'value' in {{ value }} is called a filter expression. In the above case
# the filter expression is a simple value which was inserted into the context.
# In other cases it can be {{ value|upper }}, that is the value can be passed
# through a filter called 'upper' with the '|', or filter expression can
# be {{ value|join:"-" }}, that is it can be passed through the join filter
# which takes an argument. In this case, the 'value' would actually be a list,
# and the join filter would concatenate them with a dash. A filter can have
# either no arguments, like upper, or it can take one argument, delimited by
# a colon (';'). A filter expression can consist of a value followed by a
# chain of filters, such as {{ value|join:"-"|upper }}. A filter expression
# can appear one time inside {{ }} but may appear multiple times inside {% %}
# For example {% cycle foo|upper bar|join:"-" bat %} contains 3 filter
# expressions, 'foo|upper', 'bar|join:"-"' and 'bat'.
#
# Comments are ignored in the templates.
#
# == i18n in templates ==
#
# The purpose of this script is to extract translatable strings from templates
# The aim is to allow template authors to write templates like this:
#
# This is a {{ _("translatable string") }} in the template.
# This is a {% i18n "translatable string about %1" something %}
# This is a {% i18nc "Some context information" "string about %1" something %}
# This is a {% i18np "%1 string about %2" numthings something %}
# This is a {% i18ncp "some context" "%1 string about %2" numthings something %}
#
# That is, simple translation with _(), and i18n* tags to allow for variable
# substitution, context messages and plurals. Translatable strings may appear
# in a filter expression, either as the value begin filtered, or as the argument
# or both:
#
# {{ _("hello")|upper }}
# {{ list|join:_("and") }}
#
# == How the strings are extracted ==
#
# The strings are extracted by parsing the template with regular expressions.
# The tag_re regular expression breaks the template into a stream of tokens
# containing plain text, {{ values }} and {% tags %}.
# That work is done by the tokenize method with the create_token method.
# Each token is then processed to extract the translatable strings from
# the filter expressions.


# The original context of much of this script is in the django template system:
# http://code.djangoproject.com/browser/django/trunk/django/template/base.py


TOKEN_TEXT = 0
TOKEN_VAR = 1
TOKEN_BLOCK = 2
TOKEN_COMMENT = 3

# template syntax constants
FILTER_SEPARATOR = '|'
FILTER_ARGUMENT_SEPARATOR = ':'
BLOCK_TAG_START = '{%'
BLOCK_TAG_END = '%}'
VARIABLE_TAG_START = '{{'
VARIABLE_TAG_END = '}}'
COMMENT_TAG_START = '{#'
COMMENT_TAG_END = '#}'

# match a variable or block tag and capture the entire tag, including start/end delimiters
tag_re = re.compile('(%s.*?%s|%s.*?%s)' % (re.escape(BLOCK_TAG_START), re.escape(BLOCK_TAG_END),
                                          re.escape(VARIABLE_TAG_START), re.escape(VARIABLE_TAG_END)))


# Expression to match some_token and some_token="with spaces" (and similarly
# for single-quoted strings).
smart_split_re = re.compile(r"""
    ((?:
        [^\s'"]*
        (?:
            (?:"(?:[^"\\]|\\.)*" | '(?:[^'\\]|\\.)*')
            [^\s'"]*
        )+
    ) | \S+)
""", re.VERBOSE)

def smart_split(text):
    r"""
    Generator that splits a string by spaces, leaving quoted phrases together.
    Supports both single and double quotes, and supports escaping quotes with
    backslashes. In the output, strings will keep their initial and trailing
    quote marks and escaped quotes will remain escaped (the results can then
    be further processed with unescape_string_literal()).

    >>> list(smart_split(r'This is "a person\'s" test.'))
    [u'This', u'is', u'"a person\\\'s"', u'test.']
    >>> list(smart_split(r"Another 'person\'s' test."))
    [u'Another', u"'person\\'s'", u'test.']
    >>> list(smart_split(r'A "\"funky\" style" test.'))
    [u'A', u'"\\"funky\\" style"', u'test.']
    """
    for bit in smart_split_re.finditer(text):
        yield bit.group(0)


# This only matches constant *strings* (things in quotes or marked for
# translation).

constant_string = r"(?:%(strdq)s|%(strsq)s)" % {
    'strdq': r'"[^"\\]*(?:\\.[^"\\]*)*"', # double-quoted string
    'strsq': r"'[^'\\]*(?:\\.[^'\\]*)*'", # single-quoted string
    }

filter_raw_string = r"""^%(i18n_open)s(?P<l10nable>%(constant_string)s)%(i18n_close)s""" % {
    'constant_string': constant_string,
    'i18n_open' : re.escape("_("),
    'i18n_close' : re.escape(")"),
  }

filter_re = re.compile(filter_raw_string, re.UNICODE|re.VERBOSE)

class TemplateSyntaxError(Exception):
    pass

class TranslatableString:
    _string = ''
    context = ''
    plural = ''

    def __repr__(self):
        return "String('%s', '%s', '%s')" % (self._string, self.context, self.plural)

class Token(object):
    def __init__(self, token_type, contents):
        # token_type must be TOKEN_TEXT, TOKEN_VAR, TOKEN_BLOCK or TOKEN_COMMENT.
        self.token_type, self.contents = token_type, contents

    def __str__(self):
        return '<%s token: "%s...">' % \
            ({TOKEN_TEXT: 'Text', TOKEN_VAR: 'Var', TOKEN_BLOCK: 'Block', TOKEN_COMMENT: 'Comment'}[self.token_type],
            self.contents[:20].replace('\n', ''))

def create_token(token_string, in_tag):
    """
    Convert the given token string into a new Token object and return it.
    If in_tag is True, we are processing something that matched a tag,
    otherwise it should be treated as a literal string.
    """
    if in_tag:
        if token_string.startswith(VARIABLE_TAG_START):
            token = Token(TOKEN_VAR, token_string[len(VARIABLE_TAG_START):-len(VARIABLE_TAG_END)].strip())
        elif token_string.startswith(BLOCK_TAG_START):
            token = Token(TOKEN_BLOCK, token_string[len(BLOCK_TAG_START):-len(BLOCK_TAG_END)].strip())
        elif token_string.startswith(COMMENT_TAG_START):
            token = Token(TOKEN_COMMENT, '')
    else:
        token = Token(TOKEN_TEXT, token_string)
    return token

def tokenize(template_string):

    in_tag = False
    result = []
    for bit in tag_re.split(template_string):
        if bit:
            result.append(create_token(bit, in_tag))
        in_tag = not in_tag
    return result

class TranslationOutputter:
    translatable_strings = []

    def get_translatable_filter_args(self, token):
        """
        Find the filter expressions in token and extract the strings in it.
        """
        matches = filter_re.finditer(token)
        upto = 0
        var_obj = False
        for match in matches:
            l10nable = match.group("l10nable")

            if l10nable:
                # Make sure it's a quoted string
                if l10nable.startswith('"') and l10nable.endswith('"') \
                        or l10nable.startswith("'") and l10nable.endswith("'"):
                    ts = TranslatableString()
                    ts._string = l10nable[1:-1]
                    self.translatable_strings.append(ts)

    def get_contextual_strings(self, token):
        split = []
        _bits = smart_split(token.contents)
        _bit = _bits.next()
        if _bit =="i18n" or _bit == "i18n_var":
            # {% i18n "A one %1, a two %2, a three %3" var1 var2 var3 %}
            # {% i18n_var "A one %1, a two %2, a three %3" var1 var2 var3 as result %}
            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            translatable_string = TranslatableString()
            translatable_string._string = _bit[1:-1]
            self.translatable_strings.append(translatable_string)
        elif _bit =="i18nc" or _bit == "i18nc_var":
            # {% i18nc "An email send operation failed." "%1 Failed!" var1 %}
            # {% i18nc_var "An email send operation failed." "%1 Failed!" var1 as result %}
            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            translatable_string = TranslatableString()
            translatable_string.context = _bit[1:-1]
            _bit = _bits.next()
            translatable_string._string = _bit[1:-1]
            self.translatable_strings.append(translatable_string)
        elif _bit =="i18np" or _bit =="i18np_var":
            # {% i18np "An email send operation failed." "%1 email send operations failed. Error : % 2." count count errorMsg %}
            # {% i18np_var "An email send operation failed." "%1 email send operations failed. Error : % 2." count count errorMsg as result %}
            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            translatable_string = TranslatableString()
            translatable_string._string = _bit[1:-1]
            _bit = _bits.next()
            translatable_string.plural = _bit[1:-1]
            self.translatable_strings.append(translatable_string)
        elif _bit =="i18ncp" or _bit =="i18ncp_var":
            # {% i18np "The user tried to send an email, but that failed." "An email send operation failed." "%1 email send operation failed." count count %}
            # {% i18np_var "The user tried to send an email, but that failed." "An email send operation failed." "%1 email send operation failed." count count as result %}

            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            translatable_string = TranslatableString()
            translatable_string.context = _bit[1:-1]
            _bit = _bits.next()
            translatable_string._string = _bit[1:-1]
            _bit = _bits.next()
            translatable_string.plural = _bit[1:-1]
            self.translatable_strings.append(translatable_string)
        else:
          return

        for _bit in _bits:

            if (_bit == "as"):
                return
            self.get_translatable_filter_args(_bit)

    def get_plain_strings(self, token):
        split = []
        bits = iter(smart_split(token.contents))
        for bit in bits:
            self.get_translatable_filter_args(bit)

    def translate(self, template_file, outputfile):
        template_string = template_file.read()
        self.translatable_strings = []
        for token in tokenize(template_string):
            if token.token_type == TOKEN_VAR or token.token_type == TOKEN_BLOCK:
                self.get_plain_strings(token)
            if token.token_type == TOKEN_BLOCK:
                self.get_contextual_strings(token)
        self.createOutput(os.path.relpath(template_file.name), self.translatable_strings, outputfile)

    def createOutput(self, template_filename, translatable_strings, outputfile):
      pass
