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

#ifndef FILTEREXPRESSION_H
#define FILTEREXPRESSION_H

#include "variable.h"
#include "grantlee.h"

#include "grantlee_export.h"
#include "filter.h"

namespace Grantlee
{
class Parser;
class Filter;
}

using namespace Grantlee;

typedef QPair<Filter::Ptr, Variable > ArgFilter;

class Token;

namespace Grantlee
{

class FilterExpressionPrivate;

class GRANTLEE_EXPORT FilterExpression
{
public:
  enum Reversed {
    IsNotReversed,
    IsReversed
  };

  FilterExpression();
  FilterExpression( const QString &varString, Grantlee::Parser *parser );
  FilterExpression( const FilterExpression &other );
  ~FilterExpression();

  bool isValid() const;

  FilterExpression &operator=( const FilterExpression &other );

  Variable variable() const;

  QVariant resolve( Context *c ) const;

  bool isTrue( Context *c ) const;

  QVariantList toList( Context *c ) const;

  bool isConstant() const;

  QStringList filters() const;

private:
  Q_DECLARE_PRIVATE( FilterExpression )
  FilterExpressionPrivate *d_ptr;
};

}

#endif

