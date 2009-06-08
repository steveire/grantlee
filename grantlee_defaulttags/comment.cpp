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

#include "comment.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"


CommentNodeFactory::CommentNodeFactory()
{

}

Node* CommentNodeFactory::getNode( const QString &tagContent, Parser *p, QObject *parent ) const
{
  Q_UNUSED( tagContent );

  p->skipPast( "endcomment" );

  return new CommentNode( parent );
}

CommentNode::CommentNode( QObject *parent )
    : Node( parent )
{

}

QString CommentNode::render( Context *c )
{
  Q_UNUSED( c );
  return QString();
}

