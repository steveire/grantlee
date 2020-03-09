#!/usr/bin/env python

import os

handle = os.popen("for f in `git ls-files textdocument/*.cpp textdocument/*.h templates/*.cpp templates/*.h`; do clang-format $f -i -style=file;  done;")
