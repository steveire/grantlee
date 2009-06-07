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
  LowerFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

#endif
