/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef FILTER_H
#define FILTER_H

#include <QStringList>
#include <QVariant>

#include "grantlee_export.h"

namespace Grantlee
{

/**
Abstract base class for filters.
*/
class GRANTLEE_EXPORT Filter : public QObject
{
  Q_OBJECT
public:
  Filter(QObject *parent = 0);
  virtual ~Filter();

  /**
   * Reimplement to filter @p input.
   * @returns The input string filtered.
   */
  virtual QString doFilter(const QVariant &input, const QString &argument = QString()) = 0;

signals:
  void error(int, const QString &);

};

}

#endif

