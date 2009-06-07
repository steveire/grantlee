/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef JOINFILTER_H
#define JOINFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT JoinFilter : public Filter
{
  Q_OBJECT
public:
  JoinFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

  bool isSafe() {
    return true;
  }

};

#endif
