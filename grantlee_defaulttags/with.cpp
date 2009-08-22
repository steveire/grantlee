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

#include "with.h"

#include <QStringList>
#include "parser.h"
#include "grantlee.h"


WithNodeFactory::WithNodeFactory()
{

}

Node* WithNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 4 or expr.at( 2 ) != "as" ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "%1 expected format is 'value as name'" ).arg( expr.at( 0 ) ) );
  }

  FilterExpression fe( expr.at( 1 ), p );
  QString name( expr.at( 3 ) );

  WithNode *n = new WithNode( fe, name );
  NodeList nodeList = p->parse( n, QStringList() << "endwith" );
  n->setNodeList( nodeList );
  p->deleteNextToken();

  return n;
}


WithNode::WithNode( FilterExpression fe, const QString &name, QObject *parent )
    : Node( parent )
{
  m_filterExpression = fe;
  m_name = name;
}

void WithNode::setNodeList( NodeList nodeList )
{
  m_list = nodeList;
}

QString WithNode::render( Context *c )
{
  c->push();
  c->insert( m_name, m_filterExpression.resolve( c ) );
  QString ret = m_list.render( c );
  c->pop();
  return ret;
}
