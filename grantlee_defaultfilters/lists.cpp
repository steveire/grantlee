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

#include "lists.h"
#include <util_p.h>
#include <QDateTime>

JoinFilter::JoinFilter()
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

    ret.append( s );
    if ( i.hasNext() )
      ret.append( argument );
  }
  return Util::markSafe( ret );
}

LengthFilter::LengthFilter()
{
}

Grantlee::SafeString LengthFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return QString::number( Util::variantToList( input ).size() );
}

LengthIsFilter::LengthIsFilter()
{
}

Grantlee::SafeString LengthIsFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return ( Util::variantToList( input ).size() == QVariant( argument ).toInt() ) ? "true" : QString();
}

FirstFilter::FirstFilter()
{
}

Grantlee::SafeString FirstFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return Util::getSafeString( varList.at( 0 ) );
}


LastFilter::LastFilter()
{
}

Grantlee::SafeString LastFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return Util::getSafeString( varList.at( varList.size() - 1 ) );
}


RandomFilter::RandomFilter()
{
}

Grantlee::SafeString RandomFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  qsrand( QDateTime::currentDateTime().toTime_t() );
  int rnd = qrand() % varList.size();
  return Util::getSafeString( varList.at( rnd ) );
}

SliceFilter::SliceFilter()
{
}

Grantlee::SafeString SliceFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  int splitterIndex = argument.indexOf( ":" );
  QString inputString = Util::getSafeString( input );
  if ( splitterIndex >= 0 ) {
    int left = QVariant( argument.left( splitterIndex ) ).toInt();
    int right = QVariant( argument.right( splitterIndex ) ).toInt();
    if ( right < 0 ) {
      right = inputString.size() + right;
    }
    return inputString.mid( left, right );
  } else {
    return QString( inputString.at( QVariant( argument ).toInt() ) );
  }
}

