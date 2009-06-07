/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TRUNCATEWORDSFILTER_H
#define TRUNCATEWORDSFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT TruncateWordsFilter : public Filter
{
  Q_OBJECT
public:
  TruncateWordsFilter( QObject *parent = 0 );

  bool isSafe() const {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

#endif
