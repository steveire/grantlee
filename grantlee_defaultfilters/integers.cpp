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

#include "integers.h"

#include "util_p.h"

AddFilter::AddFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString AddFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return QString::number( QVariant( Util::getSafeString( input ) ).toInt() + QVariant( argument ).toInt() );
}

GetDigitFilter::GetDigitFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString GetDigitFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariant variant = QVariant( Util::getSafeString( input ) );

  if ( !variant.canConvert( QVariant::Int ) || variant.convert( QVariant::Int ) )
    return QString();

  QString value = variant.toString();

  if ( value.size() < 1 )
    return value;

  int arg = QVariant( argument ).toInt();

  if ( value.size() < arg )
    return SafeString( "0" );

  return SafeString( value.at( arg ) );
}
