/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef REMOVETAGSFILTER_H
#define REMOVETAGSFILTER_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT RemoveTagsFilter : public Filter
{
  Q_OBJECT
public:
  RemoveTagsFilter(QObject *parent = 0);

  Grantlee::SafeString doFilter(const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape= false) const;

};

#endif
