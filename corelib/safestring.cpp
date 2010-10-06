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

#include "safestring.h"

#include <QtCore/QStringList>

using namespace Grantlee;

SafeString::SafeString()
  : m_nestedString( this ),
    m_safety( IsNotSafe ),
    m_needsescape( false )
{

}

SafeString::SafeString( const Grantlee::SafeString& safeString )
  : m_nestedString( safeString.get(), this ),
    m_safety( safeString.m_safety ),
    m_needsescape( safeString.m_needsescape )
{

}

SafeString::SafeString( const QString &str, bool safe )
  : m_nestedString( str, this ),
    m_safety( safe ? IsSafe : IsNotSafe ),
    m_needsescape( false )
{

}

SafeString::SafeString( const QString &str, Safety safety )
  : m_nestedString( str, this ),
    m_safety( safety ),
    m_needsescape( false )
{

}

SafeString::~SafeString()
{
}

void SafeString::setNeedsEscape( bool needsEscape )
{
  m_needsescape = needsEscape;
}

bool SafeString::needsEscape() const
{
  return m_needsescape;
}

void SafeString::setSafety( Grantlee::SafeString::Safety safety )
{
  m_safety = safety;
}

bool SafeString::isSafe() const
{
  return m_safety == IsSafe;
}

SafeString::NestedString::NestedString( SafeString *safeString )
    : m_safeString( safeString )
{

}

SafeString::NestedString::NestedString( const QString &content, SafeString *safeString )
  : QString( content ),
    m_safeString( safeString )
{

}

SafeString& SafeString::operator=( const SafeString &str )
{
  m_nestedString = str.m_nestedString;
  m_nestedString.m_safeString = this;
  m_safety = str.m_safety;
  m_needsescape = str.m_needsescape;
  return *this;
}

SafeString SafeString::operator+( const QString &str )
{
  return SafeString( m_nestedString + str, IsNotSafe );
}

SafeString SafeString::operator+( const SafeString &str )
{
  if ( !str.isSafe() )
    return SafeString( m_nestedString + str.get(), IsNotSafe );
  return SafeString( m_nestedString + str.get(), m_safety );
}

SafeString &SafeString::operator+=( const QString &str )
{
  m_safety = IsNotSafe;

  m_nestedString += str;
  return *this;
}

SafeString &SafeString::operator+=( const SafeString &str )
{
  if ( !str.isSafe() )
    m_safety = IsNotSafe;

  m_nestedString += str.get();
  return *this;
}

bool SafeString::operator==( const Grantlee::SafeString &other ) const
{
  // Don't compare safety or account for future escaping here.
  // See TestBuiltins testEscaping
  return m_nestedString == other.get();
}

bool SafeString::operator==( const QString &other ) const
{
  return m_nestedString == other;
}

SafeString& SafeString::NestedString::append( const SafeString &str )
{
  QString::append( str.get() );
  if ( !str.isSafe() )
    m_safeString->m_safety = IsNotSafe;

  return *m_safeString;
}

SafeString& SafeString::NestedString::append( const QString &str )
{
  QString::append( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::append( const QStringRef &reference )
{
  QString::append( reference );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::append( const QLatin1String &str )
{
  QString::append( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

#ifndef GRANTLEE_NO_CAST_FROM_ASCII
SafeString& SafeString::NestedString::append( const QByteArray &ba )
{
  QString::append( ba );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::append( const char *str )
{
  QString::append( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}
#endif

SafeString& SafeString::NestedString::append( const QChar ch )
{
  QString::append( ch );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

void SafeString::NestedString::chop( int n )
{
  QString::chop( n );
  m_safeString->m_safety = IsNotSafe;
}

SafeString& SafeString::NestedString::fill( QChar ch, int size )
{
  QString::fill( ch, size );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::insert( int position, const Grantlee::SafeString &str )
{
  QString::insert( position, str.get() );
  if ( !str.isSafe() )
    m_safeString->m_safety = IsNotSafe;

  return *m_safeString;
}

SafeString& SafeString::NestedString::insert( int position, const QString &str )
{
  QString::insert( position, str );
  return *m_safeString;
}

SafeString& SafeString::NestedString::insert( int position, const QLatin1String &str )
{
  QString::insert( position, str );
  return *m_safeString;
}

SafeString& SafeString::NestedString::insert( int position, const QChar *unicode, int size )
{
  QString::insert( position, unicode, size );
  return *m_safeString;
}

SafeString& SafeString::NestedString::insert( int position, QChar ch )
{
  QString::insert( position, ch );
  return *m_safeString;
}

SafeString SafeString::NestedString::left( int n ) const
{
  return SafeString( QString::left( n ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::leftJustified( int width, QChar fill, bool truncate ) const
{
  return SafeString( QString::leftJustified( width, fill, truncate ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::mid( int position, int n ) const
{
  return SafeString( QString::mid( position, n ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::normalized( QString::NormalizationForm mode ) const
{
  return SafeString( QString::normalized( mode ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::normalized( QString::NormalizationForm mode, QChar::UnicodeVersion version ) const
{
  return SafeString( QString::normalized( mode, version ), m_safeString->m_safety );
}

SafeString& SafeString::NestedString::prepend( const Grantlee::SafeString &str )
{
  QString::prepend( str.get() );
  if ( !str.isSafe() )
    m_safeString->m_safety = IsNotSafe;

  return *m_safeString;
}

SafeString& SafeString::NestedString::prepend( const QString &str )
{
  QString::prepend( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::prepend( const QLatin1String &str )
{
  QString::prepend( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

#ifndef GRANTLEE_NO_CAST_FROM_ASCII
SafeString& SafeString::NestedString::prepend( const QByteArray &ba )
{
  QString::prepend( ba );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::prepend( const char *str )
{
  QString::prepend( str );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}
#endif

SafeString& SafeString::NestedString::prepend( QChar ch )
{
  QString::prepend( ch );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

void SafeString::NestedString::push_back( const Grantlee::SafeString &other )
{
  QString::push_back( other.get() );
  m_safeString->m_safety = other.m_safety;
}

void SafeString::NestedString::push_front( const Grantlee::SafeString &other )
{
  QString::push_front( other.get() );
  m_safeString->m_safety = other.m_safety;
}

SafeString& SafeString::NestedString::remove( int position, int n )
{
  QString::remove( position, n );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::remove( QChar ch, Qt::CaseSensitivity cs )
{
  QString::remove( ch, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::remove( const Grantlee::SafeString &str, Qt::CaseSensitivity cs )
{
  QString::remove( str, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::remove( const QString &str, Qt::CaseSensitivity cs )
{
  QString::remove( str, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::remove( const QRegExp &rx )
{
  QString::remove( rx );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString SafeString::NestedString::repeated( int times ) const
{
  return SafeString( QString::repeated( times ), m_safeString->m_safety );
}

SafeString& SafeString::NestedString::replace( int position, int n, const Grantlee::SafeString &after )
{
  QString::replace( position, n, after.get() );
  m_safeString->m_safety = after.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( int position, int n, const QString &after )
{
  QString::replace( position, n, after );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( int position, int n, const QChar *unicode, int size )
{
  QString::replace( position, n, unicode, size );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( int position, int n, QChar after )
{
  QString::replace( position, n, after );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const Grantlee::SafeString &before, const Grantlee::SafeString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before.get(), after.get(), cs );
  m_safeString->m_safety = after.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QString &before, const Grantlee::SafeString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after.get(), cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const Grantlee::SafeString &before, const QString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before.get(), after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QString &before, const QString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QChar *before, int blen, const QChar *after, int alen, Qt::CaseSensitivity cs )
{
  QString::replace( before, blen, after, alen, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( QChar ch, const SafeString &after, Qt::CaseSensitivity cs )
{
  QString::replace( ch, after.get(), cs );
  m_safeString->m_safety = after.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( QChar ch, const QString &after, Qt::CaseSensitivity cs )
{
  QString::replace( ch, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( QChar before, QChar after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QLatin1String &before, const QLatin1String &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QLatin1String &before, const Grantlee::SafeString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after.get(), cs );
  m_safeString->m_safety = after.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QLatin1String &before, const QString &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const Grantlee::SafeString &before, const QLatin1String &after, Qt::CaseSensitivity cs )
{
  QString::replace( before.get(), after, cs );
  m_safeString->m_safety = before.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QString &before, const QLatin1String &after, Qt::CaseSensitivity cs )
{
  QString::replace( before, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( QChar c, const QLatin1String &after, Qt::CaseSensitivity cs )
{
  QString::replace( c, after, cs );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QRegExp &rx, const Grantlee::SafeString &after )
{
  QString::replace( rx, after.get() );
  m_safeString->m_safety = after.m_safety;
  return *m_safeString;
}

SafeString& SafeString::NestedString::replace( const QRegExp &rx, const QString &after )
{
  QString::replace( rx, after );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString SafeString::NestedString::right( int n ) const
{
  return SafeString( QString::right( n ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::rightJustified( int width, QChar fill, bool truncate ) const
{
  return SafeString( QString::rightJustified( width, fill, truncate ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::section( const QRegExp &reg, int start, int end, QString::SectionFlags flags ) const
{
  return SafeString( QString::section( reg, start, end, flags ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::section( QChar sep, int start, int end, QString::SectionFlags flags ) const
{
  return SafeString( QString::section( sep, start, end, flags ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::section( const Grantlee::SafeString &sep, int start, int end, QString::SectionFlags flags ) const
{
  return SafeString( QString::section( sep, start, end, flags ), m_safeString->m_safety );
}

SafeString SafeString::NestedString::section( const QString &sep, int start, int end, QString::SectionFlags flags ) const
{
  return SafeString( QString::section( sep, start, end, flags ), m_safeString->m_safety );
}

SafeString& SafeString::NestedString::setNum( int n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( uint n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( long int n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( ulong n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( qlonglong n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( qulonglong n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( short int n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( ushort n, int base )
{
  QString::setNum( n, base );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( double n, char format, int precision )
{
  QString::setNum( n, format, precision );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setNum( float n, char format, int precision )
{
  QString::setNum( n, format, precision );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setUnicode( const QChar *unicode, int size )
{
  QString::setUnicode( unicode, size );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString& SafeString::NestedString::setUtf16( const ushort *unicode, int size )
{
  QString::setUtf16( unicode, size );
  m_safeString->m_safety = IsNotSafe;
  return *m_safeString;
}

SafeString SafeString::NestedString::simplified() const
{
  return SafeString( QString::simplified(), m_safeString->m_safety );
}

QStringList SafeString::NestedString::split( const Grantlee::SafeString &sep, QString::SplitBehavior behavior, Qt::CaseSensitivity cs ) const
{
  return QString::split( sep.get(), behavior, cs );
}

QStringList SafeString::NestedString::split( const QString &sep, QString::SplitBehavior behavior, Qt::CaseSensitivity cs ) const
{
  return QString::split( sep, behavior, cs );
}

QStringList SafeString::NestedString::split( const QChar &sep, QString::SplitBehavior behavior, Qt::CaseSensitivity cs ) const
{
  return QString::split( sep, behavior, cs );
}

QStringList SafeString::NestedString::split( const QRegExp &rx, QString::SplitBehavior behavior ) const
{
  return QString::split( rx, behavior );
}

SafeString SafeString::NestedString::toLower() const
{
  return SafeString( QString::toLower(), IsNotSafe );
}

SafeString SafeString::NestedString::toUpper() const
{
  return SafeString( QString::toUpper(), IsNotSafe );
}

SafeString SafeString::NestedString::trimmed() const
{
  return SafeString( QString::trimmed(), m_safeString->m_safety );
}
