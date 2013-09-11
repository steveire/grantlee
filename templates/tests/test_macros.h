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

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include "metatype.h"

#include <map>

#ifndef GRANTLEE_NO_TR1
#include "qtunorderedmap.h"

#ifdef Q_CC_MSVC
#include <array>
#else
#include <tr1/array>
#endif

template <typename T>
struct ThreeArray : public std::tr1::array<T, 3>
{

};

#define DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, KeyType, ValueType)           \
  typedef ContainerType<KeyType, ValueType> ContainerType##KeyType##ValueType;     \
  Q_DECLARE_METATYPE(ContainerType##KeyType##ValueType)                            \

#define DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL(ContainerType, ValueType)  \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint32,     ValueType)          \

#define DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL(ContainerType, ValueType)  \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint16,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint32,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, qint64,     ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint16,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint32,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, quint64,    ValueType)      \
  DECLARE_ASSOCIATIVE_CONTAINER(ContainerType, QString,    ValueType)      \

#ifndef MINIMAL_CONTAINER_TESTS
#  define DECLARE_ASSOCIATIVE_CONTAINER_TYPES DECLARE_ASSOCIATIVE_CONTAINER_TYPES_ALL
#else
#  define DECLARE_ASSOCIATIVE_CONTAINER_TYPES DECLARE_ASSOCIATIVE_CONTAINER_TYPES_MINIMAL
#endif

#endif

#endif
