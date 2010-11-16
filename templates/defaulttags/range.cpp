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

#include "range.h"

#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "util.h"


RangeNodeFactory::RangeNodeFactory()
{

}

Grantlee::Node* RangeNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  expr.takeAt( 0 );
  int numArgs = expr.size();
  if ( numArgs != 1 ) {
    if ( numArgs <= 2 ) {
      throw Grantlee::Exception( TagSyntaxError, QLatin1String( "'range' tag requires at least three arguments" ) );
    }

    if ( expr.at( numArgs - 2 ) != QLatin1String( "as" ) ) {
      throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Invalid arguments to 'range' tag" ) );
    }
  }

  const QString name = ( numArgs > 2 ) ? expr.at( numArgs - 1 ) : QString();
  if ( numArgs > 2 )
    numArgs -= 2;

  RangeNode *n = 0;

  switch ( numArgs ) {
  case 1:
    n = new RangeNode( name, FilterExpression( QChar::fromLatin1( '0' ), p ), FilterExpression( expr.first(), p ), p );
    break;
  case 2:
    n = new RangeNode( name, FilterExpression( expr.first(), p ), FilterExpression( expr.at( 1 ), p ), p );
    break;
  case 3:
    n = new RangeNode( name, FilterExpression( expr.first(), p ), FilterExpression( expr.at( 1 ), p ), FilterExpression( expr.at( 2 ), p ), p );
    break;
  default:
    return 0;
  }

  NodeList list = p->parse( n, QLatin1String( "endrange" ) );
  p->removeNextToken();

  n->setNodeList( list );
  return n;
}

RangeNode::RangeNode( const QString &name, const FilterExpression &startExpression, const FilterExpression &stopExpression, QObject* parent )
  : Node( parent ),
    m_name( name ),
    m_startExpression( startExpression ),
    m_stopExpression( stopExpression )
{
}

RangeNode::RangeNode( const QString &name, const FilterExpression &startExpression, const FilterExpression &stopExpression, const FilterExpression &stepExpression, QObject* parent )
  : Node( parent ),
    m_name( name ),
    m_startExpression( startExpression ),
    m_stopExpression( stopExpression ),
    m_stepExpression( stepExpression )
{
}

void RangeNode::setNodeList( NodeList list )
{
  m_list = list;
}

void RangeNode::render( OutputStream *stream, Context* c ) const
{
  int start;
  int stop;
  int step;

  start = m_startExpression.resolve( c ).toInt();
  stop = m_stopExpression.resolve( c ).toInt();

  if ( m_stepExpression.isValid() ) {
    step = m_stepExpression.resolve( c ).toInt();
  } else {
    step = 1;
  }

  const bool insertContext = !m_name.isEmpty();

  Q_ASSERT( start < stop );

  QString ret;
  for ( int i = start; i < stop; i += step ) {
    if ( insertContext ) {
      c->push();
      c->insert( m_name, i );
    }
    m_list.render( stream, c );
    if ( insertContext )
      c->pop();
  }
}
