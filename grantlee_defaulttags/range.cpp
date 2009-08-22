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

#include "range.h"

#include "parser.h"

#include "grantlee.h"

#include "util_p.h"
#include "engine.h"


RangeNodeFactory::RangeNodeFactory()
{

}

Grantlee::Node* RangeNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  expr.takeAt( 0 );
  int numArgs = expr.size();
  if ( numArgs <= 0 ) {
    setError( TagSyntaxError, "'range' tag requires at least one argument" );
    return 0;
  }

  QString name;
  if ( numArgs >= 3 && expr.at( numArgs - 3 ) == "as" ) {
    name = expr.at( numArgs - 2 );
    numArgs -= 2;
  }

  RangeNode *n = 0;

  switch ( numArgs ) {
  case 1:
    n = new RangeNode( name, FilterExpression( expr.at( 0 ), p ) );
  case 2:
    n = new RangeNode( name, FilterExpression( expr.at( 0 ), p ), FilterExpression( expr.at( 1 ), p ) );
  case 3:
    n = new RangeNode( name, FilterExpression( expr.at( 0 ), p ), FilterExpression( expr.at( 1 ), p ), FilterExpression( expr.at( 2 ), p ) );
  default:
    return 0;
  }
  Q_ASSERT( n );
  NodeList list = p->parse( n, "endrange" );
  p->deleteNextToken();

  n->setNodeList( list );
  return n;

}

RangeNode::RangeNode( const QString &name, FilterExpression startOrStopExpression, FilterExpression stopExpression, FilterExpression stepExpression, QObject* parent )
    : Node( parent ),
    m_name( name ),
    m_startOrStopExpression( startOrStopExpression ),
    m_stopExpression( stopExpression ),
    m_stepExpression( stepExpression )
{
}

void RangeNode::setNodeList( NodeList list )
{
  m_list = list;
}

QString RangeNode::render( Context* c )
{
  int start;
  int stop;
  int step;

  if ( m_stopExpression.isValid() ) {
    start = m_startOrStopExpression.resolve( c ).toInt();
    stop = m_stopExpression.resolve( c ).toInt();
  } else {
    start = 0;
    stop = m_startOrStopExpression.resolve( c ).toInt();
  }

  if ( m_stepExpression.isValid() ) {
    step = m_stepExpression.resolve( c ).toInt();
  } else {
    step = 1;
  }

  const bool insertContext = !m_name.isEmpty();

  Q_ASSERT( start < stop );

  QString ret;
  for ( int i = start; start < stop; start += step ) {
    if ( insertContext ) {
      c->push();
      c->insert( m_name, i );
    }
    ret += m_list.render( c );
    if ( insertContext )
      c->pop();
  }
  return ret;
}
