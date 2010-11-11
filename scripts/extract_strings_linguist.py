#! /usr/bin/env python
# -*- coding: utf-8 -*-

from strings_extractor_linguist import LinguistExtractStrings
import os, sys, glob, operator

if __name__ == "__main__":
  ex = LinguistExtractStrings()

  outputfile = open(sys.argv[1], "w")

  files = reduce(operator.add, map(glob.glob, sys.argv[2:]))

  for filename in files:
    f = open(filename, "r")
    stri = f.read()
    ex.translate(stri, outputfile)

  outputfile.write("\n")
  outputfile.close()



