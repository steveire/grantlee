/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#include "util.h"

#include "metaenumvariable_p.h"
#include "metatype.h"

#include <QtCore/QStringList>

QString Grantlee::unescapeStringLiteral(const QString &input)
{
  return input.mid(1, input.size() - 2)
      .replace(QStringLiteral("\\\'"), QChar::fromLatin1('\''))
      .replace(QStringLiteral("\\\""), QChar::fromLatin1('"'))
      .replace(QStringLiteral("\\\\"), QChar::fromLatin1('\\'));
}

bool Grantlee::variantIsTrue(const QVariant &variant)
{

  if (!variant.isValid())
    return false;
  switch (variant.userType()) {
  case QVariant::Bool: {
    return variant.value<bool>();
  }
  case QVariant::Int: {
    return (variant.value<int>() > 0);
  }
  case QVariant::Double: {
    return (variant.value<double>() > 0);
  }
  case QMetaType::Float: {
    return (variant.value<float>() > 0);
  }
  case QMetaType::QObjectStar: {
    auto obj = variant.value<QObject *>();
    if (!obj)
      return false;

    if (obj->property("__true__").isValid()) {
      return obj->property("__true__").value<bool>();
    }
    return true;
  }
  case QVariant::List: {
    return (variant.value<QVariantList>().size() > 0);
  }
  case QVariant::Hash: {
    return (variant.value<QVariantHash>().size() > 0);
  }
  }

  return !getSafeString(variant).get().isEmpty();
}

Grantlee::SafeString Grantlee::markSafe(const Grantlee::SafeString &input)
{
  auto sret = input;
  sret.setSafety(Grantlee::SafeString::IsSafe);
  return sret;
}

Grantlee::SafeString
Grantlee::markForEscaping(const Grantlee::SafeString &input)
{
  auto temp = input;
  if (input.isSafe() || input.needsEscape())
    return input;

  temp.setNeedsEscape(true);
  return temp;
}

Grantlee::SafeString Grantlee::getSafeString(const QVariant &input)
{
  if (input.userType() == qMetaTypeId<Grantlee::SafeString>()) {
    return input.value<Grantlee::SafeString>();
  } else {
    return input.value<QString>();
  }
}

bool Grantlee::isSafeString(const QVariant &input)
{
  const auto type = input.userType();
  return ((type == qMetaTypeId<Grantlee::SafeString>())
          || type == QVariant::String);
}

static QList<int> getPrimitives()
{
  QList<int> primitives;
  primitives << qMetaTypeId<Grantlee::SafeString>() << QVariant::String
             << QVariant::Bool << QVariant::Int << QVariant::Double
             << QMetaType::Float << QVariant::Date << QVariant::Time
             << QVariant::DateTime;
  return primitives;
}

bool Grantlee::supportedOutputType(const QVariant &input)
{
  static const auto primitives = getPrimitives();
  return primitives.contains(input.userType());
}

bool Grantlee::equals(const QVariant &lhs, const QVariant &rhs)
{

  // TODO: Redesign...

  // QVariant doesn't use operator== to compare its held data, so we do it
  // manually instead for SafeString.
  auto equal = false;
  if (lhs.userType() == qMetaTypeId<Grantlee::SafeString>()) {
    if (rhs.userType() == qMetaTypeId<Grantlee::SafeString>()) {
      equal = (lhs.value<Grantlee::SafeString>()
               == rhs.value<Grantlee::SafeString>());
    } else if (rhs.userType() == QVariant::String) {
      equal = (lhs.value<Grantlee::SafeString>() == rhs.value<QString>());
    }
  } else if (rhs.userType() == qMetaTypeId<Grantlee::SafeString>()
             && lhs.userType() == QVariant::String) {
    equal = (rhs.value<Grantlee::SafeString>() == lhs.value<QString>());
  } else if (rhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
      equal = (rhs.value<MetaEnumVariable>() == lhs.value<MetaEnumVariable>());
    } else if (lhs.userType() == qMetaTypeId<int>()) {
      equal = (rhs.value<MetaEnumVariable>() == lhs.value<int>());
    }
  } else if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (rhs.userType() == qMetaTypeId<int>()) {
      equal = (lhs.value<MetaEnumVariable>() == rhs.value<int>());
    }
  } else {
    equal = ((lhs == rhs) && (lhs.userType() == rhs.userType()));
  }
  return equal;
}

Grantlee::SafeString Grantlee::toString(const QVariantList &list)
{
  QString output(QLatin1Char('['));
  auto it = list.constBegin();
  const auto end = list.constEnd();
  while (it != end) {
    const auto item = *it;
    if (isSafeString(item)) {
      output += QStringLiteral("u\'")
                + static_cast<QString>(getSafeString(item).get())
                + QLatin1Char('\'');
    }
    if ((item.userType() == qMetaTypeId<int>())
        || (item.userType() == qMetaTypeId<uint>())
        || (item.userType() == qMetaTypeId<double>())
        || (item.userType() == qMetaTypeId<float>())
        || (item.userType() == qMetaTypeId<long long>())
        || (item.userType() == qMetaTypeId<unsigned long long>())) {
      output += item.value<QString>();
    }
    if (item.userType() == qMetaTypeId<QVariantList>()) {
      output
          += static_cast<QString>(toString(item.value<QVariantList>()).get());
    }
    if ((it + 1) != end)
      output += QStringLiteral(", ");
    ++it;
  }

  return output.append(QLatin1Char(']'));
}
