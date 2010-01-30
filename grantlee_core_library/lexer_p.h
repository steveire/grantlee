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

#ifndef GRANTLEE_LEXER_P_H
#define GRANTLEE_LEXER_P_H

#include <QtCore/QList>
#include <QtCore/QString>

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

