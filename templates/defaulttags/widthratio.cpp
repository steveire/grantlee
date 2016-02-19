/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "widthratio.h"

#include "../lib/exception.h"
#include "parser.h"


WidthRatioNodeFactory::WidthRatioNodeFactory()
{

}

Node* WidthRatioNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 4 ) {
    throw Grantlee::Exception( TagSyntaxError, QStringLiteral( "widthratio takes three arguments" ) );
  }
  FilterExpression valExpr( expr.at( 1 ), p );
  FilterExpression maxExpr( expr.at( 2 ), p );
  FilterExpression maxWidth( expr.at( 3 ), p );

  return new WidthRatioNode( valExpr, maxExpr, maxWidth, p );
}


WidthRatioNode::WidthRatioNode(const FilterExpression& valExpr,
                               const FilterExpression& maxExpr,
                               const FilterExpression& maxWidth,
                               QObject *parent )
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

void WidthRatioNode::render( OutputStream *stream, Context *c ) const
{
  QVariant thisVal = m_valExpr.resolve( c );
  QVariant maxVal = m_maxExpr.resolve( c );
  if ( !thisVal.isValid() || !maxVal.isValid() )
    return;

  qreal tv = thisVal.value<double>();
  qreal mv = maxVal.value<double>();

  if ( mv == 0 )
    return;

  int maxWidth = m_maxWidth.resolve( c ).value<int>();

  qreal result = ( tv / mv ) * maxWidth;

  result = round( result );

  // TODO put integral streamers in OutputStream?
  ( *stream ) << QString::number( result );
}
