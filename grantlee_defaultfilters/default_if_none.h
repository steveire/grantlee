/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef DEFAULTIFNONEFILTER_H
#define DEFAULTIFNONEFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT DefaultIfNoneFilter : public Filter
{
  Q_OBJECT
public:
  DefaultIfNoneFilter(QObject *parent = 0);

  QString doFilter(const QVariant &input, const QString &argument = QString()) const;

};

#endif
