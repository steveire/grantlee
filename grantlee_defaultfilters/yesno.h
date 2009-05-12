
#ifndef YESNOFILTER_H
#define YESNOFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT YesNoFilter : public Filter
{
  Q_OBJECT
public:
  YesNoFilter(QObject *parent = 0);

  QString doFilter(const QVariant &input, const QString &argument = QString());

};

#endif
