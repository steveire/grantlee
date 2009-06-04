/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "safestring.h"

using namespace Grantlee;

SafeString::SafeString()
  : m_safety(false), m_needsescape(false)
{

}

SafeString::SafeString(const QString &str, Safety safety)
  : m_string(str), m_safety(safety==IsSafe), m_needsescape(false)
{

}

SafeString::~SafeString()
{
}

void SafeString::setNeedsEscape(bool needsEscape)
{
  m_needsescape = true;
}

bool SafeString::needsEscape() const
{
  return m_needsescape;
}

void SafeString::setSafety(Grantlee::SafeString::Safety safety)
{
  m_safety = (safety == IsSafe);
}

bool SafeString::isSafe() const
{
  return m_safety;
}

QString SafeString::rawString() const
{
  return m_string;
}

SafeString SafeString::operator+(const QString &str)
{
  return SafeString(m_string + str, IsNotSafe);
}

SafeString SafeString::operator+(const SafeString &str)
{
  if (!str.isSafe())
    return SafeString(m_string + str.rawString(), IsNotSafe);
  return SafeString(m_string + str.rawString(), IsSafe);
}

SafeString &SafeString::operator+=(const QString &str)
{
  m_safety = IsNotSafe;

  m_string += str;
  return *this;
}

SafeString &SafeString::operator+=(const SafeString &str)
{
  if (!str.isSafe() )
    m_safety = IsNotSafe;

  m_string += str.rawString();
  return *this;
}

SafeString &SafeString::operator=(const QString &str)
{
  m_safety = IsNotSafe;

  m_string = str;
  return *this;
}

bool SafeString::operator==(const Grantlee::SafeString &other)
{
  // Don't compare safety or account for future escaping here.
  // See TestBuiltins testEscaping
  return (other.m_string == m_string);
}

SafeString::operator QString() const
{
  return m_string;
}

