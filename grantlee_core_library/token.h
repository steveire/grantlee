/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

enum TokenType {
  TextToken,
  VariableToken,
  BlockToken,
  CommentToken
};

struct Token {
  int tokenType;
  QString content;
};


#endif

