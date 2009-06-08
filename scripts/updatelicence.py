#!/usr/bin/env python
#-*- coding: utf-8 -*-


import os, fnmatch

licence = """/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/"""

def updateFile(filename):
  f = open(filename, "r")

  content = f.read()
  f.close()
  content = content.strip()

  if content.startswith("/*"):
    bit, commentplus = content.split("/*", 1)
    comment, after = commentplus.split("*/", 1)

    f = open(filename, "w")
    f.write(bit + licence + after + "\n\n")
    f.close()
  else:
    f = open(filename, "w")
    f.write(licence + "\n\n" +  content + "\n\n")
    f.close()

# http://code.activestate.com/recipes/499305/
def locate(pattern, root=os.curdir):
  '''Locate all files matching supplied filename pattern in and below
  supplied root directory.'''
  for path, dirs, files in os.walk(os.path.abspath(root)):
    for filename in fnmatch.filter(files, pattern):
      yield os.path.join(path, filename)

if __name__ == "__main__":
  for filename in locate("*.cpp"):
    updateFile(filename)

  for filename in locate("*.h"):
    updateFile(filename)
