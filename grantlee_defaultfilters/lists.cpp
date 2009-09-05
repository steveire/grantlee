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

LengthFilter::LengthFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString LengthFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return QString::number( Util::variantToList( input ).size() );
}

LengthIsFilter::LengthIsFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString LengthIsFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return ( Util::variantToList( input ).size() == QVariant( argument ).toInt() ) ? "true" : QString();
}

FirstFilter::FirstFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString FirstFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return Util::getSafeString( varList.at( 0 ) );
}


LastFilter::LastFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString LastFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return Util::getSafeString( varList.at( varList.size() - 1 ) );
}


RandomFilter::RandomFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString RandomFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );

  qsrand( QDateTime::currentDateTime().toTime_t() );
  int rnd = qrand() % varList.size();
  return Util::getSafeString( varList.at( rnd ) );
}

SliceFilter::SliceFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString SliceFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  int splitterIndex = argument.rawString().indexOf( ":" );
  QString inputString = Util::getSafeString( input ).rawString();
  if ( splitterIndex >= 0 ) {
    int left = QVariant( argument.rawString().left( splitterIndex ) ).toInt();
    int right = QVariant( argument.rawString().right( splitterIndex ) ).toInt();
    if ( right < 0 ) {
      right = inputString.size() + right;
    }
    return inputString.mid( left, right );
  } else {
    return QString( inputString.at( QVariant( argument ).toInt() ) );
  }
}

