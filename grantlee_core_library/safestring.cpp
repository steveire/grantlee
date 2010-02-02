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

#include "safestring.h"

using namespace Grantlee;

SafeString::SafeString()
    : m_nestedString( this ), m_safety( false ), m_needsescape( false )
{

}

SafeString::SafeString( const QString &str, bool safe )
    : m_nestedString( str, this ), m_safety( safe ), m_needsescape( false )
{

}

SafeString::SafeString( const QString &str, Safety safety )
    : m_nestedString( str, this ), m_safety( safety == IsSafe ), m_needsescape( false )
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
  m_safety = ( safety == IsSafe );
}

bool SafeString::isSafe() const
{
  return m_safety;
}

SafeString::NestedString::NestedString(SafeString* safeString)
  : m_safeString(safeString)
{

}

SafeString::NestedString::NestedString(const QString& content, SafeString* safeString)
  : QString(content),
    m_safeString(safeString)
{

}

SafeString SafeString::operator+( const QString &str )
{
  return SafeString( m_nestedString + str, IsNotSafe );
}

SafeString SafeString::operator+( const SafeString &str )
{
  if ( !str.isSafe() )
    return SafeString( m_nestedString + (*str), IsNotSafe );
  return SafeString( m_nestedString + *str, m_safety );
}

SafeString &SafeString::operator+=( const QString & str )
{
  m_safety = IsNotSafe;

  m_nestedString += str;
  return *this;
}

SafeString &SafeString::operator+=( const SafeString & str )
{
  if ( !str.isSafe() )
    m_safety = IsNotSafe;

  m_nestedString += *str;
  return *this;
}

bool SafeString::operator==( const Grantlee::SafeString &other ) const
{
  // Don't compare safety or account for future escaping here.
  // See TestBuiltins testEscaping
  return m_nestedString == *other;
}

bool SafeString::operator==( const QString &other ) const
{
  return m_nestedString == other;
}
