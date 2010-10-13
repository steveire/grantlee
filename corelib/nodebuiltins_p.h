/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef NODE_BUILTINS_H
#define NODE_BUILTINS_H

#include "node.h"

namespace Grantlee
{

/**
  @internal

  A Node for plain text. Plain text is everything between variables, comments and template tags.
*/
class GRANTLEE_CORE_EXPORT TextNode : public Node
{
  Q_OBJECT
public:
  explicit TextNode( const QString &content, QObject *parent = 0 );

  /* reimp */ void render( OutputStream *stream, Context *c ) { // krazy:exclude:inline
    Q_UNUSED( c );
    ( *stream ) << m_content;
  }

  void appendContent( const QString &content ) { // krazy:exclude:inline
    m_content += content;
  }

private:
  QString m_content;
};

/**
  @internal

  A node for a variable or filter expression substitution.
*/
class GRANTLEE_CORE_EXPORT VariableNode : public Node
{
  Q_OBJECT
public:
  explicit VariableNode( const FilterExpression &fe, QObject *parent = 0 );

  /* reimp */ void render( OutputStream *stream, Context *c );

private:
  FilterExpression m_filterExpression;
};

}

#endif
