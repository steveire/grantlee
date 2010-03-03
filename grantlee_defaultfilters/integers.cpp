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

#include "integers.h"

#include "util_p.h"

QVariant AddFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return Util::getSafeString( input )->toInt() + Util::getSafeString( argument )->toInt();
}

QVariant GetDigitFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString value = Util::getSafeString( input );

  bool ok;
  (void)value->toInt(&ok);
  if ( !ok )
    return QString();

  if ( value->size() < 1 )
    return value;

  int arg = Util::getSafeString( argument )->toInt();

  if ( value->size() < arg )
    return value;

  return SafeString( value->at( value->size() - arg ) );
}

