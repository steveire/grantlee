/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include <QString>

enum TokenType
{
  TextToken,
  VariableToken,
  BlockToken,
  CommentToken
};

struct Token
{
  int tokenType;
  QString content;
};

namespace Grantlee
{

class Lexer
{
public:
  Lexer(const QString &templateString);
  ~Lexer();

  QList<Token> tokenize();

private:
  enum State
  {
    InTag,
    NotInTag
  };

  Token createToken( const QString &fragment, int inTag);

  QString m_templateString;

};

}

#endif
