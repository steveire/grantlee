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

#include "load.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"


LoadNodeFactory::LoadNodeFactory()
{
}

Node* LoadNodeFactory::getNode( const QString &tagContent, Parser *p, QObject *parent ) const
{
  QStringList expr = tagContent.split( " ", QString::SkipEmptyParts );

  expr.takeAt( 0 );

  if ( expr.size() <= 0 ) {
    setError( TagSyntaxError, QString( "%1 expects at least one argument" ).arg( "expr.at(0)" ) );
    return 0;
  }

  QListIterator<QString> i( expr );
  while ( i.hasNext() ) {
    QString libName = i.next();
    p->loadLib( libName );
  }

  return new LoadNode( parent );
}


LoadNode::LoadNode( QObject *parent )
    : Node( parent )
{
}

QString LoadNode::render( Context *c )
{
  return QString();
}

