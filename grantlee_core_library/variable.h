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

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>

namespace Grantlee
{
class Context;

class VariablePrivate;

class Variable
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
  VariablePrivate *d_ptr;
};

}

#endif

