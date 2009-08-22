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

#include "raw.h"
#include <QStringList>
#include "parser.h"
#include "grantlee.h"

RawNodeFactory::RawNodeFactory()
{

}

Node* RawNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  RawNode *n = new RawNode();
  NodeList nodeList = p->parse( n, "endraw" );
  n->setNodeList( nodeList );
  p->deleteNextToken();
  return n;
}


RawNode::RawNode( QObject* parent )
    : Node( parent )
{

}

void RawNode::setNodeList( NodeList nodeList )
{
  m_nodeList = nodeList;
}

QString RawNode::render( Context* c )
{
  c->setMutating( false );
  QString result = m_nodeList.render( c );
  c->setMutating( true );
  return result;
}

