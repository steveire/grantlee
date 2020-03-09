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

#include "grantlee_templates_export.h"

#include "typeaccessor.h"

#include <QtCore/QVariant>

/// @file

namespace Grantlee
{

/// @headerfile metatype.h grantlee/metatype.h

#ifndef Q_QDOC
/**
  @brief The **%MetaType** is the interface to the Grantlee introspection
  system.

  The **%MetaType** class is used as part of the type registration system of
  %Grantlee.

  @see @ref generic_types_and_templates
  @author Michael Jansen <kde@michael-jansen.biz>
  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TEMPLATES_EXPORT MetaType
{
public:
  /**
    @internal The signature for a property lookup method
   */
  typedef QVariant (*LookupFunction)(const QVariant &, const QString &);

  /**
    @internal Registers a property lookup method
   */
  static void registerLookUpOperator(int id, LookupFunction f);

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
  static QVariant lookup(const QVariant &object, const QString &property);

  /**
    @internal
   */
  static bool lookupAlreadyRegistered(int id);

private:
  MetaType();
};
#endif

namespace
{

/*
  This is a helper to select an appropriate overload of indexAccess
 */
template <typename RealType, typename HandleAs> struct LookupTrait {
  static QVariant doLookUp(const QVariant &object, const QString &property)
  {
    typedef typename Grantlee::TypeAccessor<RealType> Accessor;
    return Accessor::lookUp(object.value<RealType>(), property);
  }
};

template <typename RealType, typename HandleAs>
struct LookupTrait<RealType &, HandleAs &> {
  static QVariant doLookUp(const QVariant &object, const QString &property)
  {
    typedef typename Grantlee::TypeAccessor<HandleAs &> Accessor;
    return Accessor::lookUp(object.value<HandleAs>(), property);
  }
};

template <typename RealType, typename HandleAs> static int doRegister(int id)
{
  if (MetaType::lookupAlreadyRegistered(id))
    return id;

  QVariant (*lf)(const QVariant &, const QString &)
      = LookupTrait<RealType, HandleAs>::doLookUp;

  MetaType::registerLookUpOperator(
      id, reinterpret_cast<MetaType::LookupFunction>(lf));

  return id;
}

/*
  Register a type so grantlee knows how to handle it.
 */
template <typename RealType, typename HandleAs> struct InternalRegisterType {
  static int doReg()
  {
    const int id = qMetaTypeId<RealType>();
    return doRegister<RealType &, HandleAs &>(id);
  }
};

template <typename RealType, typename HandleAs>
struct InternalRegisterType<RealType *, HandleAs *> {
  static int doReg()
  {
    const int id = qMetaTypeId<RealType *>();
    return doRegister<RealType *, HandleAs *>(id);
  }
};
}

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

    // Only the introspectable API from SomeType is needed, so we can reuse that
  registration.
    registerMetaType<OtherType, SomeType>();
  @endcode

  @see @ref generic_types_and_templates
 */
template <typename RealType, typename HandleAs> int registerMetaType()
{
  MetaType::internalLock();

  const int id = InternalRegisterType<RealType, HandleAs>::doReg();

  MetaType::internalUnlock();

  return id;
}

#ifndef Q_QDOC
/**
  @internal
  Register a type so %Grantlee knows how to handle it.

  This is a convenience method.
 */
template <typename Type> int registerMetaType()
{
  return registerMetaType<Type, Type>();
}

#endif
} // namespace Grantlee

/**
  Top boundary of a lookup function for Type.

  @see @ref generic_types
 */
#define GRANTLEE_BEGIN_LOOKUP(Type)                                            \
  namespace Grantlee                                                           \
  {                                                                            \
  template <>                                                                  \
  inline QVariant TypeAccessor<Type &>::lookUp(const Type &object,             \
                                               const QString &property)        \
  {

/**
  Top boundary of a lookup function for Type*.

  @see @ref generic_types
 */
#define GRANTLEE_BEGIN_LOOKUP_PTR(Type)                                        \
  namespace Grantlee                                                           \
  {                                                                            \
  template <>                                                                  \
  inline QVariant TypeAccessor<Type *>::lookUp(const Type *const object,       \
                                               const QString &property)        \
  {

/**
  Bottom boundary of a lookup function for Type.

  @see @ref generic_types
 */
#define GRANTLEE_END_LOOKUP                                                    \
  return QVariant();                                                           \
  }                                                                            \
  }

#endif // #define GRANTLEE_METATYPE_H
