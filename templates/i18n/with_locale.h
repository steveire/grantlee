/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef WITHLOCALENODE_H
#define WITHLOCALENODE_H

#include "node.h"

using namespace Grantlee;

class WithLocaleNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  WithLocaleNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class WithLocaleNode : public Node
{
  Q_OBJECT
public:
  WithLocaleNode( const FilterExpression &localeName, QObject *parent = 0 );

  void setNodeList( NodeList nodeList );

  void render( OutputStream *stream, Context *c );

private:
  FilterExpression m_localeName;
  NodeList m_list;

};

#endif

