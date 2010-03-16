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

#ifndef GRANTLEE_SAFESTRING_H
#define GRANTLEE_SAFESTRING_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "grantlee_core_export.h"

namespace Grantlee
{

/**
  @brief A QString wrapper class for containing whether a string is safe or needs to be escaped.

  This allows lazy escaping of strings. Otherwise a string may be escaped multiple times where it
  should only be escaped once.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT SafeString
{
public:
  enum Safety {
    IsSafe,    ///< The string is safe and requires no further escaping
    IsNotSafe  ///< The string is not safe. It will be escaped before being added to the output of rendering.
  };

  /**
    Constructs an empty SafeString.
  */
  SafeString();

  /**
    Constructs a SafeString with the content @p str whose safety is given by @p safe.
  */
  SafeString( const QString &str, bool safe );

  /**
    Constructs a SafeString with the content @p str whose safety is given by @p safety.
  */
  /* implicit */ SafeString( const QString &str, Safety safety = IsNotSafe ); // krazy:exclude=explicit

  /**
    Destructor
  */
  ~SafeString();

  /**
    Set whether the string should be escaped.
  */
  void setNeedsEscape( bool needsEscape );

  /**
    Whether the string needs to be escaped.
  */
  bool needsEscape() const;

  /**
    Whether the string is safe.
  */
  bool isSafe() const;

  /**
    Set whether the string is safe.
  */
  void setSafety( Safety safety );

  class GRANTLEE_CORE_EXPORT NestedString : public QString
  {
    friend class SafeString;
    SafeString *m_safeString;
  public:
    NestedString( SafeString *safeString );
    NestedString( const QString &content, SafeString *safeString );

    void chop( int n );
  };

  const NestedString& get() const
  {
    return m_nestedString;
  }

  NestedString& get()
  {
    return m_nestedString;
  }

  operator QString() const
  {
    return m_nestedString;
  }

  /**
    Returns a concatenation of this with @p str.

    The result is not safe because str is not safe.
  */
  SafeString operator+( const QString &str );

  /**
    Returns a concatenation of this with @p str.

    The result is safe if both this and str are safe.
  */
  SafeString operator+( const SafeString &str );

  /**
    Appends the content of @p str to this.

    The result is not safe because @p str is not safe.
  */
  SafeString &operator+=( const QString &str );

  /**
    Appends the content of @p str to this.

    The result is safe if both this and @p str are safe.
  */
  SafeString &operator+=( const SafeString &str );

  /**
    Returns true if the content of @p other matches the content of this.

    Safeness and needing escaping are not accounted for in the comparison.
  */
  bool operator==( const SafeString &other ) const;

  /**
    Returns true if the content of @p other matches the content of this.

    Safeness and needing escaping are not accounted for in the comparison.
  */
  bool operator==( const QString &other ) const;

  operator QVariant() const {
    return QVariant::fromValue( *this );
  }

private:
  NestedString m_nestedString;
  bool m_safety;
  bool m_needsescape;

};

}

Q_DECLARE_METATYPE( Grantlee::SafeString )

#endif

