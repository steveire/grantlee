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

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include "metatype.h"

#include <map>

#ifndef GRANTLEE_NO_TR1
#include "qtunorderedmap.h"
#include <tr1/array>

template <typename T>
struct ThreeArray : public std::tr1::array<T, 3>
{

};
#endif

using std::map;

#define DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, KeyType, ValueType)           \
  typedef ContainerType<KeyType, ValueType> ContainerType##KeyType##ValueType;     \
  Q_DECLARE_METATYPE(ContainerType##KeyType##ValueType)                            \

#define DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL(ContainerType, ValueType)  \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint32,     ValueType)          \

#define DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL(ContainerType, ValueType)  \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint16,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint32,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint64,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint16,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint32,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint64,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, QString,    ValueType)      \

#define DECLARE_ASSOCIATIVE_CONTAINERS_ALL(Type)          \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL( QMap,  Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL( QHash, Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL( map,   Type )  \

#define DECLARE_ASSOCIATIVE_CONTAINERS_MINIMAL(Type)          \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL( QMap,  Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL( QHash, Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL( map,   Type )  \

#ifndef MINIMAL_CONTAINER_TESTS
#  define DECLARE_ASSOCIATIVE_CONTAINER_TYPES DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL
#else
#  define DECLARE_ASSOCIATIVE_CONTAINER_TYPES DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL
#endif

#define DECLARE_BUILTIN_TYPE_CONTAINERS(Type)                  \
  Q_DECLARE_METATYPE( QList      < Type > )                    \
  Q_DECLARE_METATYPE( QVector    < Type > )                    \
  Q_DECLARE_METATYPE( QQueue     < Type > )                    \
  Q_DECLARE_METATYPE( QStack     < Type > )                    \
  Q_DECLARE_METATYPE( QLinkedList< Type > )                    \
  Q_DECLARE_METATYPE( QSet       < Type > )                    \
  Q_DECLARE_METATYPE( std::vector< Type > )                    \
  Q_DECLARE_METATYPE( std::deque < Type > )                    \
  Q_DECLARE_METATYPE( std::list  < Type > )                    \
                                                               \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES( QMap,           Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES( QHash,          Type )  \
  DECLARE_ASSOCIATIVE_CONTAINER_TYPES( map,            Type )  \

#ifndef GRANTLEE_NO_TR1
#  define DECLARE_TR1_TYPE_CONTAINERS(Type)                       \
     DECLARE_ASSOCIATIVE_CONTAINER_TYPES( QtUnorderedMap, Type )  \
     Q_DECLARE_METATYPE( ThreeArray < Type > )                    \

#  define DECLARE_TYPE_CONTAINERS(Type)                           \
     DECLARE_BUILTIN_TYPE_CONTAINERS(Type)                        \
     DECLARE_TR1_TYPE_CONTAINERS(Type)                            \

namespace Grantlee {

template<typename T, typename U>
struct KeyGetter<QtUnorderedMap<T, U> > : public Getter<QtUnorderedMap<T, U> >
{
  static T get(typename QtUnorderedMap<T, U>::const_iterator it)
  {
    return it->first;
  }
};

template<typename T, typename U>
struct MappedValueGetter<QtUnorderedMap<T, U> > : public Getter<QtUnorderedMap<T, U> >
{
  static U get(typename QtUnorderedMap<T, U>::const_iterator it)
  {
    return it->second;
  }
};

}

#else

#  define DECLARE_TYPE_CONTAINERS(Type)                          \
     DECLARE_BUILTIN_TYPE_CONTAINERS(Type)                       \

#endif

#endif
