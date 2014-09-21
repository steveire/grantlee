/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

// krazy:excludeall=dpointer

#ifndef GRANTLEE_FILTER_H
#define GRANTLEE_FILTER_H

#include "grantlee_templates_export.h"
#include "outputstream.h"
#include "safestring.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace Grantlee
{

/// @headerfile filter.h grantlee/filter.h

/**
  @brief Base class for all filters

  The Filter class can be implemented in plugin libraries to make additional functionality available to templates.

  Developers are required only to implement the doFilter method and integrate the filter as part of a custom plugin, but will never create or access filters directly in application code.

  The FilterExpression class is the access interface to a chain of Filter objects.

  The escape and conditionalEscape methods are available for escaping data where needed.

  @see @ref filters
*/
class GRANTLEE_TEMPLATES_EXPORT Filter
{
public:
#ifndef Q_QDOC
  typedef QSharedPointer<Filter> Ptr;
#endif

  /**
    Destructor.
  */
  virtual ~Filter();

#ifndef Q_QDOC
  /**
    FilterExpression makes it possible to access stream methods like escape while resolving.
  */
  void setStream( OutputStream *stream );
#endif

  /**
    Escapes and returns @p input. The OutputStream::escape method is used to escape @p input.
  */
  SafeString escape( const QString &input ) const;

  /**
    Escapes and returns @p input. The OutputStream::escape method is used to escape @p input.
  */
  SafeString escape( const SafeString &input ) const;

  /**
    Escapes @p input if not already safe from further escaping and returns it. The OutputStream::escape method is used to escape @p input.
  */
  SafeString conditionalEscape( const SafeString &input ) const;

  /**
    Reimplement to filter @p input given @p argument.
    @p autoescape determines whether the autoescape feature is currently on or off. Most filters will not use this.

    @see @ref autoescaping
  */
  virtual QVariant doFilter( const QVariant &input,
                             const QVariant &argument = QVariant(),
                             bool autoescape = false
                           ) const = 0;

  /**
    Reimplement to return whether this filter is safe.
  */
  virtual bool isSafe() const;

private:
#ifndef Q_QDOC
  OutputStream *m_stream;
#endif
};

}

#endif
