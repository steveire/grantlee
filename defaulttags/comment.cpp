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

#include "comment.h"

#include <QtCore/QStringList>
#include <QtPlugin>

#include "parser.h"


CommentNodeFactory::CommentNodeFactory()
{

}

Node* CommentNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  Q_UNUSED( tagContent );

  p->skipPast( "endcomment" );

  return new CommentNode( p );
}

CommentNode::CommentNode( QObject *parent )
    : Node( parent )
{

}

void CommentNode::render( OutputStream *stream, Context *c )
{
  Q_UNUSED( stream );
  Q_UNUSED( c );
}

