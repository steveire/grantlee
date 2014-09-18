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

#ifndef GRANTLEE_SAFESTRING_H
#define GRANTLEE_SAFESTRING_H

#include "grantlee_core_export.h"

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace Grantlee
{

/// @headerfile safestring.h grantlee/safestring.h

/**
  @brief A QString wrapper class for containing whether a string is safe or needs to be escaped.

  This allows lazy escaping of strings. Otherwise a string may be escaped multiple times where it
  should only be escaped once.

  The SafeString class itself provides information about whether a string is safe from further escaping through the isSafe method. The actual string content held by the SafeString instance is available through the get() method. The get() method returns a QString subclass which should be used like any other QString. The difference is that all methods on NestedString return a SafeString instead of a QString.

  @code
    SafeString s("this & that", SafeString::IsSafe);
    s.get().replace( "this", "these" ).toUpper();

    qDebug() << s.get() << s.isSafe(); // outputs "these & that" false
  @endcode

  Note that most operations on strings make the string unsafe. For example, while <tt>"K &amp; R"</tt> is safe, using replace("m", "n") will result in <tt>"K &anp; R"</tt>, which is unsafe. Likewise using upper() will return <tt>"K &AMP; R"</tt>, which is unsafe. Because the SafeString can not determine whether a method call with particular arguments will change a SafeString from being safe to being unsafe, any operation which can possibly make the string unsafe does cause the string to become unsafe. It is then up to the caller to restore safe-ness if needed.

  NestedString has overloads for SafeStrings whereever appropriate so that strings remain marked as safe where possible.

  For example:

  @code
    SafeString s1("this & that", SafeString::IsSafe);
    s2 = s1;
    s1.append( QString( " & the other" ) );
    // s1 is now "this & that & the other" and is unsafe.

    SafeString s3(" Wobl & Bob", SafeString::IsSafe);
    s2.append(s3);
    // Both s2 and s3 are safe, and append is a safe operation, so s2 is still safe
  @endcode

  @see @ref autoescaping
  @see OutputStream::escape

  SafeString has appropriate operator overloads to make it convenient to use in methods returning a QVariant, such as Filter::doFilter, or as a QString. Note that a raw QString is essentially the same as a SafeString which is marked as unsafe.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT SafeString
{
public:
  /**
    Possible safety states of a SafeString
  */
  enum Safety {
    IsSafe,    ///< The string is safe and requires no further escaping
    IsNotSafe  ///< The string is not safe. It will be escaped before being added to the output of rendering.
  };

  /**
    Constructs an empty SafeString.
  */
  SafeString();

  /**
    Copy constructor
  */
  SafeString( const SafeString &safeString );

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

#ifndef Q_QDOC
  /**
    Set whether the string should be escaped.
  */
  void setNeedsEscape( bool needsEscape );
#endif

  /**
    Whether the string needs to be escaped.
  */
  bool needsEscape() const;

  /**
    Whether the string is safe.
  */
  bool isSafe() const;

#ifndef Q_QDOC
  /**
    Set whether the string is safe.
  */
  void setSafety( Safety safety );
#endif

  /**
    @brief The NestedString is a QString whose methods always return a SafeString

    This class is largely an implementation detail. See the SafeString documentation for details.
  */
  class GRANTLEE_CORE_EXPORT NestedString : public QString
  {
#ifndef Q_QDOC
    friend class SafeString;
    SafeString *m_safeString;
  public:
    NestedString( SafeString *safeString );
    NestedString( const QString &content, SafeString *safeString );

    SafeString& append( const SafeString &str );
    SafeString& append( const QString &str );
    SafeString& append( const QStringRef &reference );
    SafeString& append( const QLatin1String &str );
    // BIC: Use QT_NO_CAST_FROM_ASCII instead.
#ifndef GRANTLEE_NO_CAST_FROM_ASCII
    SafeString& append( const QByteArray &ba );
    SafeString& append( const char *str );
#endif
    SafeString& append( const QChar ch );

    SafeString& fill( QChar ch, int size = -1 );

    SafeString& insert( int position, const SafeString &str );
    SafeString& insert( int position, const QString &str );
    SafeString& insert( int position, const QLatin1String &str );
    SafeString& insert( int position, const QChar *unicode, int size );
    SafeString& insert( int position, QChar ch );

    SafeString left( int n ) const;
    SafeString leftJustified( int width, QChar fill = QLatin1Char( ' ' ), bool truncate = false ) const;
    SafeString mid( int position, int n = -1 ) const;

    SafeString normalized( NormalizationForm mode ) const;
    SafeString normalized( NormalizationForm mode, QChar::UnicodeVersion version ) const;

    SafeString& prepend( const SafeString &str );
    SafeString& prepend( const QString &str );
    SafeString& prepend( const QLatin1String &str );
#ifndef GRANTLEE_NO_CAST_FROM_ASCII
    SafeString& prepend( const QByteArray &ba );
    SafeString& prepend( const char *str );
#endif
    SafeString& prepend( QChar ch );

    void push_back( const SafeString& other );
    void push_front( const SafeString& other );

    SafeString& remove( int position, int n );
    SafeString& remove( QChar ch, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& remove( const SafeString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& remove( const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& remove( const QRegExp &rx );
    SafeString repeated( int times ) const;
    SafeString& replace( int position, int n, const SafeString &after );
    SafeString& replace( int position, int n, const QString &after );
    SafeString& replace( int position, int n, const QChar *unicode, int size );
    SafeString& replace( int position, int n, QChar after );
    SafeString& replace( const SafeString &before, const SafeString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QString &before, const SafeString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const SafeString &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QString &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QChar *before, int blen, const QChar *after, int alen, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( QChar ch, const SafeString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( QChar ch, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( QChar before, QChar after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QLatin1String &before, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QLatin1String &before, const SafeString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QLatin1String &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const SafeString &before, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QString &before, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( QChar c, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive );
    SafeString& replace( const QRegExp &rx, const SafeString &after );
    SafeString& replace( const QRegExp &rx, const QString &after );

    SafeString right( int n ) const;
    SafeString rightJustified( int width, QChar fill = QLatin1Char( ' ' ), bool truncate = false ) const;

    SafeString section( QChar sep, int start, int end = -1, SectionFlags flags = SectionDefault ) const;
    SafeString section( const SafeString& sep, int start, int end = -1, SectionFlags flags = SectionDefault ) const;
    SafeString section( const QString& sep, int start, int end = -1, SectionFlags flags = SectionDefault ) const;
    SafeString section( const QRegExp& reg, int start, int end = -1, SectionFlags flags = SectionDefault ) const;
    SafeString& setNum( int n, int base = 10 );
    SafeString& setNum( uint n, int base = 10 );
    SafeString& setNum( long n, int base = 10 );
    SafeString& setNum( ulong n, int base = 10 );
    SafeString& setNum( qlonglong n, int base = 10 );
    SafeString& setNum( qulonglong n, int base = 10 );
    SafeString& setNum( short n, int base = 10 );
    SafeString& setNum( ushort n, int base = 10 );
    SafeString& setNum( double n, char format = 'g', int precision = 6 );
    SafeString& setNum( float n, char format = 'g', int precision = 6 );
    SafeString& setUnicode( const QChar * unicode, int size );
    SafeString& setUtf16( const ushort * unicode, int size );
    SafeString simplified() const;

    QStringList split( const SafeString& sep, SplitBehavior behavior = KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const;
    QStringList split( const QString& sep, SplitBehavior behavior = KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const;
    QStringList split( const QChar & sep, SplitBehavior behavior = KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const;
    QStringList split( const QRegExp & rx, SplitBehavior behavior = KeepEmptyParts ) const;

    SafeString toLower() const;
    SafeString toUpper() const;
    SafeString trimmed() const;

    void chop( int n );
#endif
  };

  /**
    Returns the String held by this SafeString
  */
  const NestedString& get() const {
    return m_nestedString;
  }

  /**
    Returns the String held by this SafeString
  */
  NestedString& get() {
    return m_nestedString;
  }

  /**
    Convenience operator for treating a SafeString like a QString.
  */
  operator QString() const {
    return m_nestedString;
  }

  /**
    Assignment operator.
  */
  SafeString &operator=( const SafeString &str );

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

  /**
    Convenience operator for storing a SafeString in a QVariant.
  */
  operator QVariant() const {
    return QVariant::fromValue( *this );
  }

private:
#ifndef Q_QDOC
  NestedString m_nestedString;
#endif
  Safety m_safety;
  bool m_needsescape;
};

}

Q_DECLARE_METATYPE( Grantlee::SafeString )

#endif
