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

#include "filtertag.h"
#include "parser.h"
#include "filterexpression.h"

#include "util_p.h"

FilterNodeFactory::FilterNodeFactory()
{

}

Grantlee::Node* FilterNodeFactory::getNode( const QString& tagContent, Grantlee::Parser* p ) const
{
  QStringList expr = tagContent.split( " ", QString::SkipEmptyParts );

  expr.removeFirst();

  QString expression = expr.join( " " );
  FilterExpression fe( QString( "var|%1" ).arg( expression ), p );

  QStringList filters = fe.filters();
  if ( filters.contains( "safe" ) || filters.contains( "escape" ) ) {
    throw Grantlee::Exception( TagSyntaxError, "Use the \"autoescape\" tag instead." );
  }

  FilterNode *n = new FilterNode( fe );

  NodeList filterNodes = p->parse( n, QStringList() << "endfilter" );
  p->deleteNextToken();

  n->setNodeList( filterNodes );
  return n;
}

FilterNode::FilterNode( FilterExpression fe, QObject *parent )
    : Node( parent ), m_fe( fe )
{

}

void FilterNode::setNodeList( NodeList filterList )
{
  m_filterList = filterList;
}

QString FilterNode::render( Context* c )
{
  QString output = m_filterList.render( c );
  c->push();
  c->insert( "var", output );
  QString filtered = Util::getSafeString( m_fe.resolve( c ) );
  c->pop();
  return filtered;
}
