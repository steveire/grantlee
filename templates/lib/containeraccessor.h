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

#ifndef CONTAINERACCESSOR_H
#define CONTAINERACCESSOR_H

#include <QtCore/QVariant>

#include <map>

namespace Grantlee
{

/// @headerfile containeraccessor.h grantlee/containeraccessor.h
/**
  @internal Used as a convenience base class for container %iterator access.
*/
template<typename Container_>
struct Getter
{
  /**
   * @internal Used to extract a list from the container.
   */
  typedef Container_ Container;
};

/// @headerfile containeraccessor.h grantlee/containeraccessor.h
/**
  Trait for retrieving a key from a container %iterator.

  This can be trivially specialized in rare cases if supporting a third party container.

  @see @ref third_party_containers
 */
template<typename Container>
struct KeyGetter : public Getter<Container>
{
  /**
    Returns the key available through the container %iterator @p it.
   */
  static typename Container::key_type get( const typename Container::const_iterator it ) {
    return it.key();
  }
};

/// @headerfile containeraccessor.h grantlee/containeraccessor.h
/**
  Trait for retrieving a mapped value from a container %iterator.

  This can be trivially specialized in rare cases if supporting a third party container.

  @see @ref third_party_containers
 */
template<typename Container>
struct MappedValueGetter : public Getter<Container>
{
  /**
    Returns the key available through the container %iterator @p it.
   */
  static typename Container::mapped_type get( const typename Container::const_iterator it ) {
    return *it;
  }
};

#ifndef Q_QDOC
template<typename T, typename U>
struct KeyGetter<std::map<T, U> > : public Getter<std::map<T, U> >
{
  static T get( typename std::map<T, U>::const_iterator it ) {
    return it->first;
  }
};

template<typename T, typename U>
struct MappedValueGetter<std::map<T, U> > : public Getter<std::map<T, U> >
{
  static U get( typename std::map<T, U>::const_iterator it ) {
    return it->second;
  }
};

template<typename Container>
struct ValueGetter : public Getter<Container>
{
  static typename Container::value_type get( const typename Container::const_iterator it ) {
    return *it;
  }
};

template<typename Container, typename T = typename Container::key_type>
struct Finder
{
  static typename Container::const_iterator find( const Container &container, const QString &nextPart )
  {
    {
      // Compile error if key_type is not a number.
      static const QString s = QString::number( T() );
      Q_UNUSED( s )
    }

    QVariant v = nextPart;
    if ( !v.canConvert<typename Container::key_type>() || !v.convert( QVariant::Double ) )
      return container.end();
    typename Container::key_type key = v.value<typename Container::key_type>();
    return container.find( key );
  }
};

template<typename Container>
struct Finder<Container, QString>
{
  static typename Container::const_iterator find( const Container &container, const QString &nextPart )
  {
    return container.find( nextPart );
  }
};
#endif

namespace {

template<typename Getter>
QVariantList getList( const QVariant &obj )
{
  const typename Getter::Container c = obj.value<typename Getter::Container>();
  typename Getter::Container::const_iterator it = c.begin();
  const typename Getter::Container::const_iterator end = c.end();
  QVariantList list;
  list.reserve(static_cast<int>(std::distance(it, end)));
  for ( ; it != end; ++it ) {
    list << QVariant::fromValue( Getter::get( it ) );
  }
  return list;
}

template<typename Container>
struct SequentialContainerAccessor
{
  static QVariantList doToList( const QVariant &obj )
  {
    return getList<ValueGetter<Container> >( obj );
  }
};

template<>
struct SequentialContainerAccessor<QVariantList>
{
  static QVariantList doToList( const QVariant &obj )
  {
    return obj.toList();
  }
};

template<typename Container>
struct AssociativeContainerAccessor
{
  static QVariantList doToList( const QVariant &obj )
  {
    return getList<KeyGetter<Container> >( obj );
  }
};

}

}

#endif
