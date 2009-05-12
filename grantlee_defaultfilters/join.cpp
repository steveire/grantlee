
#include "join.h"

JoinFilter::JoinFilter(QObject* parent): Filter(parent)
{

}

QString JoinFilter::doFilter(const QVariant& input, const QString &argument)
{
  QVariantList varList = input.toList();
  QListIterator<QVariant> i(varList);
  QString ret;
  while (i.hasNext())
  {
    ret.append(i.next().toString());
    if (i.hasNext())
      ret.append(argument);
  }
  return ret;
}

#include "join.moc"

