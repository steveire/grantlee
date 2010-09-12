/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Michael Jansen <kde@michael-jansen.biz>

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

namespace Grantlee
{

class GRANTLEE_CORE_EXPORT MetaType
{
public:
  typedef QVariant (*LookupFunction)(const QVariant &, const QString &);

  static void registerLookUpOperator( int id, LookupFunction f );

  static QVariant lookup( const QVariant &object, const QString &property );

  static bool lookupAlreadyRegistered(int id);

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

}

/*
 * Register a type so grantlee knows how to handle it.
 *
 * :TODO: Real docu
 */
template<typename RealType, typename HandleAs>
int registerMetaType()
{
  const int id = qMetaTypeId<RealType>();

  if ( MetaType::lookupAlreadyRegistered( id ) )
    return id;

  QVariant ( *lf )(const QVariant&, const QString&) = LookupTrait<RealType, HandleAs>::doLookUp;

  MetaType::registerLookUpOperator( id, reinterpret_cast<MetaType::LookupFunction>( lf ) );

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

} // namespace Grantlee

#endif // #define GRANTLEE_METATYPE_H

