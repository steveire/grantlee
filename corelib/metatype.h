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

#ifndef GRANTLEE_METATYPE_H
#define GRANTLEE_METATYPE_H

#include "grantlee_core_export.h"

#include "typeaccessor.h"

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QStack>
#include <QtCore/QQueue>

namespace Grantlee
{

class GRANTLEE_CORE_EXPORT MetaType
{
public:
  typedef QVariant (*LookupFunction)(const QVariant &, const QString &);
  typedef QVariantList (*ToVariantListFunction)(const QVariant &);

  static void registerLookUpOperator( int id, LookupFunction f );
  static void registerToVariantListOperator( int id, ToVariantListFunction f );

  static void internalLock();
  static void internalUnlock();

  static QVariant lookup( const QVariant &object, const QString &property );
  static QVariantList toVariantList( const QVariant &obj );

  static bool lookupAlreadyRegistered(int id);
  static bool toListAlreadyRegistered(int id);

private:
  MetaType();
};

namespace
{

/*
 * This is a helper to select an appropriate overload of indexAccess
 */
template<typename RealType, typename HandleAs>
struct LookupTrait
{
  static QVariant doLookUp( const QVariant &object, const QString &property )
  {
    typedef typename Grantlee::TypeAccessor<HandleAs> Accessor;
    return Accessor::lookUp( static_cast<HandleAs>( object.value<RealType>() ), property );
  }
};

/*
 * Register a type so grantlee knows how to handle it.
 *
 * :TODO: Real docu
 */
template<typename RealType, typename HandleAs>
int internalRegisterType()
{
  const int id = qMetaTypeId<RealType>();

  if ( MetaType::lookupAlreadyRegistered( id ) )
    return id;

  QVariant ( *lf )(const QVariant&, const QString&) = LookupTrait<RealType, HandleAs>::doLookUp;

  MetaType::registerLookUpOperator( id, reinterpret_cast<MetaType::LookupFunction>( lf ) );

  return id;
}

template<typename Container, typename HandleAs>
int registerSequentialContainer()
{
  const int id = internalRegisterType<Container, HandleAs>();

  if ( MetaType::toListAlreadyRegistered( id ) )
    return id;

  QVariantList ( *tlf )(const QVariant&) = SequentialContainerAccessor<Container>::doToList;
  MetaType::registerToVariantListOperator( id, reinterpret_cast<MetaType::ToVariantListFunction>( tlf ) );
  return id;
}

template<typename Container>
int registerSequentialContainer()
{
  return registerSequentialContainer<Container, Container>();
}

template<typename Container, typename HandleAs>
int registerAssociativeContainer()
{
  const int id = internalRegisterType<Container, HandleAs>();

  if ( MetaType::toListAlreadyRegistered( id ) )
    return id;

  QVariantList ( *tlf )(const QVariant&) = AssociativeContainerAccessor<Container>::doToList;
  MetaType::registerToVariantListOperator( id, reinterpret_cast<MetaType::ToVariantListFunction>( tlf ) );
  return id;
}

template<typename Container>
int registerAssociativeContainer()
{
  return registerAssociativeContainer<Container, Container>();
}

}

template<typename RealType, int n>
struct RegisterTypeContainer
{
  static void reg()
  {
  }
};

#define GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(Container, Type)                                   \
  Grantlee::RegisterTypeContainer<Container<Type>, QMetaTypeId2<Container<Type> >::Defined>::reg();  \

#define GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, Key, Type)                                   \
  Grantlee::RegisterTypeContainer<Container<Key, Type>, QMetaTypeId2<Container<Key, Type> >::Defined>::reg();  \

#define GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF(Container,          Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, QString, Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, qint16,  Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, qint32,  Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, qint64,  Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, quint16, Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, quint32, Type)     \
    GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, quint64, Type)     \

namespace
{

template<typename T>
void registerContainers()
{
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QList,          T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QQueue,         T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QVector,        T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QStack,         T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QSet,           T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  QLinkedList,    T )

  GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF( QHash,          T )
  GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF( QMap,           T )

  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  std::deque,     T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  std::vector,    T )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(  std::list,      T )
  GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF( std::map,       T )
}

}

template<typename RealType, typename HandleAs>
int registerMetaType()
{
  MetaType::internalLock();

  const int id = internalRegisterType<RealType, HandleAs>();

  registerContainers<RealType>();

  MetaType::internalUnlock();

  return id;
}

/*
 * Register a type so grantlee knows how to handle it.
 *
 * This is a convenience method.
 */
template<typename Type>
int registerMetaType()
{
  return registerMetaType<Type, Type>();
}

// http://catb.org/jargon/html/magic-story.html
enum {
  Magic,
  MoreMagic
};

} // namespace Grantlee

#define GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER(Container)             \
namespace Grantlee {                                                  \
template<typename T>                                                  \
struct RegisterTypeContainer<Container<T>, MoreMagic>                 \
{                                                                     \
  static int reg()                                                    \
  {                                                                   \
    return registerSequentialContainer<Container<T> >();              \
  }                                                                   \
};                                                                    \
}                                                                     \

#define GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER(Container)                     \
namespace Grantlee {                                                           \
template<typename T, typename U>                                               \
struct RegisterTypeContainer<Container<T, U>, MoreMagic>                       \
{                                                                              \
  static int reg()                                                             \
  {                                                                            \
    return registerAssociativeContainer<Container<T, U> >();                   \
  }                                                                            \
};                                                                             \
}                                                                              \

#define GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_AS(Container, As)               \
namespace Grantlee {                                                           \
template<typename T>                                                           \
struct RegisterTypeContainer<Container<T>, MoreMagic>                          \
{                                                                              \
  static int reg()                                                             \
  {                                                                            \
    return registerSequentialContainer<Container<T>, As<T> >();                \
  }                                                                            \
};                                                                             \
}                                                                              \

GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (QList)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_AS (QQueue, QList)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (QVector)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_AS (QStack, QVector)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (QSet) // Actually associative, but iterated as a sequential.
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (QLinkedList)
GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER   (QHash)
GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER   (QMap)

GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (std::deque)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (std::vector)
GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER    (std::list)
GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER   (std::map)

#define GRANTLEE_BEGIN_LOOKUP(TYPE)                                                      \
namespace Grantlee                                                                       \
{                                                                                        \
template<>                                                                               \
inline QVariant TypeAccessor<TYPE>::lookUp( const TYPE object, const QString &property ) \
{                                                                                        \

#define GRANTLEE_END_LOOKUP                                                              \
  return QVariant();                                                                     \
}                                                                                        \
}                                                                                        \


#endif // #define GRANTLEE_METATYPE_H

