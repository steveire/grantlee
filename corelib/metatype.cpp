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

Q_GLOBAL_STATIC(CustomTypeRegistry, customTypes)

void Grantlee::MetaType::registerLookUpOperator( int id, LookupFunction f )
{
  Q_ASSERT(id>0);
  Q_ASSERT(f);
  if ( id < 0 ) {
    qWarning() << "registerTypeOperators() called with invalid arguments";
    return;
  }
  customTypes()->registerLookupOperator( id, f );
}

void Grantlee::MetaType::registerToVariantListOperator( int id, ToVariantListFunction f )
{
  Q_ASSERT(id>0);
  Q_ASSERT(f);
  if ( id < 0 ) {
    qWarning() << "registerTypeOperators() called with invalid arguments";
    return;
  }
  customTypes()->registerToListOperator( id, f );
}

QVariant Grantlee::MetaType::lookup( const QVariant &object, const QString &property )
{
  return customTypes()->lookup( object, property );
}

QVariantList Grantlee::MetaType::toVariantList( const QVariant &obj )
{
  return customTypes()->toVariantList( obj );
}

bool Grantlee::MetaType::lookupAlreadyRegistered( int id )
{
  return customTypes()->lookupAlreadyRegistered( id );
}

bool Grantlee::MetaType::toListAlreadyRegistered( int id )
{
  return customTypes()->toListAlreadyRegistered( id );
}
