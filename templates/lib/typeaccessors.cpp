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

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace Grantlee
{

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
QVariant TypeAccessor<Grantlee::SafeString&>::lookUp( const Grantlee::SafeString &object, const QString &property )
{
  if ( property == QLatin1String( "capitalize" ) ) {
    const QString s = object.get();
    return s.at( 0 ).toUpper() + s.right( s.length() - 1 );
  }

  static const QLatin1String falseString( "False" );
  static const QLatin1String trueString( "True" );

  if ( property == QLatin1String( "isalnum" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isLetterOrNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QLatin1String( "isalpha" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isLetter() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QLatin1String( "isdigit" ) ) {
    const QString s = object.get();
    QString::const_iterator it = s.constBegin();
    while ( it != s.constEnd() ) {
      if ( !it->isNumber() )
        return falseString;
      ++it;
    }
    return trueString;
  }
  if ( property == QLatin1String( "islower" ) ) {
    const QString s = object.get().toLower();
    return ( s == object.get() ) ? trueString : falseString;
  }
  if ( property == QLatin1String( "isspace" ) ) {
    const QString s = object.get().trimmed();
    return ( s.isEmpty() ) ? trueString : falseString;
  }
  if ( property == QLatin1String( "istitle" ) ) {
    const QString s = object.get();

    static const QRegExp titleRe = getIsTitleRegexp();
    return ( titleRe.indexIn( s ) < 0 ) ? trueString : falseString;
  }
  if ( property == QLatin1String( "isupper" ) ) {
    const QString s = object.get().toUpper();
    return ( s == object ) ? trueString : falseString;
  }
  if ( property == QLatin1String( "lower" ) ) {
    return object.get().toLower();
  }
  if ( property == QLatin1String( "splitlines" ) ) {
    const QStringList strings = object.get().split( QLatin1Char( '\n' ) );
    QVariantList list;
    Q_FOREACH( const QString &string, strings )
      list << string;
    return list;
  }
  if ( property == QLatin1String( "strip" ) ) {
    return object.get().trimmed();
  }
  if ( property == QLatin1String( "swapcase" ) ) {
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
  if ( property == QLatin1String( "title" ) ) {
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
  if ( property == QLatin1String( "upper" ) ) {
    return object.get().toUpper();
  }
  return QVariant();
}

QVariant doQobjectLookUp( const QObject * const object, const QString &property )
{
  if ( property == QLatin1String( "children" ) ) {
    QObjectList childList = object->children();
    if ( childList.isEmpty() )
      return QVariant();
    QVariantList children;
    foreach( QObject *object, childList )
      children.append( QVariant::fromValue( object ) );
    return children;
  }

  if ( property == QLatin1String( "objectName" ) ) {
    return object->objectName();
  }
  // Can't be const because of invokeMethod.
  const QMetaObject *metaObj = object->metaObject();

  QMetaProperty mp;
  for ( int i = 0; i < metaObj->propertyCount(); ++i ) {
    // TODO only read-only properties should be allowed here.
    // This might also handle the variant messing I hit before.
    mp = metaObj->property( i );

    if ( QString::fromUtf8( mp.name() ) != property )
      continue;

    if ( mp.isEnumType() ) {
      MetaEnumVariable mev( mp.enumerator(), mp.read( object ).toInt() );
      return QVariant::fromValue( mev );
    }

    return mp.read( object );
  }
  QMetaEnum me;
  for ( int i = 0; i < metaObj->enumeratorCount(); ++i ) {
    me = metaObj->enumerator( i );

    if ( QLatin1String( me.name() ) == property ) {
      MetaEnumVariable mev( me );
      return QVariant::fromValue( mev );
    }

    const int value = me.keyToValue( property.toLatin1() );

    if ( value < 0 )
      continue;

    const MetaEnumVariable mev( me, value );

    return QVariant::fromValue( mev );
  }
  return object->property( property.toUtf8() );
}

template <>
QVariant TypeAccessor<QObject*>::lookUp( const QObject * const object, const QString &property )
{
  return doQobjectLookUp( object, property );
}

template <>
QVariant TypeAccessor<MetaEnumVariable&>::lookUp( const MetaEnumVariable &object, const QString &property )
{
  if ( property == QLatin1String( "name" ) )
    return QLatin1String( object.enumerator.name() );
  if ( property == QLatin1String( "value" ) )
    return object.value;
  if ( property == QLatin1String( "key" ) )
    return QLatin1String( object.enumerator.valueToKey( object.value ) );
  if ( property == QLatin1String( "scope" ) )
    return QLatin1String( object.enumerator.scope() );
  if ( property == QLatin1String( "keyCount" ) )
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
