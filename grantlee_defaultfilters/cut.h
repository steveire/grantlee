/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef CUTFILTER_H
#define CUTFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT CutFilter : public Filter
{
  Q_OBJECT
public:
  CutFilter(QObject *parent = 0);

  QString doFilter(const QVariant &input, const QString &argument = QString());

};

#endif
