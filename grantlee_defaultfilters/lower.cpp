
#include "lower.h"

LowerFilter::LowerFilter(QObject* parent): Filter(parent)
{

}

QString LowerFilter::doFilter(const QVariant& input, const QString &argument) const
{
  return input.toString().toLower();
}

#include "lower.moc"