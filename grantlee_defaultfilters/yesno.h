/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef YESNOFILTER_H
#define YESNOFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT YesNoFilter : public Filter
{
  Q_OBJECT
public:
  YesNoFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

#endif
