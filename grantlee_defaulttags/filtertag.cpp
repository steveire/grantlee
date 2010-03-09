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

#include "filtertag.h"
#include "parser.h"
#include "filterexpression.h"
#include "exception.h"

#include "util_p.h"

FilterNodeFactory::FilterNodeFactory()
{

}

Grantlee::Node* FilterNodeFactory::getNode( const QString& tagContent, Grantlee::Parser* p ) const
{
  QStringList expr = tagContent.split( ' ', QString::SkipEmptyParts );

  expr.removeFirst();

  QString expression = expr.join( " " );
  FilterExpression fe( QString( "var|%1" ).arg( expression ), p );

  QStringList filters = fe.filters();
  if ( filters.contains( "safe" ) || filters.contains( "escape" ) ) {
    throw Grantlee::Exception( TagSyntaxError, "Use the \"autoescape\" tag instead." );
  }

  FilterNode *n = new FilterNode( fe, p );

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

void FilterNode::render( OutputStream *stream, Context* c )
{
  QString output;
  QTextStream textStream( &output );
  QSharedPointer<OutputStream> temp = stream->clone( &textStream );
  m_filterList.render( temp.data(), c );
  c->push();
  c->insert( "var", output );
  m_fe.resolve( stream, c );
  c->pop();
}

