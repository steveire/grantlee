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

#include "widthratio.h"

#include <QStringList>
#include "parser.h"

#include "exception.h"


WidthRatioNodeFactory::WidthRatioNodeFactory()
{

}

Node* WidthRatioNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 4 ) {
    throw Grantlee::Exception( TagSyntaxError, "widthratio takes three arguments" );
  }
  FilterExpression valExpr( expr.at( 1 ), p );
  FilterExpression maxExpr( expr.at( 2 ), p );
  FilterExpression maxWidth( expr.at( 3 ), p );

  return new WidthRatioNode( valExpr, maxExpr, maxWidth, p );
}


WidthRatioNode::WidthRatioNode( FilterExpression valExpr, FilterExpression maxExpr, FilterExpression maxWidth, QObject *parent )
    : Node( parent )
{
  m_valExpr = valExpr;
  m_maxExpr = maxExpr;
  m_maxWidth = maxWidth;
}

int WidthRatioNode::round( qreal number )
{
  int intPart = ( int )number;
  if ( number < ( intPart + 0.5 ) )
    return intPart;
  return intPart + 1;
}

QString WidthRatioNode::render( Context *c )
{
  QVariant thisVal = m_valExpr.resolve( c );
  QVariant maxVal = m_maxExpr.resolve( c );
  if ( !thisVal.isValid() || !maxVal.isValid() )
    return QString();

  qreal tv = thisVal.toDouble();
  qreal mv = maxVal.toDouble();

  if ( mv == 0 )
    return QString();

  int maxWidth = m_maxWidth.resolve( c ).toInt();

  qreal result = ( tv / mv ) * maxWidth;

  result = round( result );

  return QString::number( result );
}

