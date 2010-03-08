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

#include "spaceless.h"

#include <QStringList>
#include "parser.h"

SpacelessNodeFactory::SpacelessNodeFactory()
{

}

Node* SpacelessNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  Q_UNUSED( tagContent )
  SpacelessNode *n = new SpacelessNode( p );
  NodeList list = p->parse( n, QStringList() << "endspaceless" );
  n->setList( list );
  p->deleteNextToken();
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

  QRegExp re( ">\\s+<" );
  stripped.replace( re, "><" );
  return stripped;
}


void SpacelessNode::render( OutputStream *stream, Context *c )
{
  QString output;
  QTextStream textStream( &output );
  OutputStream temp( &textStream );
  m_nodeList.render( &temp, c );
  ( *stream ) << stripSpacesBetweenTags( output.trimmed() );
}

