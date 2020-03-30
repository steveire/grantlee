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

#ifndef GRANTLEE_UTIL_P_H
#define GRANTLEE_UTIL_P_H

#include "context.h"
#include "grantlee_templates_export.h"
#include "safestring.h"

#include <QtCore/QVariant>

/// @file util.h Utility functions used throughout %Grantlee.

namespace Grantlee
{

/**
  Returns whether the @p variant is evaluated to true.

  @see @ref truthiness
*/
GRANTLEE_TEMPLATES_EXPORT bool variantIsTrue(const QVariant &variant);

/**
  Converts the @p input into its unescaped form.

  Double slashes are converted to single slashes, \\&quot is converted to &quot
  etc.
*/
GRANTLEE_TEMPLATES_EXPORT QString unescapeStringLiteral(const QString &input);

/**
  Marks the @p input as safe.
*/
GRANTLEE_TEMPLATES_EXPORT Grantlee::SafeString
markSafe(const Grantlee::SafeString &input);

/**
  Marks the @p input as requiring escaping.
*/
GRANTLEE_TEMPLATES_EXPORT Grantlee::SafeString
markForEscaping(const Grantlee::SafeString &input);

/**
  Retrieves and returns a SafeString from the @p input.
*/
GRANTLEE_TEMPLATES_EXPORT Grantlee::SafeString
getSafeString(const QVariant &input);

/**
  Returns whether @p input contains a SafeString.
*/
GRANTLEE_TEMPLATES_EXPORT bool isSafeString(const QVariant &input);

/**
  Returns true if the type of @p input can be inserted into a rendered template
  directly.

  Note that lists, hashes and QObject*s can not be.
*/
GRANTLEE_TEMPLATES_EXPORT bool supportedOutputType(const QVariant &input);

/**
  Compares @p lhs and @p rhs for equality. SafeStrings are compared as raw
  QStrings. Their safeness is not part of the comparison.

  @see QVariant::operator==
*/
GRANTLEE_TEMPLATES_EXPORT bool equals(const QVariant &lhs, const QVariant &rhs);

/**
  Converts @p size into the nearest file size unit like MB or MiB, based on the
  @p unitSystem value. Use @c 2 for the @p unitSystem to get binary units, use
  @c 10 to get decimal units - by default, decimal units will be returned. The
  @p multiplier can be used if the input @p size is not in pure bytes. If @p
  size is for example given in @a KiB, use a multiplier of @a 1024. The returned
  pair will have the converted size as @a first and the unit as @a second.
*/
GRANTLEE_TEMPLATES_EXPORT std::pair<qreal, QString>
calcFileSize(qreal size, int unitSystem = 10, qreal multiplier = 1.0);

#ifndef Q_QDOC
/**
  @internal
  Returns the @p list as a formatted string. This is for debugging purposes
  only.
*/
GRANTLEE_TEMPLATES_EXPORT Grantlee::SafeString
toString(const QVariantList &list);
#endif
}

#endif
