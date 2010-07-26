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

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include "safestring.h"
#include <QtCore/QRegExp>

template <typename T>
struct TypeAccessor
{
  static QVariant lookUp( T object, const QString &part );
};

template <>
QVariant TypeAccessor<QVariantHash>::lookUp( QVariantHash object, const QString& part )
{
  if ( object.contains( part ) )
    return object.value( part );
  if ( part == QLatin1String( "items" ) ) {
    QVariantList itemsList;
    Q_FOREACH( const QString &key, object.keys() ) {
      itemsList.append( QVariant( QVariantList() << key << object.value( key ) ) );
    }
    return itemsList;
  }

  if ( part == QLatin1String( "keys" ) ) {
    const QStringList keys = object.keys();
    QVariantList list;
    Q_FOREACH( const QString &key, keys )
      list << key;
    return list;
  }

  if ( part == QLatin1String( "values" ) ) {
    return object.values();
  }

  return QVariant();
}

static QRegExp getIsTitleRegexp() {
  QRegExp titleRe( QLatin1String( "\\b[a-z]" ) );
  titleRe.setMinimal( true );
  return titleRe;
}

static QRegExp getTitleRegexp() {
  QRegExp titleRe( QLatin1String( "\\b(.)" ) );
  titleRe.setMinimal( true );
  return titleRe;
}

template <>
QVariant TypeAccessor<Grantlee::SafeString>::lookUp( Grantlee::SafeString object, const QString& part )
{
  if ( part == QLatin1String( "capitalize" ) ) {
    const QString s = object.get();
    return s.at( 0 ).toUpper() + s.right( s.length() - 1 );
  }

  static const QLatin1String falseString( "False" );
  static const QLatin1String trueString( "True" );

  if ( part == QLatin1String( "isalnum" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isLetterOrNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( part == QLatin1String( "isalpha" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isLetter() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( part == QLatin1String( "isdigit" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( part == QLatin1String( "islower" ) ) {
    const QString s = object.get().toLower();
    return ( s == object.get() ) ? trueString : falseString;
  }
  if ( part == QLatin1String( "isspace" ) ) {
    const QString s = object.get().trimmed();
    return ( s.isEmpty() ) ? trueString : falseString;
  }
  if ( part == QLatin1String( "istitle" ) ) {
    const QString s = object.get();

    static const QRegExp titleRe = getIsTitleRegexp();
    return ( titleRe.indexIn( s ) < 0 ) ? trueString : falseString;
  }
  if ( part == QLatin1String( "isupper" ) ) {
    const QString s = object.get().toUpper();
    return ( s == object ) ? trueString : falseString;
  }
  if ( part == QLatin1String( "lower" ) ) {
    return object.get().toLower();
  }
  if ( part == QLatin1String( "splitlines" ) ) {
    const QStringList strings = object.get().split( QLatin1Char( '\n' ) );
    QVariantList list;
    Q_FOREACH( const QString &string, strings )
      list << string;
    return list;
  }
  if ( part == QLatin1String( "strip" ) ) {
    return object.get().trimmed();
  }
  if ( part == QLatin1String( "swapcase" ) ) {
    const QString inputString = object.get();
    QString s;
    s.reserve( inputString.size() );
    QString::const_iterator it = inputString.constBegin();
    while ( it != inputString.constEnd() ) {
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
  if ( part == QLatin1String( "title" ) ) {
    static const QRegExp titleRe = getTitleRegexp();

    const QString s = object.get();
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
  if ( part == QLatin1String( "upper" ) ) {
    return object.get().toUpper();
  }
  return QVariant();
}

#endif
