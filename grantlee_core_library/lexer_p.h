/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include <QString>

#include "token.h"

namespace Grantlee
{

class Lexer
{
public:
  Lexer( const QString &templateString );
  ~Lexer();

  QList<Token> tokenize() const;

private:
  enum State {
    InTag,
    NotInTag
  };

  Token createToken( const QString &fragment, int inTag ) const;

  QString m_templateString;

};

}

#endif
