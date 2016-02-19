/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#include "with_locale.h"

#include "../lib/exception.h"
#include "parser.h"
#include <util.h>

#include <QDebug>
#include <abstractlocalizer.h>

WithLocaleNodeFactory::WithLocaleNodeFactory()
{

}

Node* WithLocaleNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 2 ) {
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "%1 expected format is for example 'with_locale \"de_DE\"'" ).arg( expr.first() ) );
  }

  FilterExpression fe( expr.at( 1 ), p );

  WithLocaleNode *n = new WithLocaleNode( fe, p );
  NodeList nodeList = p->parse( n, QStringLiteral( "endwith_locale" ) );
  n->setNodeList( nodeList );
  p->removeNextToken();

  return n;
}


WithLocaleNode::WithLocaleNode( const FilterExpression &localeName, QObject *parent )
  : Node( parent ), m_localeName( localeName )
{
}

void WithLocaleNode::setNodeList(const NodeList& nodeList )
{
  m_list = nodeList;
}

void WithLocaleNode::render( OutputStream *stream, Context *c ) const
{
  const QString name = Grantlee::getSafeString( m_localeName.resolve( c ) ).get();

  c->push();
  c->localizer()->pushLocale( name );
  m_list.render( stream, c );
  c->localizer()->popLocale();
  c->pop();
}
