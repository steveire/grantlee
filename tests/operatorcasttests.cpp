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

#include <iostream>

class QString {
public:
  void operation1()
  {
    std::cout << "QString operation 1" << std::endl;
  }
  void operation2()
  {
    std::cout << "QString operation 2" << std::endl;
  }
};

class QVariant
{
public:
  QVariant() {}
  QVariant(const QString &) {
    std::cout << "QVariant::QVariant(QString)" << std::endl;
  }
};

class SafeString : public QString
{
public:
  operator QVariant() const {
    std::cout << "SafeString::operator QVariant()" << std::endl;
    return QVariant();
  }
};

class WrappingSafeString
{
public:
  WrappingSafeString() {}

  WrappingSafeString(const QString &) {
    std::cout << "WrappingSafeString::WrappingSafeString(QString)" << std::endl;
  }

  operator QVariant() const {
    std::cout << "WrappingSafeString::operator QVariant()" << std::endl;
    return QVariant();
  }

  operator QString() const {
    std::cout << "WrappingSafeString::operator QString()" << std::endl;
    return QString();
  }
};

class WrappingSubclassSafeString
{
public:
  WrappingSubclassSafeString() : m_isSafe(false), m_wrappedSubclass(this) {}

  WrappingSubclassSafeString(const QString &)  : m_isSafe(false), m_wrappedSubclass(this) {
    std::cout << "WrappingSubclassSafeString::WrappingSubclassSafeString(QString)" << std::endl;
  }

  bool isSafe() const { return m_isSafe; };
  void setSafe(bool safe) { m_isSafe = safe; }

  class Subclass : public QString
  {
    friend class WrappingSubclassSafeString;
    Subclass(WrappingSubclassSafeString *wsss) : m_wsss(wsss) {}
    WrappingSubclassSafeString *m_wsss;

  public:
    void operation2()
    {
      m_wsss->m_isSafe = false;
      std::cout << "overridden string operation 2 (wrapping)" << std::endl;
    }
  };

  Subclass m_wrappedSubclass;

  Subclass* operator->()
  {
    return &m_wrappedSubclass;
  }

  operator QVariant() const {
    std::cout << "WrappingSubclassSafeString::operator QVariant()" << std::endl;
    return QVariant();
  }

  operator QString() const {
    std::cout << "WrappingSubclassSafeString::operator QString()" << std::endl;
    return m_wrappedSubclass;
  }

private:
  bool m_isSafe;
};

class UsingSafeString : private QString
{
public:
  UsingSafeString() {}
  UsingSafeString(const QString &) {}

  void operation2() {
    std::cout << "overridden string operation 2 (using)" << std::endl;
  };

  using QString::operation1;

  operator QVariant()
  {
    return QVariant();
  }

  operator QString()
  {
    return QString();
  }
};

QVariant f1() {
  return QString();
}

QVariant f2() {
  return SafeString();
}

QVariant f3() {
  return WrappingSafeString();
}

QString f4() {
  return WrappingSafeString();
}

WrappingSafeString f5() {
  return QString();
}

QVariant f6() {
  return WrappingSubclassSafeString();
}

QString f7() {
  return WrappingSubclassSafeString();
}

WrappingSubclassSafeString f8() {
  return QString();
}

#ifdef BROKEN

QVariant f9() {
  return UsingSafeString();
}

QString f10() {
  return UsingSafeString();
}

UsingSafeString f11() {
  return QString();
}

#endif

int main(int argc, char ** argv)
{
  f1();
  f2();
  f3();
  f4();
  f5();
  f6();
  f7();
  f8();
#ifdef BROKEN
  f9();
  f10();
  f11();
#endif

  WrappingSubclassSafeString wsss;
  wsss.setSafe(true);
  wsss->operation1();
  std::cout << ( wsss.isSafe() ? "IsSafe" : "IsNotSafe" ) << std::endl;
  wsss->operation2();
  std::cout << ( wsss.isSafe() ? "IsSafe" : "IsNotSafe" ) << std::endl;

  return 0;
}

