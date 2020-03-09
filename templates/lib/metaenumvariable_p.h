/*
  This file is part of the Grantlee template system.

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

#ifndef GRANTLEE_METAENUMVARIABLE_P_H
#define GRANTLEE_METAENUMVARIABLE_P_H

#include <QtCore/QMetaEnum>

struct MetaEnumVariable {
  MetaEnumVariable() : value(-1) {}

  MetaEnumVariable(QMetaEnum _enumerator) : enumerator(_enumerator), value(-1)
  {
  }

  MetaEnumVariable(QMetaEnum _enumerator, int _value)
      : enumerator(_enumerator), value(_value)
  {
  }

  bool operator==(const MetaEnumVariable &other) const
  {
    return (enumerator.scope() == other.enumerator.scope()
            && enumerator.name() == other.enumerator.name())
           && value == other.value;
  }

  bool operator==(int otherValue) const { return value == otherValue; }

  QMetaEnum enumerator;
  int value;
};

Q_DECLARE_METATYPE(MetaEnumVariable)

#endif
