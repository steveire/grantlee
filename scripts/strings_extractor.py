#! /usr/bin/env python
# -*- coding: utf-8 -*-

import re

TOKEN_TEXT = 0
TOKEN_VAR = 1
TOKEN_BLOCK = 2
TOKEN_COMMENT = 3

# template syntax constants
FILTER_SEPARATOR = '|'
FILTER_ARGUMENT_SEPARATOR = ':'
VARIABLE_ATTRIBUTE_SEPARATOR = '.'
BLOCK_TAG_START = '{%'
BLOCK_TAG_END = '%}'
VARIABLE_TAG_START = '{{'
VARIABLE_TAG_END = '}}'
COMMENT_TAG_START = '{#'
COMMENT_TAG_END = '#}'
SINGLE_BRACE_START = '{'
SINGLE_BRACE_END = '}'

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
# translation). Numbers are treated as variables for implementation reasons
# (so that they retain their type when passed to filters).

constant_string = r"(?:%(strdq)s|%(strsq)s)" % {
    'strdq': r'"[^"\\]*(?:\\.[^"\\]*)*"', # double-quoted string
    'strsq': r"'[^'\\]*(?:\\.[^'\\]*)*'", # single-quoted string
    }

l10nable = r"""(?:%(constant)s|%(var_chars)s)""" % {
    'constant': constant_string,
    'var_chars': "[\w\.]*",
    }

l10nable = l10nable.replace("\n", "")

filter_raw_string = r"""
^(?P<constant>%(constant)s)|
^%(i18n_open)s(?P<l10nable>%(l10nable)s)%(i18n_close)s|
^(?P<var>[%(var_chars)s]+|%(num)s)|
 (?:%(filter_sep)s
     (?P<filter_name>\w+)
         (?:%(arg_sep)s
             (?:
              (?P<constant_arg>%(constant)s)|
              %(i18n_open)s(?P<l10nable_arg>%(l10nable)s)%(i18n_close)s|
              (?P<var_arg>[%(var_chars)s]+|%(num)s)
             )
         )?
 )""" % {
    'constant': constant_string,
    'l10nable': l10nable,
    'num': r'[-+\.]?\d[\d\.e]*',
    'var_chars': "\w\." ,
    'filter_sep': re.escape(FILTER_SEPARATOR),
    'arg_sep': re.escape(FILTER_ARGUMENT_SEPARATOR),
    'i18n_open' : re.escape("_("),
    'i18n_close' : re.escape(")"),
  }

filter_re = re.compile(filter_raw_string, re.UNICODE|re.VERBOSE)

class TemplateSyntaxError(Exception):
    pass

plain_strings = []
context_strings = []

def get_translatable_filter_args(token):
    matches = filter_re.finditer(token)
    upto = 0
    var_obj = False
    for match in matches:
        start = match.start()
        if upto != start:
            raise TemplateSyntaxError("Could not parse some characters: %s|%s|%s"  % \
                    (token[:upto], token[upto:start], token[start:]))
        if not var_obj:
            l10nable = match.group("l10nable")

            if l10nable:
                if l10nable.startswith('"') and l10nable.endswith('"') or l10nable.startswith('"') and l10nable.endswith('"'):
                    # The result of the lookup should be translated at rendering
                    # time.
                    plain_strings.append(l10nable[1:-1])
            var_obj = True
        else:
            l10nable_arg = match.group("l10nable_arg")
            if  l10nable_arg:
                if l10nable_arg.startswith('"') and l10nable_arg.endswith('"') or l10nable_arg.startswith('"') and l10nable_arg.endswith('"'):
                    # The result of the lookup should be translated at rendering
                    # time.
                    plain_strings.append(l10nable_arg[1:-1])
        upto = match.end()
    if upto != len(token):
        raise TemplateSyntaxError("Could not parse the remainder: '%s' from '%s'" % (token[upto:], token))


class ContextString:
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

    def get_contextual_strings(self):
        split = []
        _bits = smart_split(self.contents)
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

            context_string = ContextString()
            context_string._string = _bit[1:-1]
            context_strings.append(context_string)
        elif _bit =="i18nc" or _bit == "i18nc_var":
            # {% i18nc "An email send operation failed." "%1 Failed!" var1 %}
            # {% i18nc_var "An email send operation failed." "%1 Failed!" var1 as result %}
            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            context_string = ContextString()
            context_string.context = _bit[1:-1]
            _bit = _bits.next()
            context_string._string = _bit[1:-1]
            context_strings.append(context_string)
        elif _bit =="i18np" or _bit =="i18np_var":
            # {% i18np "An email send operation failed." "%1 email send operations failed. Error : % 2." count count errorMsg %}
            # {% i18np_var "An email send operation failed." "%1 email send operations failed. Error : % 2." count count errorMsg as result %}
            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            context_string = ContextString()
            context_string._string = _bit[1:-1]
            _bit = _bits.next()
            context_string.plural = _bit[1:-1]
            context_strings.append(context_string)
        elif _bit =="i18ncp" or _bit =="i18ncp_var":
            # {% i18np "The user tried to send an email, but that failed." "An email send operation failed." "%1 email send operation failed." count count %}
            # {% i18np_var "The user tried to send an email, but that failed." "An email send operation failed." "%1 email send operation failed." count count as result %}

            _bit = _bits.next()
            if not _bit.startswith("'") and not _bit.startswith('"'):
                return

            sentinal = _bit[0]
            if not _bit.endswith(sentinal):
                return

            context_string = ContextString()
            context_string.context = _bit[1:-1]
            _bit = _bits.next()
            context_string._string = _bit[1:-1]
            _bit = _bits.next()
            context_string.plural = _bit[1:-1]
            context_strings.append(context_string)
        elif _bit =="trans":
            # Django compat
            pass
        elif _bit =="blocktrans":
            # Django compat
            pass
        else:
          return

        for _bit in _bits:

            if (_bit == "as"):
                return
            get_translatable_filter_args(_bit)



    def get_plain_strings(self):
        split = []
        bits = iter(smart_split(self.contents))
        for bit in bits:
            get_translatable_filter_args(bit)

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
  def translate(self, template_string, outputfile):

      for token in tokenize(template_string):
          if token.token_type == TOKEN_VAR or token.token_type == TOKEN_BLOCK:
              token.get_plain_strings()
          if token.token_type == TOKEN_BLOCK:
              token.get_contextual_strings()
      self.createOutput(plain_strings, context_strings, outputfile)

  def createOutput(self, plain_strings, context_strings, outputfile):
    pass
