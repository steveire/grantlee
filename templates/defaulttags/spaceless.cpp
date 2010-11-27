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

#include "spaceless.h"

#include "parser.h"
#include "util.h"

SpacelessNodeFactory::SpacelessNodeFactory()
{

}

Node* SpacelessNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  Q_UNUSED( tagContent )
  SpacelessNode *n = new SpacelessNode( p );
  NodeList list = p->parse( n, QLatin1String( "endspaceless" ) );
  n->setList( list );
  p->removeNextToken();
  return n;
}

SpacelessNode::SpacelessNode( QObject *parent )
    : Node( parent )
{

}

void SpacelessNode::setList( NodeList nodeList )
{
  m_nodeList = nodeList;
}

QString SpacelessNode::stripSpacesBetweenTags( const QString& input )
{
  QString stripped = input;

  static const QRegExp re( QLatin1String( ">\\s+<" ) );
  stripped.replace( re, QLatin1String( "><" ) );
  return stripped;
}


void SpacelessNode::render( OutputStream *stream, Context *c )
{
  QString output;
  QTextStream textStream( &output );
  QSharedPointer<OutputStream> temp = stream->clone( &textStream );
  m_nodeList.render( temp.data(), c );
  ( *stream ) << markSafe( stripSpacesBetweenTags( output.trimmed() ) );
}

