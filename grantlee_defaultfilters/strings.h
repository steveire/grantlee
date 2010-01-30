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

//krazy:excludeall=includes

#include "filter.h"

using namespace Grantlee;

class AddSlashesFilter : public Filter
{
public:
  AddSlashesFilter();

  virtual bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class CapFirstFilter : public Filter
{
public:
  CapFirstFilter();

  virtual bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class EscapeJsFilter : public Filter
{
public:
  EscapeJsFilter();

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

private:
  QList<QPair<QString, QString> > m_jsEscapes;
};

class FixAmpersandsFilter : public Filter
{
public:
  FixAmpersandsFilter();

  virtual bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};


class CutFilter : public Filter
{
public:
  CutFilter();

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class SafeFilter : public Filter
{
public:
  SafeFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class LineNumbersFilter : public Filter
{
public:
  LineNumbersFilter();

  bool isSafe() const {
    return true;
  }

  bool needsAutoescape() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class LowerFilter : public Filter
{
public:
  LowerFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class StringFormatFilter : public Filter
{
public:
  StringFormatFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class TitleFilter : public Filter
{
public:
  TitleFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class TruncateWordsFilter : public Filter
{
public:
  TruncateWordsFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class UpperFilter : public Filter
{
public:
  UpperFilter();

  // &amp; may be safe, but it will be changed to &AMP; which is not safe.
  bool isSafe() const {
    return false;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class WordCountFilter : public Filter
{
public:
  WordCountFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class LJustFilter : public Filter
{
public:
  LJustFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class RJustFilter : public Filter
{
public:
  RJustFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class CenterFilter : public Filter
{
public:
  CenterFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class EscapeFilter : public Filter
{
public:
  EscapeFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class ForceEscapeFilter : public Filter
{
public:
  ForceEscapeFilter();

  bool isSafe() const {
    return true;
  }

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class RemoveTagsFilter : public Filter
{
public:
  RemoveTagsFilter();

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class StripTagsFilter : public Filter
{
public:
  StripTagsFilter();

  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;
};

class WordWrapFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class FloatFormatFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class SafeSequenceFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

class LineBreaksFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }

  bool needsAutoescape() const {
    return true;
  }
};

class LineBreaksBrFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }

  bool needsAutoescape() const {
    return true;
  }
};

class SlugifyFilter : public Filter
{
public:
  QVariant doFilter( const QVariant &input, const QVariant &argument = QVariant(), bool autoescape = false ) const;

  bool isSafe() const {
    return true;
  }
};

#endif
