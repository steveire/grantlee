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

#include "integers.h"

#include "util.h"

QVariant AddFilter::doFilter(const QVariant &input, const QVariant &argument,
                             bool autoescape) const
{
  Q_UNUSED(autoescape)

  if (isSafeString(input)) {
    if (isSafeString(argument))
      return getSafeString(input) + getSafeString(argument);
    return input;
  }

  if (input.userType() == qMetaTypeId<QVariantList>()) {
    if (argument.userType() == qMetaTypeId<QVariantList>())
      return input.value<QVariantList>() + argument.value<QVariantList>();
    return input;
  }

  if (input.userType() == qMetaTypeId<QStringList>()) {
    if (argument.userType() == QVariant::StringList)
      return input.value<QStringList>() + argument.value<QStringList>();
    return input;
  }

  if (input.userType() == qMetaTypeId<int>()) {
    if (argument.userType() == qMetaTypeId<int>())
      return input.value<int>() + argument.value<int>();
    return input;
  }

  if (input.userType() == qMetaTypeId<uint>()) {
    if (argument.userType() == qMetaTypeId<uint>())
      return input.value<uint>() + argument.value<uint>();
    return input;
  }

  if (input.canConvert<double>()) {
    if (argument.canConvert<double>())
      return input.value<double>() + argument.value<double>();
    return input;
  }

  if (input.userType() == qMetaTypeId<long long>()) {
    if (argument.userType() == qMetaTypeId<long long>())
      return input.value<long long>() + argument.value<long long>();
    return input;
  }

  if (input.userType() == qMetaTypeId<unsigned long long>()) {
    if (input.userType() == qMetaTypeId<unsigned long long>())
      return input.value<unsigned long long>()
             + argument.value<unsigned long long>();
    return input;
  }
  return input;
}

QVariant GetDigitFilter::doFilter(const QVariant &input,
                                  const QVariant &argument,
                                  bool autoescape) const
{
  Q_UNUSED(autoescape)
  auto value = getSafeString(input);

  bool ok;
  (void)value.get().toInt(&ok);
  if (!ok)
    return QString();

  if (value.get().size() < 1)
    return value;

  auto arg = getSafeString(argument).get().toInt();

  if (value.get().size() < arg)
    return value;

  return SafeString(value.get().at(value.get().size() - arg));
}
