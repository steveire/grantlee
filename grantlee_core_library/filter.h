/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef FILTER_H
#define FILTER_H

#include <QStringList>
#include <QVariant>

#include "grantlee_export.h"
#include "safestring.h"
#include "util_p.h"

#include <QDebug>

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
  virtual Grantlee::SafeString doFilter(const QVariant &input,
                const Grantlee::SafeString &argument = Grantlee::SafeString(),
                bool autoescape = false
                ) const = 0;


  virtual bool isSafe() const { return false; }
  virtual bool needsAutoescape() const { return false; }

};

}

#endif

