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
    return variant.value<int>() > 0;
  }
  case QVariant::Double: {
    return variant.value<double>() > 0;
  }
  case QMetaType::Float: {
    return variant.value<float>() > 0;
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
    return !variant.value<QVariantList>().isEmpty();
  }
  case QVariant::Hash: {
    return !variant.value<QVariantHash>().isEmpty();
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
  if (lhs.userType() == qMetaTypeId<Grantlee::SafeString>() &&
      (rhs.userType() == qMetaTypeId<QString>() || rhs.userType() == qMetaTypeId<QByteArray>())) {
    equal = lhs.value<Grantlee::SafeString>() == rhs.value<QString>();
  } else if (rhs.userType() == qMetaTypeId<Grantlee::SafeString>() &&
      (lhs.userType() == qMetaTypeId<QString>() || lhs.userType() == qMetaTypeId<QByteArray>())) {
    equal = rhs.value<Grantlee::SafeString>() == lhs.value<QString>();
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

bool Grantlee::lessThan(const QVariant &lhs, const QVariant &rhs)
{
  // TODO: Redesign...

  // QVariant doesn't use operator== to compare SafeString to QString or QByteArray
  if (lhs.userType() == qMetaTypeId<Grantlee::SafeString>()) {
    if (rhs.userType() == QVariant::String || rhs.userType() == QVariant::ByteArray) {
      return lhs.value<Grantlee::SafeString>() < rhs.value<QString>();
    }
  } else if (rhs.userType() == qMetaTypeId<Grantlee::SafeString>() && (lhs.userType() == QVariant::String || lhs.userType() == QVariant::ByteArray)) {
    if (lhs.userType() == QVariant::String) {
      return rhs.value<Grantlee::SafeString>() < lhs.value<QString>();
    }
  } else if (rhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
      return rhs.value<MetaEnumVariable>().value < lhs.value<MetaEnumVariable>().value;
    } else if (lhs.type() == QVariant::Int) {
      return rhs.value<MetaEnumVariable>().value < lhs.value<int>();
    }
  } else if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (rhs.type() == QVariant::Int) {
      return lhs.value<MetaEnumVariable>().value < rhs.value<int>();
    }
  }

  return lhs < rhs;
}

bool Grantlee::contains( const QVariant &lhs, const QVariant &rhs )
{
  // TODO: Redesign...

  int rUserType = rhs.userType();
  int lUserType = lhs.userType();
  if (rUserType == qMetaTypeId<Grantlee::SafeString>()) {
    const QString right = rhs.value<Grantlee::SafeString>();
    if (lUserType == QVariant::String || lUserType == QVariant::ByteArray) {
      return right.contains(lhs.value<QString>());
    } else if ( lUserType == qMetaTypeId<Grantlee::SafeString>() ) {
      const QString left = lhs.value<Grantlee::SafeString>();
      return right.contains(left);
    }
  } else if (rUserType == QVariant::String) {
      const QString right = rhs.value<QString>();
      if (lUserType == QVariant::String || lUserType == QVariant::ByteArray) {
        return right.contains(lhs.value<QString>());
      } else if (lUserType == qMetaTypeId<Grantlee::SafeString>()) {
        const QString left = lhs.value<Grantlee::SafeString>();
        return right.contains(left);
      }
  } else if (rUserType == QVariant::ByteArray) {
      if (lUserType == QVariant::String) {
        const QString right = QString::fromUtf8(rhs.value<QByteArray>());
        return right.contains(lhs.value<QString>());
      } else if (lUserType == QVariant::ByteArray) {
        const QByteArray right = rhs.value<QByteArray>();
        return right.contains(lhs.value<QByteArray>());
      } else if (lUserType == qMetaTypeId<Grantlee::SafeString>()) {
        const QString right = QString::fromUtf8( rhs.value<QByteArray>());
        const QString left = lhs.value<Grantlee::SafeString>();
        return right.contains(left);
      }
  } else if (rUserType == QVariant::List) {
    QVariantList list = rhs.toList();
    Q_FOREACH (const QVariant &item, list) {
      if (equals( item, lhs)) {
        return true;
      }
    }
    return false;
  } else if (rUserType == QVariant::StringList) {
    QStringList list = rhs.toStringList();
    if (lUserType == QVariant::String || lUserType == QVariant::ByteArray) {
      return list.contains(lhs.value<QString>());
    } else if ( lUserType == qMetaTypeId<Grantlee::SafeString>()) {
      QString left = lhs.value<Grantlee::SafeString>();
      return list.contains(left);
    }
    return false;
  }

  if (rhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
      return rhs.value<MetaEnumVariable>() == lhs.value<MetaEnumVariable>();
    } else if (lhs.type() == QVariant::Int) {
      return rhs.value<MetaEnumVariable>() == lhs.value<int>();
    }
  } else if (lhs.userType() == qMetaTypeId<MetaEnumVariable>()) {
    if (rhs.type() == QVariant::Int) {
      return lhs.value<MetaEnumVariable>() == rhs.value<int>();
    }
  }

  return lhs == rhs;
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
