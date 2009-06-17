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

Grantlee::Node* FilterNodeFactory::getNode( const QString& tagContent, Grantlee::Parser* p, QObject *parent ) const
{
  QStringList expr = tagContent.split( " ", QString::SkipEmptyParts );

  expr.removeFirst();

  QString expression = expr.join( " " );
  FilterExpression fe( QString( "var|%1" ).arg( expression ), p );

  if ( fe.error() ) {
    setError( fe.error(), fe.errorString() );
    return 0;
  }

  QStringList filters = fe.filters();
  if ( filters.contains( "safe" ) || filters.contains( "escape" ) )
  {
    setError( TagSyntaxError, "Use the \"autoescape\" tag instead." );
    return 0;
  }

  NodeList filterNodes = p->parse( QStringList() << "endfilter", parent );
  p->deleteNextToken();

  return new FilterNode( fe, filterNodes, parent );
}

FilterNode::FilterNode( FilterExpression fe, NodeList filterList, QObject *parent )
    : Node( parent ), m_fe( fe ), m_filterList( filterList )
{

}

QString FilterNode::render( Context* c )
{
  QString output = m_filterList.render( c );
  c->push();
  c->insert( "var", output );
  QString filtered = Util::getSafeString( m_fe.resolve( c ) ).rawString();
  c->pop();
  return filtered;
}

NodeList FilterNode::getNodesByType( const char* className )
{
  return m_filterList.getNodesByType( className );
}

