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
#include <QtCore/QDateTime>

#include <deque>
#include <list>
#include <map>
#include <vector>

/// @file

namespace Grantlee
{

/// @headerfile metatype.h grantlee/metatype.h

#ifndef Q_QDOC
/**
  @brief The MetaType is the interface to the Grantlee introspection system.

  The MetaType class is used as part of the type registration system of %Grantlee.

  The init method should be called in some location where types to be introspected are defined.

  @see @ref generic_types_and_templates
  @author Michael Jansen <kde@michael-jansen.biz>
  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT MetaType
{
public:
  /**
    @internal The signature for a property lookup method
   */
  typedef QVariant ( *LookupFunction )( const QVariant &, const QString & );

  /**
    @internal The signature for a object to list method
   */
  typedef QVariantList ( *ToVariantListFunction )( const QVariant & );

  /**
    @internal Registers a property lookup method
   */
  static void registerLookUpOperator( int id, LookupFunction f );

  /**
    @internal Registers a object to list method
   */
  static void registerToVariantListOperator( int id, ToVariantListFunction f );

  /**
    @internal
   */
  static void internalLock();

  /**
    @internal
   */
  static void internalUnlock();

  /**
    @internal
   */
  static QVariant lookup( const QVariant &object, const QString &property );

  /**
    @internal
   */
  static QVariantList toVariantList( const QVariant &obj );

  /**
    @internal
   */
  static bool lookupAlreadyRegistered( int id );

  /**
    @internal
   */
  static bool toListAlreadyRegistered( int id );

  /**
    Initializes the MetaType system with built in types and containers.
   */
  static inline int init();

  /**
    @internal
   */
  static int initBuiltins() { return init(); }

private:
  MetaType();
};
#endif

namespace
{

/*
 * This is a helper to select an appropriate overload of indexAccess
 */
template<typename RealType, typename HandleAs>
struct LookupTrait
{
  static QVariant doLookUp( const QVariant &object, const QString &property );
};

template<typename T>
struct IsQObjectStar
{
  enum { Yes = false };
};

template<typename T>
struct IsQObjectStar<T*>
{
  typedef int yes_type;
  typedef char no_type;

  static yes_type check(QObject*);
  static no_type check(...);
  enum { Yes = sizeof(check(static_cast<T*>(0))) == sizeof(yes_type) };
};

template<typename T, bool>
struct LookupPointer
{
  static QVariant doLookUp( const QVariant &object, const QString &property )
  {
    typedef typename Grantlee::TypeAccessor<T> Accessor;
    return Accessor::lookUp( object.value<T>(), property );
  }
};

template<typename T>
struct LookupPointer<T, true>
{
  static QVariant doLookUp( const QVariant &object, const QString &property )
  {
    typedef typename Grantlee::TypeAccessor<QObject*> Accessor;
    return Accessor::lookUp( object.value<T>(), property );
  }
};

template<typename RealType>
struct LookupTrait<RealType*, RealType*>
{
  static QVariant doLookUp( const QVariant &object, const QString &property )
  {
    return LookupPointer<RealType*, IsQObjectStar<RealType*>::Yes>::doLookUp(object, property);
  }
};

template<typename RealType, typename HandleAs>
struct LookupTrait<RealType&, HandleAs&>
{
  static QVariant doLookUp( const QVariant &object, const QString &property )
  {
    typedef typename Grantlee::TypeAccessor<HandleAs&> Accessor;
    return Accessor::lookUp( static_cast<HandleAs>( object.value<RealType>() ), property );
  }
};

template<typename RealType, typename HandleAs>
static int doRegister( int id )
{
  if ( MetaType::lookupAlreadyRegistered( id ) )
    return id;

  QVariant ( *lf )( const QVariant&, const QString& ) = LookupTrait<RealType, HandleAs>::doLookUp;

  MetaType::registerLookUpOperator( id, reinterpret_cast<MetaType::LookupFunction>( lf ) );

  return id;
}

/*
 * Register a type so grantlee knows how to handle it.
 */
template<typename RealType, typename HandleAs>
struct InternalRegisterType
{
  static int doReg() {
    const int id = qMetaTypeId<RealType>();
    return doRegister<RealType&, HandleAs&>( id );
  }
};

template<typename RealType, typename HandleAs>
struct InternalRegisterType<RealType*, HandleAs*>
{
  static int doReg() {
    const int id = qMetaTypeId<RealType*>();
    return doRegister<RealType*, HandleAs*>( id );
  }
};

template<typename Container, typename HandleAs>
int registerSequentialContainer()
{
  const int id = InternalRegisterType<Container, HandleAs>::doReg();

  if ( MetaType::toListAlreadyRegistered( id ) )
    return id;

  QVariantList ( *tlf )( const QVariant& ) = SequentialContainerAccessor<Container>::doToList;
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
  const int id = InternalRegisterType<Container, HandleAs>::doReg();

  if ( MetaType::toListAlreadyRegistered( id ) )
    return id;

  QVariantList ( *tlf )( const QVariant& ) = AssociativeContainerAccessor<Container>::doToList;
  MetaType::registerToVariantListOperator( id, reinterpret_cast<MetaType::ToVariantListFunction>( tlf ) );
  return id;
}

template<typename Container>
int registerAssociativeContainer()
{
  return registerAssociativeContainer<Container, Container>();
}

}

#ifndef Q_QDOC
/**
  @internal Registers a Type with the system so that containers of the type declared as a QMetaType can be used.

  This is an implementation detail. GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF or GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF
  should be used instead.
 */
template<typename RealType, int n>
struct RegisterTypeContainer
{
  static void reg()
  {
  }
};
#endif

/**
  Registers Container&lt;Type&gt; with %Grantlee if it has been declared as a metatype.

  @see ref third_party_containers
*/
#define GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(Container, Type)                                   \
  Grantlee::RegisterTypeContainer<Container<Type>, QMetaTypeId2<Container<Type> >::Defined>::reg();  \

#ifndef Q_QDOC
/**
 @internal
*/
#define GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_KEY_IF(Container, Key, Type)                                   \
  Grantlee::RegisterTypeContainer<Container<Key, Type>, QMetaTypeId2<Container<Key, Type> >::Defined>::reg();  \

#endif

/**
  Registers Container&lt;%Key, Type&gt; with %Grantlee if it has been declared as a metatype.

  The following containers will be available to %Grantlee if they were declared as a QMetaType:

  @li Container&lt;QString, Type&gt;
  @li Container&lt;qint16,  Type&gt;
  @li Container&lt;qint32,  Type&gt;
  @li Container&lt;qint64,  Type&gt;
  @li Container&lt;quint16, Type&gt;
  @li Container&lt;quint32, Type&gt;
  @li Container&lt;quint64, Type&gt;

  @see ref third_party_containers
*/
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

struct BuiltinRegister
{
  void registerBuiltinContainers() const
  {
    Grantlee::MetaType::internalLock();

    registerContainers< bool      >();
    registerContainers< qint16    >();
    registerContainers< qint32    >();
    registerContainers< qint64    >();
    registerContainers< quint16   >();
    registerContainers< quint32   >();
    registerContainers< quint64   >();
    registerContainers< float     >();
    registerContainers< double    >();
    registerContainers< QString   >();
    registerContainers< QVariant  >();
    registerContainers< QDateTime >();
    registerContainers< QObject*  >();

    registerSequentialContainer<QStringList, QList<QString> >();
    Grantlee::MetaType::internalUnlock();
  }
};

Q_GLOBAL_STATIC( BuiltinRegister, builtinRegister )

}

#ifndef Q_QDOC
struct MetaTypeInitializer {
  static inline int initialize()
  {
      static const BuiltinRegister *br = builtinRegister();
      br->registerBuiltinContainers();
      return 0;
  }
};
#endif

/**
  Macro to initialize the metatype system.

  @see @ref generic_types
 */
#define GRANTLEE_METATYPE_INITIALIZE static const int i = Grantlee::MetaTypeInitializer::initialize(); Q_UNUSED(i)

#ifndef Q_QDOC
inline int MetaType::init()
{
  GRANTLEE_METATYPE_INITIALIZE
  return 0;
}
#endif

/**
  @brief Registers the type RealType with the metatype system.

  This method can take a second template parameter to specify a cast
  that should be invoked during registration. This is useful if a base type is
  already supported.

  @code
    class SomeType
    {
    public:
      QString someProp() const;
    };

    // define some introspectable API for SomeType

    GRANTLEE_BEGIN_LOOKUP(SomeType)
      if (property == "someProp")
        return object.someProp();
    GRANTLEE_END_LOOKUP


    class OtherType : public SomeType
    {
      // ...
    };

    registerMetaType<SomeType>();

    // Only the introspectable API from SomeType is needed, so we can reuse that registration.
    registerMetaType<OtherType, SomeType>();
  @endcode

  @see @ref generic_types_and_templates
 */
template<typename RealType, typename HandleAs>
int registerMetaType()
{
  {
    GRANTLEE_METATYPE_INITIALIZE
    Q_UNUSED( i )
  }
  MetaType::internalLock();

  const int id = InternalRegisterType<RealType, HandleAs>::doReg();

  registerContainers<RealType>();

  MetaType::internalUnlock();

  return id;
}

#ifndef Q_QDOC
/**
  @internal
  Register a type so grantlee knows how to handle it.

  This is a convenience method.
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

#endif
} // namespace Grantlee

/**
  Registers Container so that it can be accessed by %Grantlee.

  @see @ref third_party_containers
*/
#define GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER(Container)             \
namespace Grantlee {                                                  \
template<typename T>                                                  \
struct RegisterTypeContainer<Container<T>, MoreMagic>                 \
{                                                                     \
  static int reg()                                                    \
  {                                                                   \
    const int id = registerSequentialContainer<Container<T> >();      \
    registerContainers<Container<T> >();                              \
    return id;                                                        \
  }                                                                   \
};                                                                    \
}                                                                     \

/**
  Registers Container so that it can be accessed by %Grantlee.

  @see @ref third_party_containers
*/
#define GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER(Container)                     \
namespace Grantlee {                                                           \
template<typename T, typename U>                                               \
struct RegisterTypeContainer<Container<T, U>, MoreMagic>                       \
{                                                                              \
  static int reg()                                                             \
  {                                                                            \
    const int id = registerAssociativeContainer<Container<T, U> >();           \
    registerContainers<Container<T, U> >();                                    \
    return id;                                                                 \
  }                                                                            \
};                                                                             \
}                                                                              \

#ifndef Q_QDOC
/**
  @internal
 */
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

#endif

/**
  Top boundary of a lookup function for Type.

  @see @ref generic_types
 */
#define GRANTLEE_BEGIN_LOOKUP(Type)                                                        \
namespace Grantlee                                                                         \
{                                                                                          \
template<>                                                                                 \
inline QVariant TypeAccessor<Type&>::lookUp( const Type &object, const QString &property ) \
{                                                                                          \

/**
  Top boundary of a lookup function for Type*.

  @see @ref generic_types
 */
#define GRANTLEE_BEGIN_LOOKUP_PTR(Type)                                                            \
namespace Grantlee                                                                                 \
{                                                                                                  \
template<>                                                                                         \
inline QVariant TypeAccessor<Type*>::lookUp( const Type * const object, const QString &property )  \
{                                                                                                  \

/**
  Bottom boundary of a lookup function for Type.

  @see @ref generic_types
 */
#define GRANTLEE_END_LOOKUP                                                              \
  return QVariant();                                                                     \
}                                                                                        \
}                                                                                        \


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


#endif // #define GRANTLEE_METATYPE_H

