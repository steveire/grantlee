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

#ifndef GRANTLEE_FILTEREXPRESSION_H
#define GRANTLEE_FILTEREXPRESSION_H

#include "variable.h"

#include "grantlee_export.h"
#include "filter.h"

namespace Grantlee
{
class Parser;
class Filter;
class Token;

typedef QPair<Filter::Ptr, Variable > ArgFilter;

class FilterExpressionPrivate;

/**
  @brief A FilterExpression object represents a filter expression in a template.

  This class is only relevant if implementing custom tags or filters. Most of the API here is internal.
  Usually when implementing tags or filters, filter expressions will just be created and resolved.
*/
class GRANTLEE_EXPORT FilterExpression
{
public:
  /**
    Constructs an invalid FilterExpression.
  */
  FilterExpression();

  /**
    Constructs a filter expression from the string @p varString. The Parser @p parser is used to retrieve filters.
  */
  FilterExpression( const QString &varString, Grantlee::Parser *parser );

  /**
    Copy constructor.
  */
  FilterExpression( const FilterExpression &other );

  /**
    Destructor.
  */
  ~FilterExpression();

  /**
    Returns whether a filter expression is valid.

    A FilterExpression is valid if all filters in the expression exist and the initial variable being filtered is valid.
  */
  bool isValid() const;

  /**
    Assignment operator.
  */
  FilterExpression &operator=( const FilterExpression &other );

  /**
    Returns the initial variable in the FilterExpression.
  */
  Variable variable() const;

  /**
    Resolves the FilterExpression in the Context @p c.
  */
  QVariant resolve( Context *c ) const;

  /**
    Returns whether the Filter resolves to true in the Context @p c.
    @see truthiness
  */
  bool isTrue( Context *c ) const;

  /**
    Returns a list for the FilterExpression.
    If the FilterExpression can not be resolved to a list, an empty list will be returned.
  */
  QVariantList toList( Context *c ) const;

  /**
    Returns the list of filters in the FilterExpression.
  */
  QStringList filters() const;

private:
  Q_DECLARE_PRIVATE( FilterExpression )
  FilterExpressionPrivate * const d_ptr;
};

}

#endif
