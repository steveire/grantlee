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

#ifndef GRANTLEE_FILTEREXPRESSION_H
#define GRANTLEE_FILTEREXPRESSION_H

#include "variable.h"

#include "grantlee_core_export.h"

namespace Grantlee
{
class Filter;
class OutputStream;
class Parser;
struct Token;

class FilterExpressionPrivate;

/// @headerfile filterexpression.h grantlee/filterexpression.h

/**
  @brief A FilterExpression object represents a filter expression in a template.

  This class is only relevant if implementing custom tags or filters. Most of the API here is internal.
  Usually when implementing tags or filters, filter expressions will just be created and resolved.

  In template markup, a filter expression is a variable followed by one or more filters separated by pipes:

  %Filter expressions may appear in variable nodes:
  @code
    {{ some_var|upper_filter|lower_filter }}
  @endcode

  Or as arguments to tags
  @code
    {% some_tag some_arg1|filter1|filter2 some_arg2|filter3 %}
  @endcode

  The FilterExpression class would be used in the getNode implementation of the AbstractNodeFactory implementation for the some_tag tag.

  @code
    Node* SomeTagFactory::getNode(const QString &tagContent, Parser *p) const {
      QStringList parts = smartSplit( tagContent );

      parts.removeFirst(); // Remove the "some_tag" part.

      FilterExpression arg1( parts.at( 0 ), p );
      FilterExpression arg2( parts.at( 1 ), p );

      return new SomeTagNode( arg1, arg2, p );
    }
  @endcode

  @see AbstractNodeFactory::getFilterExpressionList

  When implementing the AbstractNodeFactory::render method, the resolve method may be used to process the filter expression.

  For example, if our SomeTagNode was to concatenate the resolved values given as arguments:

  @code
    void SomeTagNode::render( QTextStream *stream, Context *c ) {
      m_arg1.resolve( stream, c );
      m_arg2.resolve( stream, c );
    }
  @endcode

  Because Filters are highly generic, they do not all write data to the stream. For example, a Filter might take as input a string, and return a list by splitting the string on commas, or a Filter might compare an input to its argument and return whether they are the same, but not write anything to the stream. For that reason, the resolve method writes data to the given stream, and returns the same data in its returned QVariant.

  The suitability of either of the resolve methods will depend on the implementation and requirements of your custom tag. For example if the SomeTagNode ran a comparison of the arguments:

  @code
    void SomeTagNode::render( QTextStream *stream, Context *c ) {
      QString first = m_arg1.resolve( c ).toString();
      QString second = m_arg2.resolve( c ).toString();

      if ( first == second )
        m_trueList.render( stream, c );
      else
        m_falseList.render( stream, c );
    }
  @endcode

  @see @ref tags_with_end_tags

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT FilterExpression
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
    Assignment operator.
  */
  FilterExpression &operator=( const FilterExpression &other );

  /**
    Returns the initial variable in the FilterExpression.
  */
  Variable variable() const;

  /**
    Resolves the FilterExpression in the Context @p c and writes it to the stream @p stream.
  */
  QVariant resolve( OutputStream *stream, Context *c ) const;

  /**
    Resolves the FilterExpression in the Context @p c.
  */
  QVariant resolve( Context *c ) const;

  /**
    Returns whether the Filter resolves to true in the Context @p c.
    @see @ref truthiness
  */
  bool isTrue( Context *c ) const;

  /**
    Returns a list for the FilterExpression.
    If the FilterExpression can not be resolved to a list, an empty list will be returned.
  */
  QVariantList toList( Context *c ) const;

  /**
    Returns whether a filter expression is valid.

    A FilterExpression is valid if all filters in the expression exist and the initial variable being filtered is valid.
  */
  bool isValid() const;

#ifndef Q_QDOC
  /**
    @internal
    Returns the list of filters in the FilterExpression.
  */
  QStringList filters() const;
#endif

private:
  Q_DECLARE_PRIVATE( FilterExpression )
  FilterExpressionPrivate * const d_ptr;
};

}

#endif

