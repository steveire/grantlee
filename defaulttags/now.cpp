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

#include "now.h"

#include "exception.h"
#include "parser.h"

#include <QtCore/QDateTime>


NowNodeFactory::NowNodeFactory()
{

}

Node* NowNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( QLatin1Char( '"' ), QString::KeepEmptyParts );

  if ( expr.size() != 3 ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "now tag takes one argument" ) );
  }

  QString formatString = expr.at( 1 );

  return new NowNode( formatString, p );
}

NowNode::NowNode( const QString &formatString, QObject *parent )
    : Node( parent ), m_formatString( formatString )
{
}

void NowNode::render( OutputStream *stream, Context *c )
{
  Q_UNUSED( c )
  ( *stream ) << QDateTime::currentDateTime().toString( m_formatString );
}

