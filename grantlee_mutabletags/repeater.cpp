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

#include "repeater.h"
#include "parser.h"


RepeaterNodeFactory::RepeaterNodeFactory( QObject* parent )
    : AbstractNodeFactory( parent )
{

}

Node* RepeaterNodeFactory::getNode( const QString& tagContent, Parser* p, QObject* parent ) const
{
  NodeList nodeList = p->parse( "endrepeater", parent );
  return new RepeaterNode( nodeList, parent );
}

RepeaterNode::RepeaterNode( NodeList nodeList, QObject* parent )
    : Node( parent ), m_nodeList( nodeList )
{

}

QString RepeaterNode::render( Context* c )
{
  c->setMutating( false );
  QString result = m_nodeList.render( c );
  c->setMutating( true );
  return result;
}

