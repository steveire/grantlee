/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef SLICEFILTER_H
#define SLICEFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT SliceFilter : public Filter
{
  Q_OBJECT
public:
  SliceFilter(QObject *parent = 0);

  Grantlee::SafeString doFilter(const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false) const;

};

#endif
