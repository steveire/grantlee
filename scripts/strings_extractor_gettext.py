#! /usr/bin/env python
# -*- coding: utf-8 -*-

from strings_extractor import TranslationOutputter

class GettextExtractStrings(TranslationOutputter):
  def createOutput(self, plain_strings, context_strings, outputfile):

    for plain_string in plain_strings:
      outputfile.write("gettext(\"" + plain_string + "\");\n")
    for context_string in context_strings:
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


