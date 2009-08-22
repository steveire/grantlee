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

#include "now.h"

#include <QStringList>
#include <QDateTime>

#include "parser.h"



NowNodeFactory::NowNodeFactory()
{

}

Node* NowNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( "\"", QString::KeepEmptyParts );

  if ( expr.size() != 3 ) {
    throw Grantlee::Exception( TagSyntaxError, "now tag takes one argument" );
  }

  QString formatString = expr.at( 1 );

  return new NowNode( formatString );
}

NowNode::NowNode( const QString &formatString, QObject *parent )
    : Node( parent ), m_formatString( formatString )
{
}

QString NowNode::render( Context *c )
{
  return QDateTime::currentDateTime().toString( m_formatString );
}

