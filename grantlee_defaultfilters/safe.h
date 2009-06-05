/*
Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef SAFEFILTER_H
#define SAFEFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT SafeFilter : public Filter
{
  Q_OBJECT
public:
  SafeFilter(QObject *parent = 0);

  bool isSafe() { return true; }

  Grantlee::SafeString doFilter(const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false) const;

};

#endif