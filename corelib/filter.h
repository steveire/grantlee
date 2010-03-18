/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

// krazy:excludeall=dpointer

#ifndef GRANTLEE_FILTER_H
#define GRANTLEE_FILTER_H

#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include "outputstream.h"
#include "safestring.h"

#include "grantlee_core_export.h"

namespace Grantlee
{

/**
  @brief Base class for all filters

  The Filter class can be implemented in plugin libraries to make additional functionality available to templates.

*/
class GRANTLEE_CORE_EXPORT Filter
{
public:
  typedef QSharedPointer<Filter> Ptr;

  /**
    Destructor.
  */
  virtual ~Filter() {}

  void setStream( OutputStream *stream ) {
    m_stream = stream;
  }

  SafeString escape( const QString &input ) const {
    return m_stream->escape( input );
  }

  SafeString escape( const SafeString &input ) const {
    if ( input.isSafe() )
      return SafeString( m_stream->escape( input ), SafeString::IsSafe );
    return m_stream->escape( input );
  }

  SafeString conditionalEscape( const SafeString &input ) const {
    if ( !input.isSafe() )
      return m_stream->escape( input );
    return input;
  }

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
  virtual bool isSafe() const { // krazy:exclude:inline
    return false;
  }
private:
  OutputStream *m_stream;
};

}

#endif

