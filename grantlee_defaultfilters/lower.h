/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef LOWERFILTER_H
#define LOWERFILTER_H

#include "filter.h"

using namespace Grantlee;

class LowerFilter : public Filter
{
  Q_OBJECT
public:
  LowerFilter(QObject *parent = 0);

  QString doFilter(const QVariant &input, const QString &argument = QString()) const;

};

#endif
