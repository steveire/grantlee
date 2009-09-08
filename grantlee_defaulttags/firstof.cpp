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

#include "firstof.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"

#include "util_p.h"


FirstOfNodeFactory::FirstOfNodeFactory()
{
}

Node* FirstOfNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  expr.takeAt( 0 );

  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "%1 expects at least one argument" ).arg( "expr.at(0)" ) );
  }

  return new FirstOfNode( getFilterExpressionList( expr, p ), p );
}


FirstOfNode::FirstOfNode( QList<FilterExpression> list, QObject *parent )
    : Node( parent ), m_variableList( list )
{
}

QString FirstOfNode::render( Context *c )
{
  foreach( FilterExpression fe, m_variableList ) {
    if ( fe.isTrue( c ) ) {
      return Util::getSafeString( fe.resolve( c ) );
    }
  }
  return QString();
}

