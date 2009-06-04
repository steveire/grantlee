/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef UPPERFILTER_H
#define UPPERFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT UpperFilter : public Filter
{
  Q_OBJECT
public:
  UpperFilter(QObject *parent = 0);

  // &amp; may be safe, but it will be changed to &AMP; which is not safe.
  bool isSafe() { return false; }

  Grantlee::SafeString doFilter(const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false) const;

};

#endif
