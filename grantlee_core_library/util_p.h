
#ifndef UTIL_H
#define UTIL_H

#include <QVariant>

class Util
{
public:
  Util();

  static bool variantIsTrue(const QVariant &variant);

  static QString unescapeStringLiteral(const QString &input);

};

#endif

