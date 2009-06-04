/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/


#include "truncatewords.h"
#include <QVariant>

TruncateWordsFilter::TruncateWordsFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString TruncateWordsFilter::doFilter(const QVariant& input, const Grantlee::SafeString& argument, bool autoescape) const
{
  QVariant v(argument);
  if (!v.convert(QVariant::Int))
  {
    return input.toString();
  }
  int numWords = v.toInt();

  QString inputString = Util::getSafeString(input).rawString();
  QStringList words = inputString.split(" ", QString::SkipEmptyParts);

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
