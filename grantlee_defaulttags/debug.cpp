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

#include "debug.h"

#include <QStringList>
#include "parser.h"
#include "grantlee.h"


DebugNodeFactory::DebugNodeFactory()
{

}

Node* DebugNodeFactory::getNode( const QString &tagContent, Parser *p, QObject *parent ) const
{
  return new DebugNode( parent );
}


DebugNode::DebugNode( QObject *parent )
    : Node( parent )
{

}

QString DebugNode::render( Context *c )
{
  QString ret;
  int i = 0;
  QHash<QString, QVariant> h = c->stackHash( i++ );

  ret += "\n\nContext:\n";
  while ( !h.isEmpty() ) {
    QHashIterator<QString, QVariant> it( h );
    while ( it.hasNext() ) {
      it.next();
      ret += "key " + it.key() + ", ";
      ret += "type ";
      ret.append( it.value().typeName() );
      ret += "\n";
    }
    h = c->stackHash( i++ );
  }

  ret += "End context:\n\n";

  return ret;
}

