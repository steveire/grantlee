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

#include "join.h"
#include <util_p.h>

JoinFilter::JoinFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString JoinFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );
  QListIterator<QVariant> i( varList );
  QString ret;
  while ( i.hasNext() ) {
    QVariant var = i.next();
    Grantlee::SafeString s = Util::getSafeString( var );
    if ( autoescape )
      s = Util::conditionalEscape( s );

    ret.append( s.rawString() );
    if ( i.hasNext() )
      ret.append( argument );
  }
  return Util::markSafe( ret );
}

#include "join.moc"

