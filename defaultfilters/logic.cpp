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

#include "logic.h"
#include "util.h"

QVariant DefaultFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  if ( !input.isValid() || getSafeString( input ).get().isEmpty() )
    return argument;
  return getSafeString( input );
}

QVariant DefaultIfNoneFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  if ( !input.isValid() )
    return argument;
  return getSafeString( input );
}

QVariant DivisibleByFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return ( getSafeString( input ).get().toInt()
           % QVariant( argument ).toInt() == 0 )
         ? QLatin1String( "true" ) : QString();
}

QVariant YesNoFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString arg = getSafeString( argument );
  QString yes;
  QString no;
  QString maybe;
  if ( arg.get().isEmpty() ) {
    yes = QLatin1String( "yes" );
    no = QLatin1String( "no" );
    maybe = QLatin1String( "maybe" );
  } else {
    QStringList argList = arg.get().split( QLatin1Char( ',' ) );
    int numArgs = argList.size();
    if (( numArgs < 2 ) || ( numArgs > 3 ) ) {
      return input.toString();
    } else if ( numArgs == 2 ) {
      yes = argList.at( 0 );
      no = argList.at( 1 );
      maybe = argList.at( 1 );
    } else if ( numArgs == 3 ) {
      yes = argList.at( 0 );
      no = argList.at( 1 );
      maybe = argList.at( 2 );
    }
  }
  if ( !input.isValid() )
    return maybe;
  if ( !getSafeString( input ).get().isEmpty() )
    return yes;
  return no;
}

