#! /usr/bin/env python
# -*- coding: utf-8 -*-

##
# Copyright 2010 Stephen Kelly <steveire@gmail.com>
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

from strings_extractor import TranslationOutputter

class GettextExtractStrings(TranslationOutputter):
  def createOutput(self, template_filename, context_strings, outputfile):
    for context_string in context_strings:
      outputfile.write("// i18n: file: " + template_filename + "\n")
      if context_string.context:
        if not context_string.plural:
          outputfile.write("pgettext(\"" + context_string.context + "\", \"" + context_string._string + "\");\n")
        else:
          outputfile.write("npgettext(\"" + context_string.context + "\", \"" + context_string._string + "\", \"" + context_string.plural + "\");\n")
      else:
        if context_string.plural:
          outputfile.write("ngettext(\"" + context_string._string + "\", \"" + context_string.plural + "\");\n")
        else:
          outputfile.write("gettext(\"" + context_string._string + "\");\n")


