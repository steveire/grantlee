/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_UTIL_P_H
#define GRANTLEE_UTIL_P_H

#include <QtCore/QVariant>

#include "context.h"
#include "safestring.h"

#include "grantlee_core_export.h"

class GRANTLEE_CORE_EXPORT Util
{
public:
  Util();

  static bool variantIsTrue( const QVariant &variant );

  // TODO: Make this return an iterator instead.
  static QVariantList variantToList( const QVariant &variant );

  static QString unescapeStringLiteral( const QString &input );

  static Grantlee::SafeString conditionalEscape( const Grantlee::SafeString &input );

  static Grantlee::SafeString markSafe( const Grantlee::SafeString &input );

  static Grantlee::SafeString markForEscaping( const Grantlee::SafeString& arg1 );

  static Grantlee::SafeString escape( const Grantlee::SafeString &input );

  static Grantlee::SafeString getSafeString( const QVariant &input );

  static bool isSafeString( const QVariant &input );

  /**
  Returns true if the type of @p input can be inserted into a rendered template directly.
  Not that lists, hashes and QObject*s can not be.
  */
  static bool supportedOutputType( const QVariant &input );

  static bool equals( const QVariant &lhs, const QVariant &rhs );

  static Grantlee::SafeString toString( const QVariantList &list );

};

#endif

