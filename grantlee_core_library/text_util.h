
#ifndef TEXTUTIL_H
#define TEXTUTIL_H

#include <QStringList>

namespace Grantlee
{

class TextUtil
{
public:
  static QStringList smartSplit(const QString &str)
  {
    QRegExp r("(\"(?:[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*)\"|\\\'(?:[^\\\'\\\\]*(?:\\\\.[^\\\'\\\\]*)*)\\\'|[^\\s]+)");

    QStringList l;
    int count = 0;
    int pos = 0;
    while ((pos = r.indexIn(str, pos)) != -1) {
        ++count;
        pos += r.matchedLength();
        l << r.capturedTexts().at(0);
    }

    return l;
  }

};

}

#endif
