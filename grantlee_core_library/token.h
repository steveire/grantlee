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

/**
  A token in a parse stream for a template.

  This class is only relevant for template tag implementations.
*/
struct Token {
  int tokenType;
  QString content;
};

}

#endif

