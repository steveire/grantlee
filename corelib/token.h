/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_TOKEN_H
#define GRANTLEE_TOKEN_H

#include <QtCore/QString>

namespace Grantlee
{

/**
  @internal

  The available token types.
*/
enum TokenType {
  TextToken,
  VariableToken,
  BlockToken,
  CommentToken
};

/// @headerfile token.h grantlee/token.h

/**
  A token in a parse stream for a template.

  This class is only relevant for template tag implementations.
*/
struct Token {
  int tokenType;    ///< The Type of this Token
  QString content;  ///< The content of this Token
};

}

Q_DECLARE_TYPEINFO(Grantlee::Token, Q_MOVABLE_TYPE);

#endif

