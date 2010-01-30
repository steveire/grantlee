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

#ifndef GRANTLEE_FILTER_H
#define GRANTLEE_FILTER_H

#include <QStringList>
#include <QVariant>
#include <QSharedPointer>

#include "grantlee_export.h"
#include "safestring.h"

#include <QDebug>

namespace Grantlee
{

/**
  @brief Base class for all filters

  The Filter class can be implemented in plugin libraries to make additional functionality available to templates.

*/
class GRANTLEE_EXPORT Filter
{
public:
  typedef QSharedPointer<Filter> Ptr;

  /**
    Destructor.
  */
  virtual ~Filter() {}

  /**
   Reimplement to filter @p input given @p argument.
   The autoescape argument amy also need to be taken into account, depending on the purpose of the filter.
   @returns The input string filtered.
  */
  virtual QVariant doFilter( const QVariant &input,
                               const QVariant &argument = QVariant(),
                               bool autoescape = false
                             ) const = 0;

  /**
    Reimplement to return whether this filter is safe.
  */
  virtual bool isSafe() const {
    return false;
  }

  /**
    Reimplement to return whether this filter requires an autoescape argument to its doFilter method.
  */
  virtual bool needsAutoescape() const {
    return false;
  }

};

}

#endif
