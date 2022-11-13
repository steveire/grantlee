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

#include <cfloat>
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
  case QMetaType::Bool: {
    return variant.value<bool>();
  }
  case QMetaType::Int: {
    return variant.value<int>() > 0;
  }
  case QMetaType::UInt: {
    return variant.value<uint>() > 0;
  }
  case QMetaType::LongLong: {
    return variant.value<qlonglong>() > 0;
  }
  case QMetaType::ULongLong: {
    return variant.value<qulonglong>() > 0;
  }
  case QMetaType::Double: {
    return variant.value<double>() > 0;
  }
  case QMetaType::Float: {
    return variant.value<float>() > 0;
  }
  case QMetaType::Char: {
    return variant.value<char>() > 0;
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
  case QMetaType::QVariantList: {
    return !variant.value<QVariantList>().isEmpty();
  }
  case QMetaType::QVariantHash: {
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
  }
  return input.value<QString>();
}

bool Grantlee::isSafeString(const QVariant &input)
{
  const auto type = input.userType();
  return ((type == qMetaTypeId<Grantlee::SafeString>())
          || type == QMetaType::QString);
}

static QList<int> getPrimitives()
{
  QList<int> primitives;
  primitives << qMetaTypeId<Grantlee::SafeString>() << QMetaType::QString
             << QMetaType::Bool << QMetaType::Int << QMetaType::Double
             << QMetaType::Float << QMetaType::QDate << QMetaType::QTime
             << QMetaType::QDateTime;
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
    } else if (rhs.userType() == QMetaType::QString) {
      equal = (lhs.value<Grantlee::SafeString>() == rhs.value<QString>());
    }
  } else if (rhs.userType() == qMetaTypeId<Grantlee::SafeString>()
             && lhs.userType() == QMetaType::QString) {
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
    equal = (lhs == rhs);
  }
  return equal;
}

std::pair<qreal, QString> Grantlee::calcFileSize(qreal size, int unitSystem,
                                                 qreal multiplier)
{
  std::pair<qreal, QString> ret;

  int _unitSystem = unitSystem;

  if ((_unitSystem != 2) && (_unitSystem != 10)) {
    qWarning("%s", "Unrecognized file size unit system. Falling back to "
                   "decimal unit system.");
    _unitSystem = 10;
  }

  if (size == 0.0) {
    ret.first = 0.0;
    ret.second = QStringLiteral("bytes");
    return ret;
  }
  if ((size == 1.0) || (size == -1.0)) {
    ret.first = 1.0;
    ret.second = QStringLiteral("byte");
    return ret;
  }

  qreal _size = size * multiplier;

  const bool positiveValue = (_size > 0);

  if (!positiveValue) {
    _size *= -1;
  }

  static const QStringList binaryUnits(
      {QStringLiteral("bytes"), QStringLiteral("KiB"), QStringLiteral("MiB"),
       QStringLiteral("GiB"), QStringLiteral("TiB"), QStringLiteral("PiB"),
       QStringLiteral("EiB"), QStringLiteral("ZiB"), QStringLiteral("YiB")});

  static const QStringList decimalUnits(
      {QStringLiteral("bytes"), QStringLiteral("KB"), QStringLiteral("MB"),
       QStringLiteral("GB"), QStringLiteral("TB"), QStringLiteral("PB"),
       QStringLiteral("EB"), QStringLiteral("ZB"), QStringLiteral("YB")});

  bool found = false;
  int count = 0;
  const qreal baseVal = (_unitSystem == 10) ? 1000.0F : 1024.0F;
#if FLT_EVAL_METHOD == 2
  // Avoid that this is treated as long double, as the increased
  // precision breaks the comparison below.
  volatile qreal current = 1.0F;
#else
  qreal current = 1.0F;
#endif
  int units = decimalUnits.size();
  while (!found && (count < units)) {
    current *= baseVal;
    if (_size < current) {
      found = true;
      break;
    }
    count++;
  }

  if (count >= units) {
    count = (units - 1);
  }

  qreal devider = current / baseVal;
  _size = _size / devider;

  if (!positiveValue) {
    _size *= -1.0;
  }

  ret.first = _size;
  ret.second
      = (_unitSystem == 10) ? decimalUnits.at(count) : binaryUnits.at(count);

  return ret;
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
