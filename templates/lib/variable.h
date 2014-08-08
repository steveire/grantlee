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

#ifndef GRANTLEE_VARIABLE_H
#define GRANTLEE_VARIABLE_H

#include "grantlee_templates_export.h"

#include <QtCore/QVariant>

namespace Grantlee
{
class Context;

class VariablePrivate;

/// @headerfile variable.h grantlee/variable.h

/**
  @brief A container for static variables defined in Templates.

  This class is only relevant to Template tag authors.

  When processing a template tag in a AbstractNodeFactory implementation, it will sometimes make sense
  to process arguments to the tag as Grantlee::Variables. Note that usually they should be processed as
  FilterExpression objects instead.

  Arguments to the tag can be used to construct Variables, which may then be resolved into the objects they
  represent in the given Context in the render stage.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TEMPLATES_EXPORT Variable
{
public:
  /**
    Constructs an invalid Variable
  */
  Variable();

  /**
    Creates a Variable represented by the given @p var
  */
  explicit Variable( const QString &var );

  /**
    Copy constructor
  */
  Variable( const Variable &other );

  /**
    Destructor
  */
  ~Variable();

  /**
    Assignment operator.
  */
  Variable &operator=( const Variable &other );

  /**
    Returns whether this Variable is valid.
  */
  bool isValid() const;

  /**
    Returns whether this Variable evaluates to true with the Context @p c.
  */
  bool isTrue( Context *c ) const;

  /**
    Resolves this Variable with the Context @p c.
  */
  QVariant resolve( Context *c ) const;

  /**
    Returns whether this Variable is a constant in the Template. A constant is represented as a static string in the template

    @code
      Text content
      {% some_tag "constant" variable %}
    @endcode
  */
  bool isConstant() const;

  /**
    Returns whether this variable is localized, that is, if it is wrapped with _(). @see @ref i18n_l10n
   */
  bool isLocalized() const;

  /**
    Returns whether this variable is a literal string or number. A literal Variable does not have any lookup components.
   */
  QVariant literal() const;

  /**
    Returns the lookup components of this variable.
   */
  QStringList lookups() const;

private:
  Q_DECLARE_PRIVATE( Variable )
  VariablePrivate * const d_ptr;
};

}

#endif
