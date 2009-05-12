
#ifndef UPPERFILTER_H
#define UPPERFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT UpperFilter : public Filter
{
  Q_OBJECT
public:
  UpperFilter(QObject *parent = 0);

  QString doFilter(const QVariant &input, const QString &argument = QString());

};

#endif
