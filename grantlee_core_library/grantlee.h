/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef GRANTLEE_H
#define GRANTLEE_H

namespace Grantlee
{

enum LoopDirection
{
  ForwardLoop,
  ReversedLoop
};

enum Error
{
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
