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

#ifndef GRANTLEE_VARIABLE_H
#define GRANTLEE_VARIABLE_H

#include <QtCore/QVariant>

#include "grantlee_core_export.h"

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
class GRANTLEE_CORE_EXPORT Variable
{
public:
  Variable();
  explicit Variable( const QString &var );
  Variable( const Variable &other );
  ~Variable();

  Variable &operator=( const Variable &other );

  bool isValid() const;

  QString toString() const;
  bool isTrue( Context *c ) const;

  QVariant resolve( Context *c ) const;

  bool isConstant() const;

private:
  Q_DECLARE_PRIVATE( Variable )
  VariablePrivate * const d_ptr;
};

}

#endif

