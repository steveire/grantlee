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

#include "raw.h"
#include <QtCore/QStringList>
#include "parser.h"

RawNodeFactory::RawNodeFactory()
{

}

Node* RawNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  Q_UNUSED( tagContent )
  RawNode *n = new RawNode( p );
  NodeList nodeList = p->parse( n, QLatin1String( "endraw" ) );
  n->setNodeList( nodeList );
  p->removeNextToken();
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

void RawNode::render( OutputStream *stream, Context* c )
{
  c->setMutating( false );
  m_nodeList.render( stream, c );
  c->setMutating( true );
}

