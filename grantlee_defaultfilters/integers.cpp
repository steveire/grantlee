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

AddFilter::AddFilter()
{
}

QVariant AddFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).toInt() + Util::getSafeString( argument ).toInt();
}

GetDigitFilter::GetDigitFilter()
{
}

QVariant GetDigitFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QVariant variant = QVariant( Util::getSafeString( input ) );

  if ( !variant.canConvert( QVariant::Int ) || !variant.convert( QVariant::Int ) )
    return QString();

  QString value = variant.toString();

  if ( value.size() < 1 )
    return value;

  int arg = QVariant( Util::getSafeString( argument ) ).toInt();

  if ( value.size() < arg )
    return value;

  return SafeString( value.at( value.size() - arg ) );
}
