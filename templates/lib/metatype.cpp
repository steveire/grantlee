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

QVariant Grantlee::MetaType::lookup( const QVariant &object, const QString &property )
{
  if (object.canConvert<QObject*>()) {
    return doQobjectLookUp(object.value<QObject*>(), property);
  }
  if (object.canConvert<QVariantList>()) {
    QSequentialIterable iter = object.value<QSequentialIterable>();
    if (property == QLatin1String("size") || property == QLatin1String( "count" ) ) {
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

    if (property == QLatin1String("size") || property == QLatin1String( "count" ) )
    {
      return iter.size();
    }

    if ( property == QLatin1String( "items" ) ) {
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

    if ( property == QLatin1String( "keys" ) ) {
      QAssociativeIterable::const_iterator it = iter.begin();
      const QAssociativeIterable::const_iterator end = iter.end();
      QVariantList list;
      for ( ; it != end; ++it ) {
        list.push_back( it.key() );
      }
      return list;
    }

    if ( property == QLatin1String( "values" ) ) {
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
