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

#include "yesno.h"


YesNoFilter::YesNoFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString YesNoFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QString arg = argument;
  QString yes;
  QString no;
  QString maybe;
  if ( arg.isNull() ) {
    yes = "yes";
    no = "no";
    maybe = "maybe";
  } else {
    QStringList argList = arg.split( "," );
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
  if ( !Util::getSafeString( input ).rawString().isEmpty() )
    return yes;
  return no;
}

#include "yesno.moc"

