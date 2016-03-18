#!/usr/bin/env python

import os

handle = os.popen("for f in `git ls-files textdocument/*.cpp textdocument/*.h templates/*.cpp templates/*.h`; do clang-format-3.8 $f -i -style=file;  done;")

handle = os.popen("""/usr/bin/git grep -l QStri""" + """ngLiteral""", "r")

lines = handle.read().splitlines()

def fixClangFormat(input):

  output = ""

  start = 0

  while start < len(input):
    found = input.find("QStri" + "ngLiteral(", start)
    if (found < 0):
      break
    beg = found + len("QStrin" + "gLiteral(")

    quoteLoc = input.find("\"", beg)
    closeLoc = input.find(")", beg)
    if (quoteLoc < 0 or closeLoc < quoteLoc):
      output += input[start:closeLoc]
      start = closeLoc
      continue

    output += input[start:beg]

    numFragments = 0
    origCloseLoc = closeLoc
    origQuoteLoc = quoteLoc

    quoteLocStart = beg
    while closeLoc > quoteLoc:
      numFragments += 1
      endQuoteLoc = input.find("\"", quoteLoc + 1)
      while True:
        numEscapes = 0
        escStart = endQuoteLoc - 1
        while(input[escStart] == "\\"):
          numEscapes += 1
          escStart -= 1
        if (numEscapes % 2 != 0):
          endQuoteLoc = input.find("\"", endQuoteLoc + 1)
        else:
          break

      closeLoc = input.find(")", endQuoteLoc + 1)
      quoteLoc = input.find("\"", endQuoteLoc + 1)
      if (quoteLoc < 0):
        break

    if (numFragments == 1):
      output += input[beg:closeLoc]
      start = closeLoc
      continue

    output += "\""

    closeLoc = origCloseLoc
    quoteLoc = origQuoteLoc

    quoteLocStart = beg
    while closeLoc > quoteLoc:
      endQuoteLoc = input.find("\"", quoteLoc + 1)
      while True:
        numEscapes = 0
        escStart = endQuoteLoc - 1
        while(input[escStart] == "\\"):
          numEscapes += 1
          escStart -= 1
        if (numEscapes % 2 != 0):
          endQuoteLoc = input.find("\"", endQuoteLoc + 1)
        else:
          break

      output += input[quoteLoc + 1:endQuoteLoc]

      closeLoc = input.find(")", endQuoteLoc + 1)
      quoteLoc = input.find("\"", endQuoteLoc + 1)
      if (quoteLoc < 0 or closeLoc < quoteLoc):
        output += "\""
      if (quoteLoc < 0):
        break

    start = closeLoc

  output += input[start:]

  return output

for line in lines:
  f = open(line)
  all = fixClangFormat(f.read())
  f = open(line, "w")
  f.write(all)
