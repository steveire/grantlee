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

class LexerPrivate;

namespace Grantlee
{

class Lexer
{
public:
  Lexer(const QString &templateString);

  QList<Token> tokenize();

// protected:
//   Token createToken( const QString &fragment, bool inTag);

private:
  Q_DECLARE_PRIVATE(Lexer);
  LexerPrivate *d_ptr;

};

}

#endif
