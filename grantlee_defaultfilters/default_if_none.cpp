
#include "default_if_none.h"

DefaultIfNoneFilter::DefaultIfNoneFilter(QObject* parent): Filter(parent)
{

}

QString DefaultIfNoneFilter::doFilter(const QVariant& input, const QString &argument)
{
  if (!input.isValid())
    return argument;
  return input.toString();
}

#include "default_if_none.moc"
