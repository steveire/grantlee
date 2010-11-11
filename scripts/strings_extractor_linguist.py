#! /usr/bin/env python
# -*- coding: utf-8 -*-

from strings_extractor import TranslationOutputter

class LinguistExtractStrings(TranslationOutputter):
  def createOutput(self, plain_strings, context_strings, outputfile):

    for plain_string in plain_strings:
      outputfile.write("QCoreApplication::translate(\"GR_FILENAME\", \"" + plain_string + "\");\n")
    for context_string in context_strings:
      if context_string.context:
        if not context_string.plural:
          outputfile.write("QCoreApplication::translate(\"GR_FILENAME\", \"" + context_string._string + "\", \"" + context_string.context + "\");\n")
        else:
          outputfile.write("QCoreApplication::translate(\"GR_FILENAME\", \"" + context_string._string + "\", \"" + context_string.context + "\", count );\n")
      else:
        if context_string.plural:
          outputfile.write("QCoreApplication::translate(\"GR_FILENAME\", \"" + context_string._string + "\", \"\", count);\n")
        else:
          outputfile.write("QCoreApplication::translate(\"GR_FILENAME\", \"" + context_string._string + "\");\n")

