/*
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

*/

#ifndef GRANTLEE_H
#define GRANTLEE_H

namespace Grantlee
{

enum LoopDirection {
  ForwardLoop,
  ReversedLoop
};

enum Error {
  NoError,
  VariableStringInvalid,
  VariableNotInContext,
  ObjectReturnTypeInvalid,
  UnknownError,
  TagSyntaxError,
  EmptyVariableError,
  EmptyBlockTagError,
  InvalidBlockTagError,
  UnclosedBlockTagError,
  CompileFunctionError
};

static const char * BLOCK_TAG_START = "{%";
static const char * BLOCK_TAG_END = "%}";
static const char * VARIABLE_TAG_START = "{{";
static const char * VARIABLE_TAG_END = "}}";
static const char * COMMENT_TAG_START = "{#";
static const char * COMMENT_TAG_END = "#}";
static const char * SINGLE_BRACE_START = "{";
static const char * SINGLE_BRACE_END = "}";

}

#endif

