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

#ifndef CUSTOMTYPESREGISTRY_P_H
#define CUSTOMTYPESREGISTRY_P_H

#include "metatype.h"
#include "containeraccessor.h"

#include <QtCore/QMutex>

namespace Grantlee
{

template<typename ContainedType>
struct VariantToList
{
  static QVariantList doConvert( const QVariant &obj );
};

struct CustomTypeInfo
{
 public:
  CustomTypeInfo()
    : lookupFunction( 0 ),
      toVariantListFunction( 0 )
  {
  }

  Grantlee::MetaType::LookupFunction lookupFunction;
  Grantlee::MetaType::ToVariantListFunction toVariantListFunction;
};

struct CustomTypeRegistry
{
  CustomTypeRegistry();

  void registerLookupOperator( int id, MetaType::LookupFunction f );
  void registerToListOperator( int id, MetaType::ToVariantListFunction f );

  template<typename RealType, typename HandleAs>
  int registerBuiltInMetatype()
  {
    QVariant ( *lf )( const QVariant&, const QString& ) = LookupTrait<RealType, HandleAs>::doLookUp;

    const int id = qMetaTypeId<RealType>();

    registerLookupOperator( id, reinterpret_cast<MetaType::LookupFunction>( lf ) );

    return id;
  }

  template<typename Type>
  int registerBuiltInMetatype()
  {
    return registerBuiltInMetatype<Type, Type>();
  }

  template<typename Type, typename HandleAs>
  int registerVariantToList()
  {
    QVariantList ( *vtlf )( const QVariant& ) = VariantToList<HandleAs>::doConvert;

    const int id = qMetaTypeId<Type>();

    registerToListOperator( id, reinterpret_cast<MetaType::ToVariantListFunction>( vtlf ) );

    return id;
  }

  template<typename Type>
  int registerVariantToList()
  {
    return registerVariantToList<Type, Type>();
  }

  QVariant lookup( const QVariant &object, const QString &property ) const;
  QVariantList toVariantList( const QVariant &variant ) const;
  bool lookupAlreadyRegistered( int id ) const;
  bool toListAlreadyRegistered( int id ) const;

  QHash<int, CustomTypeInfo> types;
  QMutex mutex;
};

}

#endif
