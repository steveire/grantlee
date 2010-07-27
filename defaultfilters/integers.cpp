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

#include "util.h"

QVariant AddFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )

  if ( isSafeString( input ) )
  {
    if ( isSafeString( argument ) )
      return getSafeString( input ) + getSafeString( argument );
    return input;
  }

  if ( input.type() == QVariant::List )
  {
    if ( argument.type() == QVariant::List )
      return input.toList() + argument.toList();
    return input;
  }

  if ( input.type() == QVariant::StringList )
  {
    if ( argument == QVariant::StringList )
      return input.toStringList() + argument.toStringList();
    return input;
  }

  if ( input.type() == QVariant::Int )
  {
    if ( argument.type() == QVariant::Int )
      return input.toInt() + argument.toInt();
    return input;
  }

  if ( input.type() == QVariant::UInt )
  {
    if ( argument.type() == QVariant::UInt )
      return input.toUInt() + argument.toUInt();
    return input;
  }

  if ( input.type() == QVariant::Double )
  {
    if ( argument.type() == QVariant::Double )
      return input.toDouble() + argument.toDouble();
    return input;
  }

  if ( input.type() == QVariant::LongLong )
  {
    if ( argument.type() == QVariant::LongLong )
      return input.toLongLong() + argument.toLongLong();
    return input;
  }

  if ( input.type() == QVariant::ULongLong )
  {
    if ( argument.type() == QVariant::ULongLong )
      return input.toULongLong() + argument.toULongLong();
    return input;
  }
  return input;
}

QVariant GetDigitFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString value = getSafeString( input );

  bool ok;
  ( void )value.get().toInt( &ok );
  if ( !ok )
    return QString();

  if ( value.get().size() < 1 )
    return value;

  int arg = getSafeString( argument ).get().toInt();

  if ( value.get().size() < arg )
    return value;

  return SafeString( value.get().at( value.get().size() - arg ) );
}

