/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
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
class SafeString
{
public:
  enum Safety {
    IsSafe,
    IsNotSafe
  };
  SafeString();
  SafeString( const QString &str, Safety safety = IsNotSafe );
  ~SafeString();

  QString rawString() const;

  void setNeedsEscape( bool needsEscape );
  bool needsEscape() const;
  bool isSafe() const;
  void setSafety( Safety safety );

  SafeString operator+( const QString &str );
  SafeString operator+( const SafeString &str );
  SafeString &operator+=( const QString &str );
  SafeString &operator+=( const SafeString &str );

  bool operator==( const SafeString &other );

  operator QString() const;

  SafeString &operator=( const QString &s );

private:
  QString m_string;
  bool m_safety;
  bool m_needsescape;


};

}

Q_DECLARE_METATYPE( Grantlee::SafeString )

#endif

