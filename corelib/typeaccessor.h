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

#ifndef GRANTLEE_TYPEACCESSOR_H
#define GRANTLEE_TYPEACCESSOR_H

#include "containeraccessor.h"
#include "grantlee_core_export.h"

#include <QtCore/QLinkedList>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>

#include <vector>
#include <deque>

namespace Grantlee
{

#ifndef Q_QDOC
template <typename T>
struct TypeAccessor
{
  static QVariant lookUp( const T object, const QString &property );
};

template <typename T>
struct TypeAccessor<T*>
{
  static QVariant lookUp( const T * const object, const QString &property );
};

template <typename T>
struct TypeAccessor<T&>
{
  static QVariant lookUp( const T &object, const QString &property );
};
#endif

namespace
{

template<typename Container>
struct SequentialContainerLookup
{
  static QVariant doLookUp( const Container &container, const QString &property )
  {
    bool ok = false;
    const size_t listIndex = (size_t)property.toInt( &ok );

    if ( !ok || listIndex >= (size_t)container.size() ) {
        return QVariant();
    }

    return QVariant::fromValue( container[listIndex] );
  }
};

template<typename Container>
QVariant doAssociativeContainerLookup( const Container &object, const QString &property )
{
  {
    typename Container::const_iterator it = Finder<Container>::find( object, property );
    if ( it != object.end() )
      return QVariant::fromValue( MappedValueGetter<Container>::get( it ) );
  }
  if ( property == QLatin1String( "items" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      QVariantList nested;
      nested.push_back( QVariant::fromValue( KeyGetter<Container>::get( it ) ) );
      nested.push_back( QVariant::fromValue( MappedValueGetter<Container>::get( it ) ) );
      list.push_back( nested );
    }
    return list;
  }

  if ( property == QLatin1String( "keys" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      list.push_back( QVariant::fromValue( KeyGetter<Container>::get( it ) ) );
    }
    return list;
  }

  if ( property == QLatin1String( "values" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      list.push_back( QVariant::fromValue( MappedValueGetter<Container>::get( it ) ) );
    }
    return list;
  }

  return QVariant();
}

}

#ifndef Q_QDOC
/**
 * @internal Looks up the property @p property from QObject @p object.
 */
QVariant GRANTLEE_CORE_EXPORT doQobjectLookUp( const QObject * const object, const QString& property );
#endif
}

#define GRANTLEE_DISABLE_RANDOM_ACCESS(Container)                     \
namespace Grantlee {                                                  \
template<typename T>                                                  \
struct TypeAccessor<Container<T>&>                                    \
{                                                                     \
  static QVariant lookUp( const Container<T> &, const QString &)      \
  {                                                                   \
    return QVariant();                                                \
  }                                                                   \
};                                                                    \
}                                                                     \

/**
  Registers Container with %Grantlee so that it can be iterated in a @gr_tag{for} tag.

  @see @ref third_party_containers
*/
#define GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR(Container)                          \
namespace Grantlee {                                                                    \
template<typename T>                                                                    \
struct TypeAccessor<Container<T>&>                                                      \
{                                                                                       \
  static QVariant lookUp( const Container<T> &c, const QString &property )              \
  {                                                                                     \
    return SequentialContainerLookup<Container<T> >::doLookUp( c, property );           \
  }                                                                                     \
};                                                                                      \
}                                                                                       \

/**
  Registers Container with %Grantlee so that it can be iterated in a @gr_tag{for} tag.

  Also makes the properties <tt>items</tt>, <tt>keys</tt> and <tt>values</tt> available.

  @see @ref third_party_containers
*/
#define GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR(Container)                    \
namespace Grantlee {                                                               \
template<typename T, typename U>                                                   \
struct TypeAccessor<Container<T, U>&>                                              \
{                                                                                  \
  static QVariant lookUp( const Container<T, U> &c, const QString &property )      \
  {                                                                                \
    return doAssociativeContainerLookup( c, property );                            \
  }                                                                                \
};                                                                                 \
}                                                                                  \

/**
  Allows %Grantlee to access SmartPointer&lt;QObjectSubclass&gt; in templates.

  @see @ref smart_pointers
 */
#define GRANTLEE_SMART_PTR_ACCESSOR(SmartPointer)                                  \
namespace Grantlee {                                                               \
template<typename T>                                                               \
struct TypeAccessor<SmartPointer<T>&>                                              \
{                                                                                  \
  static QVariant lookUp( const SmartPointer<T> &object, const QString &property ) \
  {                                                                                \
    return doQobjectLookUp( object.operator->(), property );                       \
  }                                                                                \
};                                                                                 \
}                                                                                  \

GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR(QList)
GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR(QVector)

GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR(QHash)
GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR(QMap)

GRANTLEE_DISABLE_RANDOM_ACCESS(QSet)
GRANTLEE_DISABLE_RANDOM_ACCESS(QLinkedList)
GRANTLEE_DISABLE_RANDOM_ACCESS(std::list)

GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR  (std::deque)
GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR  (std::vector)
GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR (std::map)

GRANTLEE_SMART_PTR_ACCESSOR(QSharedPointer)

#endif
