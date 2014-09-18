/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010,2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef LISTFILTERS_H
#define LISTFILTERS_H

//krazy:excludeall=includes

#include "filter.h"

using namespace Grantlee;

class JoinFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class LengthFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class LengthIsFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class FirstFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class LastFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class RandomFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class SliceFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class MakeListFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class UnorderedListFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
protected:
  SafeString processList( const QVariantList &list, int tabs, bool autoescape ) const;

};

class DictSortFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return false;
  }
};

#endif
