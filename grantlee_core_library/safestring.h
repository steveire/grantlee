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

#ifndef GRANTLEE_STRING_H
#define GRANTLEE_STRING_H

#include <QString>
#include <QVariant>

namespace Grantlee
{

/**
  @brief A QString wrapper class for containing whether a string is safe or needs to be escaped.

  This allows lazy escaping of strings. Otherwise a string may be escaped multiple times where it
  should only be escaped once.
*/
class SafeString : public QString
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
  /* implicit */ SafeString( const QString &str, Safety safety = IsNotSafe );

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

    Safeness and needing escaping are not accounted for in the comparision.
  */
  bool operator==( const SafeString &other ) const;

  /**
    Returns true if the content of @p other matches the content of this.

    Safeness and needing escaping are not accounted for in the comparision.
  */
  bool operator==( const QString &other ) const;

  /**
    Assignment operator. The resulting SafeString is not safe.
  */
  SafeString &operator=( const QString &s );

  operator QVariant()
  {
    return QVariant::fromValue( *this );
  }

private:
  QString m_string;
  bool m_safety;
  bool m_needsescape;

};

}

Q_DECLARE_METATYPE( Grantlee::SafeString )

#endif
