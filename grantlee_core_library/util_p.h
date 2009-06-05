
#ifndef UTIL_H
#define UTIL_H

#include <QVariant>

#include "safestring.h"
#include "context.h"

class Util
{
public:
  Util();

  static bool variantIsTrue(const QVariant &variant);

  // TODO: Make this return an iterator instead.
  static QVariantList variantToList(const QVariant &variant);

  static QString unescapeStringLiteral(const QString &input);

  static Grantlee::SafeString conditionalEscape(const Grantlee::SafeString &input);

  static Grantlee::SafeString markSafe(const Grantlee::SafeString &input);

  static Grantlee::SafeString markForEscaping(const Grantlee::SafeString& arg1);

  static Grantlee::SafeString escape(const Grantlee::SafeString &input);

  static Grantlee::SafeString getSafeString(const QVariant &input);

  static bool isSafeString(const QVariant &input);

  /**
  Returns true if the type of @p input can be inserted into a rendered template directly.
  Not that lists, hashes and QObject*s can not be.
  */
  static bool supportedOutputType(const QVariant &input);

  static bool equals(const QVariant &lhs, const QVariant &rhs);

};

#endif

