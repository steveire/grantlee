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

#include "load.h"

#include <QStringList>
#include "parser.h"
#include "exception.h"


LoadNodeFactory::LoadNodeFactory()
{
}

Node* LoadNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( ' ', QString::SkipEmptyParts );

  if ( expr.size() <= 1 ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "%1 expects at least one argument" ).arg( expr.at( 0 ) ) );
  }

  expr.takeAt( 0 );

  QListIterator<QString> i( expr );
  while ( i.hasNext() ) {
    QString libName = i.next();
    p->loadLib( libName );
  }

  return new LoadNode( p );
}


LoadNode::LoadNode( QObject *parent )
    : Node( parent )
{
}

void LoadNode::render( OutputStream *stream, Context *c )
{
  Q_UNUSED( stream )
  Q_UNUSED( c )
}

