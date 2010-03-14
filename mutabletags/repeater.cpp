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

#include "repeater.h"
#include "parser.h"


RepeaterNodeFactory::RepeaterNodeFactory( QObject* parent )
    : AbstractNodeFactory( parent )
{

}

Node* RepeaterNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  Q_UNUSED( tagContent )
  RepeaterNode *n = new RepeaterNode( p );
  NodeList nodeList = p->parse( n, "endrepeater" );
  p->deleteNextToken();
  n->setNodeList( nodeList );
  return n;
}

RepeaterNode::RepeaterNode( QObject* parent )
    : Node( parent )
{

}

void RepeaterNode::setNodeList( NodeList nodeList )
{
  m_nodeList = nodeList;
}

void RepeaterNode::render( OutputStream *stream, Context* c )
{
  c->setMutating( false );
  m_nodeList.render( stream, c );
  c->setMutating( true );
}

