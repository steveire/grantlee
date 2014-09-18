/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Michael Jansen <kde@michael-jansen.biz>
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

#include "metatype.h"

#include "customtyperegistry_p.h"
#include "metaenumvariable_p.h"

#include <QtCore/QDebug>

using namespace Grantlee;

Q_GLOBAL_STATIC( CustomTypeRegistry, customTypes )


void Grantlee::MetaType::internalLock()
{
  return customTypes()->mutex.lock();
}

void Grantlee::MetaType::internalUnlock()
{
  return customTypes()->mutex.unlock();
}

void Grantlee::MetaType::registerLookUpOperator( int id, LookupFunction f )
{
  Q_ASSERT( id > 0 );
  Q_ASSERT( f );

  customTypes()->registerLookupOperator( id, f );
}

static QVariant doQobjectLookUp( const QObject * const object, const QString &property )
{
  if (!object)
    return QVariant();
  if ( property == QStringLiteral( "children" ) ) {
    const QObjectList childList = object->children();
    if ( childList.isEmpty() )
      return QVariant();
    QVariantList children;

    QObjectList::const_iterator it = childList.constBegin();
    const QObjectList::const_iterator end = childList.constEnd();
    for ( ; it != end; ++it )
      children.append( QVariant::fromValue( *it ) );
    return children;
  }

  if ( property == QStringLiteral( "objectName" ) ) {
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

    const int value = me.keyToValue( property.toLatin1().constData() );

    if ( value < 0 )
      continue;

    const MetaEnumVariable mev( me, value );

    return QVariant::fromValue( mev );
  }
  return object->property( property.toUtf8().constData() );
}

QVariant Grantlee::MetaType::lookup( const QVariant &object, const QString &property )
{
  if (object.canConvert<QObject*>()) {
    return doQobjectLookUp(object.value<QObject*>(), property);
  }
  if (object.canConvert<QVariantList>()) {
    QSequentialIterable iter = object.value<QSequentialIterable>();
    if (property == QStringLiteral("size") || property == QStringLiteral( "count" ) ) {
      return iter.size();
    }

    bool ok = false;
    const int listIndex = property.toInt( &ok );

    if ( !ok || listIndex >= iter.size() ) {
      return QVariant();
    }

    return iter.at(listIndex);
  }
  if (object.canConvert<QVariantHash>()) {

    QAssociativeIterable iter = object.value<QAssociativeIterable>();

    QVariant mappedValue = iter.value(property);
    if(mappedValue.isValid())
      return mappedValue;

    if (property == QStringLiteral("size") || property == QStringLiteral( "count" ) )
    {
      return iter.size();
    }

    if ( property == QStringLiteral( "items" ) ) {
      QAssociativeIterable::const_iterator it = iter.begin();
      const QAssociativeIterable::const_iterator end = iter.end();
      QVariantList list;
      for ( ; it != end; ++it ) {
        QVariantList nested;
        nested.push_back( it.key() );
        nested.push_back( it.value() );
        list.push_back( nested );
      }
      return list;
    }

    if ( property == QStringLiteral( "keys" ) ) {
      QAssociativeIterable::const_iterator it = iter.begin();
      const QAssociativeIterable::const_iterator end = iter.end();
      QVariantList list;
      for ( ; it != end; ++it ) {
        list.push_back( it.key() );
      }
      return list;
    }

    if ( property == QStringLiteral( "values" ) ) {
      QAssociativeIterable::const_iterator it = iter.begin();
      const QAssociativeIterable::const_iterator end = iter.end();
      QVariantList list;
      for ( ; it != end; ++it ) {
        list.push_back( it.value() );
      }
      return list;
    }

    return QVariant();
  }
  return customTypes()->lookup( object, property );
}

bool Grantlee::MetaType::lookupAlreadyRegistered( int id )
{
  return customTypes()->lookupAlreadyRegistered( id );
}
