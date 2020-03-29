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

#include "customtyperegistry_p.h"

#include "metaenumvariable_p.h"
#include "safestring.h"

#include <QtCore/QLoggingCategory>
#include <QtCore/QQueue>
#include <QtCore/QStack>
#include <QtCore/QStringList>

Q_LOGGING_CATEGORY(GRANTLEE_CUSTOMTYPE, "grantlee.customtype")

using namespace Grantlee;

CustomTypeRegistry::CustomTypeRegistry()
{
  // Grantlee Types
  registerBuiltInMetatype<SafeString>();
  registerBuiltInMetatype<MetaEnumVariable>();
}

void CustomTypeRegistry::registerLookupOperator(int id,
                                                MetaType::LookupFunction f)
{
  CustomTypeInfo &info = types[id];
  info.lookupFunction = f;
}

QVariant CustomTypeRegistry::lookup(const QVariant &object,
                                    const QString &property) const
{
  if (!object.isValid())
    return QVariant();
  const auto id = object.userType();
  MetaType::LookupFunction lf;

  {
    auto it = types.constFind(id);
    if (it == types.constEnd()) {
      qCWarning(GRANTLEE_CUSTOMTYPE)
          << "Don't know how to handle metatype" << QMetaType::typeName(id);
      // :TODO: Print out error message
      return QVariant();
    }

    const CustomTypeInfo &info = it.value();
    if (!info.lookupFunction) {
      qCWarning(GRANTLEE_CUSTOMTYPE)
          << "No lookup function for metatype" << QMetaType::typeName(id);
      lf = nullptr;
      // :TODO: Print out error message
      return QVariant();
    }

    lf = info.lookupFunction;
  }

  return lf(object, property);
}

bool CustomTypeRegistry::lookupAlreadyRegistered(int id) const
{
  auto it = types.constFind(id);
  if (it != types.constEnd()) {
    return it.value().lookupFunction != nullptr;
  }
  return false;
}
