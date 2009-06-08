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


#include "truncatewords.h"
#include <QVariant>

TruncateWordsFilter::TruncateWordsFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString TruncateWordsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QVariant v( argument );
  if ( !v.convert( QVariant::Int ) ) {
    return input.toString();
  }
  int numWords = v.toInt();

  QString inputString = Util::getSafeString( input ).rawString();
  QStringList words = inputString.split( " ", QString::SkipEmptyParts );

  if ( words.size() > numWords ) {
    words = words.mid( 0, numWords );
    if ( !words.at( words.size() - 1 ).endsWith( "..." ) ) {
      words << "...";
    }
  }
  return words.join( " " );

}

#include "truncatewords.moc"

