

#include "truncatewords.h"
#include <QVariant>

TruncateWordsFilter::TruncateWordsFilter(QObject* parent): Filter(parent)
{

}

QString TruncateWordsFilter::doFilter(const QVariant& input, const QString &argument) const
{
  QVariant v(argument);
  if (!v.convert(QVariant::Int))
  {
    return input.toString();
  }
  int numWords = v.toInt();
  QStringList words = input.toString().split(" ", QString::SkipEmptyParts);

  if (words.size() > numWords)
  {
    words = words.mid(0, numWords);
    if (!words.at(words.size() - 1).endsWith("..."))
    {
      words << "...";
    }
  }
  return words.join(" ");

}

#include "truncatewords.moc"
