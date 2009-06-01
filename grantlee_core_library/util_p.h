
#ifndef UTIL_H
#define UTIL_H

#include <QVariant>

class Util
{
public:
  Util();

  static bool variantIsTrue(const QVariant &variant);

  // TODO: Make this return an iterator instead.
  static QVariantList variantToList(const QVariant &variant);

  static QString unescapeStringLiteral(const QString &input);

};

#endif

