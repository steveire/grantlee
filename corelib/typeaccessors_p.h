/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_TYPE_ACCESSORS_H
#define GRANTLEE_TYPE_ACCESSORS_H

#include <QVariant>
#include <QStringList>
#include <QDebug>
#include "safestring.h"
#include <QRegExp>

template <typename T>
struct TypeAccessor
{
  static QVariant lookUp( T object, const QString &part );
};

template <>
QVariant TypeAccessor<QVariantHash>::lookUp( QVariantHash object, const QString& part )
{
  if ( part == QLatin1String( "items" ) )
  {
    QVariantList itemsList;
    foreach (const QString &key, object.keys() )
    {
      itemsList.append( QVariant( QVariantList() << key << object.value( key ) ) );
    }
    return itemsList;
  }

  if ( part == QLatin1String( "keys" ) )
  {
    QStringList keys = object.keys();
    QVariantList list;
    foreach( const QString &key, keys )
      list << key;
    return list;
  }

  if ( part == QLatin1String( "values" ) )
  {
    return object.values();
  }

  return QVariant();
}

template <>
QVariant TypeAccessor<Grantlee::SafeString>::lookUp( Grantlee::SafeString object, const QString& part )
{
  if ( part == QLatin1String( "capitalize" ) )
  {
    QString s = object.get();
    return s.at( 0 ).toUpper() + s.right( s.length() - 1 );
  }
  if ( part == QLatin1String( "isalnum" ) )
  {
    QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() )
    {
      if ( !it->isLetterOrNumber() )
        return "False";
      ++it;
    }
    return "True";
  }
  if ( part == QLatin1String( "isalpha" ) )
  {
    QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() )
    {
      if ( !it->isLetter() )
        return "False";
      ++it;
    }
    return "True";
  }
  if ( part == QLatin1String( "isdigit" ) )
  {
    QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() )
    {
      if ( !it->isNumber() )
        return "False";
      ++it;
    }
    return "True";
  }
  if ( part == QLatin1String( "islower" ) )
  {
    QString s = object.get().toLower();
    return ( s == object.get() ) ? "True" : "False";
  }
  if ( part == QLatin1String( "isspace" ) )
  {
    QString s = object.get().trimmed();
    return ( s.isEmpty() ) ? "True" : "False";
  }
  if ( part == QLatin1String( "istitle" ) )
  {
    QString s = object.get();
    QRegExp titleRe( "\\b[a-z]" );
    titleRe.setMinimal( true );
    return ( titleRe.indexIn( s ) < 0 ) ? "True" : "False";
  }
  if ( part == QLatin1String( "isupper" ) )
  {
    QString s = object.get().toUpper();
    return ( s == object ) ? "True" : "False";
  }
  if ( part == QLatin1String( "lower" ) )
  {
    QString s = object.get().toLower();
    return s;
  }
  if ( part == QLatin1String( "splitlines" ) )
  {
    QStringList strings = object.get().split( QLatin1Char( '\n' ) );
    QVariantList list;
    foreach( const QString &string, strings )
      list << string;
    return list;
  }
  if ( part == QLatin1String( "strip" ) )
  {
    QString s = object.get().trimmed();
    return s;
  }
  if ( part == QLatin1String( "swapcase" ) )
  {
    QString inputString = object.get();
    QString s;
    s.reserve( inputString.size() );
    QString::const_iterator it = inputString.constBegin();
    while ( it != inputString.constEnd() )
    {
      if ( it->isUpper() )
        s += it->toLower();
      else if ( it->isLower() )
        s += it->toUpper();
      else
        s += *it;
      ++it;
    }
    return s;
  }
  if ( part == QLatin1String( "title" ) )
  {
    QRegExp titleRe( "\\b(.)" );
    titleRe.setMinimal( true );

    QString s = object.get();
    QString output;
    output.reserve( s.size() );
    int pos = 0;
    int nextPos = 0;
    int matchedLength;

    while ( ( pos = titleRe.indexIn( s, pos ) ) != -1 ) {
      output += titleRe.cap( 1 ).toUpper();
      matchedLength = titleRe.matchedLength();
      if ( ( nextPos = titleRe.indexIn( s, pos + matchedLength ) ) != -1 )
        output += s.mid( pos + matchedLength, nextPos - pos - 1 );
      else
        output += s.right( s.length() - ( pos + matchedLength ) );
      pos += matchedLength;
    }
    return output;
  }
  if ( part == QLatin1String( "upper" ) )
  {
    QString s = object.get().toUpper();
    return s;
  }
  return QVariant();
}

#endif
