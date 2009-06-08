/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

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
  Filter( QObject *parent = 0 );
  virtual ~Filter();

  /**
   * Reimplement to filter @p input.
   * @returns The input string filtered.
   */
  virtual SafeString doFilter( const QVariant &input,
                               const SafeString &argument = SafeString(),
                               bool autoescape = false
                             ) const = 0;


  virtual bool isSafe() const {
    return false;
  }
  virtual bool needsAutoescape() const {
    return false;
  }

};

}

#endif

