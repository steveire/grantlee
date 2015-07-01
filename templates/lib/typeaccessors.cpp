/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "typeaccessor.h"

#include "metaenumvariable_p.h"
#include "safestring.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

namespace Grantlee
{

static QRegularExpression getIsTitleRE() {
  QRegularExpression titleRe( QStringLiteral( "\\b[a-z]" ), QRegularExpression::InvertedGreedinessOption );
  return titleRe;
}

static QRegularExpression getTitleRE() {
  QRegularExpression titleRe( QStringLiteral( "\\b(.)" ), QRegularExpression::InvertedGreedinessOption );
  return titleRe;
}

template <>
QVariant TypeAccessor<Grantlee::SafeString&>::lookUp( const Grantlee::SafeString &object, const QString &property )
{
  if ( property == QStringLiteral( "capitalize" ) ) {
    const QString s = object.get();
    return QVariant(s.at( 0 ).toUpper() + s.right( s.length() - 1 ));
  }

  static const QLatin1String falseString( "False" );
  static const QLatin1String trueString( "True" );

  if ( property == QStringLiteral( "isalnum" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isLetterOrNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QStringLiteral( "isalpha" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    if ( it == s.constEnd() )
      return falseString;
    while ( it != s.constEnd() ) {
      if ( !it->isLetter() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QStringLiteral( "isdigit" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QStringLiteral( "islower" ) ) {
    const QString s = object.get().toLower();
    return ( s == object.get() ) ? trueString : falseString;
  }
  if ( property == QStringLiteral( "isspace" ) ) {
    const QString s = object.get().trimmed();
    return ( s.isEmpty() ) ? trueString : falseString;
  }
  if ( property == QStringLiteral( "istitle" ) ) {
    const QString s = object.get();

    static const QRegularExpression titleRe = getIsTitleRE();
    return ( titleRe.match( s ).hasMatch() ) ? falseString : trueString;
  }
  if ( property == QStringLiteral( "isupper" ) ) {
    const QString s = object.get().toUpper();
    return ( s == object ) ? trueString : falseString;
  }
  if ( property == QStringLiteral( "lower" ) ) {
    return object.get().toLower();
  }
  if ( property == QStringLiteral( "splitlines" ) ) {
    const QStringList strings = object.get().split( QLatin1Char( '\n' ) );
    QVariantList list;
    QStringList::const_iterator it = strings.constBegin();
    const QStringList::const_iterator end = strings.constEnd();
    for ( ; it != end; ++it )
      list << *it;
    return list;
  }
  if ( property == QStringLiteral( "strip" ) ) {
    return object.get().trimmed();
  }
  if ( property == QStringLiteral( "swapcase" ) ) {
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
  if ( property == QStringLiteral( "title" ) ) {
    static const QRegularExpression titleRe = getTitleRE();

    const QString s = object.get();
    QString output;
    output.reserve( s.size() );
    int pos = 0;
    int nextPos = 0;
    int matchedLength;

    QRegularExpressionMatchIterator it = titleRe.globalMatch( s );
    while ( it.hasNext() ) {
      QRegularExpressionMatch match = it.next();
      pos = match.capturedStart();
      output += match.captured().toUpper();
      matchedLength = match.capturedLength();
      if ( it.hasNext() ) {
        match = it.peekNext();
        nextPos = match.capturedStart();
        output += s.mid( pos + matchedLength, nextPos - pos - 1 );
      } else {
        output += s.right( s.length() - ( pos + matchedLength ) );
      }
    }

    return output;
  }
  if ( property == QStringLiteral( "upper" ) ) {
    return object.get().toUpper();
  }
  return QVariant();
}

template <>
QVariant TypeAccessor<MetaEnumVariable&>::lookUp( const MetaEnumVariable &object, const QString &property )
{
  if ( property == QStringLiteral( "name" ) )
    return QLatin1String( object.enumerator.name() );
  if ( property == QStringLiteral( "value" ) )
    return object.value;
  if ( property == QStringLiteral( "key" ) )
    return QLatin1String( object.enumerator.valueToKey( object.value ) );
  if ( property == QStringLiteral( "scope" ) )
    return QLatin1String( object.enumerator.scope() );
  if ( property == QStringLiteral( "keyCount" ) )
    return object.enumerator.keyCount();

  bool ok = false;
  const int listIndex = property.toInt( &ok );
  if ( ok ) {
    if ( listIndex >= object.enumerator.keyCount() )
      return QVariant();

    const MetaEnumVariable mev( object.enumerator, object.enumerator.value( listIndex ) );
    return QVariant::fromValue( mev );
  }

  return QVariant();
}

}
