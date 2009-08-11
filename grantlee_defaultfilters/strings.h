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

#ifndef STRINGFILTERS_H
#define STRINGFILTERS_H

#include "filter.h"

using namespace Grantlee;

class GRANTLEE_EXPORT AddSlashesFilter : public Filter
{
  Q_OBJECT
public:
  AddSlashesFilter( QObject *parent = 0 );

  virtual bool isSafe() const
  {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT CapFirstFilter : public Filter
{
  Q_OBJECT
public:
  CapFirstFilter( QObject *parent = 0 );

  virtual bool isSafe() const
  {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT EscapeJsFilter : public Filter
{
  Q_OBJECT
public:
  EscapeJsFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT FixAmpersandsFilter : public Filter
{
  Q_OBJECT
public:
  FixAmpersandsFilter( QObject *parent = 0 );

  virtual bool isSafe() const
  {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};


class GRANTLEE_EXPORT CutFilter : public Filter
{
  Q_OBJECT
public:
  CutFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT SafeFilter : public Filter
{
  Q_OBJECT
public:
  SafeFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT LineNumbersFilter : public Filter
{
  Q_OBJECT
public:
  LineNumbersFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  bool needsAutoEscape()
  {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class LowerFilter : public Filter
{
  Q_OBJECT
public:
  LowerFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

// class MakeListFilter : public Filter
// {
//   Q_OBJECT
// public:
//   MakeListFilter( QObject *parent = 0 );
//
//   bool isSafe() {
//     return false;
//   }
//
//   Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;
//
// };

class StringFormatFilter : public Filter
{
  Q_OBJECT
public:
  StringFormatFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;
};

class TitleFilter : public Filter
{
  Q_OBJECT
public:
  TitleFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;
};

class GRANTLEE_EXPORT TruncateWordsFilter : public Filter
{
  Q_OBJECT
public:
  TruncateWordsFilter( QObject *parent = 0 );

  bool isSafe() const {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT UpperFilter : public Filter
{
  Q_OBJECT
public:
  UpperFilter( QObject *parent = 0 );

  // &amp; may be safe, but it will be changed to &AMP; which is not safe.
  bool isSafe() {
    return false;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT WordCountFilter : public Filter
{
  Q_OBJECT
public:
  WordCountFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT LJustFilter : public Filter
{
  Q_OBJECT
public:
  LJustFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT RJustFilter : public Filter
{
  Q_OBJECT
public:
  RJustFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT CenterFilter : public Filter
{
  Q_OBJECT
public:
  CenterFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT EscapeFilter : public Filter
{
  Q_OBJECT
public:
  EscapeFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT ForceEscapeFilter : public Filter
{
  Q_OBJECT
public:
  ForceEscapeFilter( QObject *parent = 0 );

  bool isSafe() {
    return true;
  }

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = Grantlee::SafeString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT RemoveTagsFilter : public Filter
{
  Q_OBJECT
public:
  RemoveTagsFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};

class GRANTLEE_EXPORT StripTagsFilter : public Filter
{
  Q_OBJECT
public:
  StripTagsFilter( QObject *parent = 0 );

  Grantlee::SafeString doFilter( const QVariant &input, const Grantlee::SafeString &argument = QString(), bool autoescape = false ) const;

};


#endif