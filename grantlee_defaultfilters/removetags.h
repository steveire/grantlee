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

  QString doFilter(const QVariant &input, const QString &argument = QString()) const;

};

#endif
