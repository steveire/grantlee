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
 * @brief A QString wrapper class for containing whether a string is safe or needs to be escaped.
 *
 * This allows lazy escaping of strings. Otherwise a string may be escaped multiple times where it
 * should only be escaped once.
 */
class SafeString : public QString
{
public:
  enum Safety {
    IsSafe,
    IsNotSafe
  };
  SafeString();
  SafeString( const QString &str, bool safe );
  SafeString( const QString &str, Safety safety = IsNotSafe );
  ~SafeString();

  void setNeedsEscape( bool needsEscape );
  bool needsEscape() const;
  bool isSafe() const;
  void setSafety( Safety safety );

  SafeString operator+( const QString &str );
  SafeString operator+( const SafeString &str );
  SafeString &operator+=( const QString &str );
  SafeString &operator+=( const SafeString &str );

  bool operator==( const SafeString &other ) const;
  bool operator==( const QString &other ) const;

  SafeString &operator=( const QString &s );

  operator QVariant()
  {
    return QVariant::fromValue(*this);
  }

private:
  QString m_string;
  bool m_safety;
  bool m_needsescape;


};

}

Q_DECLARE_METATYPE( Grantlee::SafeString )

#endif

