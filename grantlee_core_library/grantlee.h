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

#ifndef GRANTLEE_H
#define GRANTLEE_H

#include <exception>

#include <QDebug>

namespace Grantlee
{

enum LoopDirection {
  ForwardLoop,
  ReversedLoop
};

enum Error {
  NoError,
  EmptyVariableError,
  EmptyBlockTagError,
  InvalidBlockTagError,
  UnclosedBlockTagError,
  UnknownFilterError,
  TagSyntaxError,
//   VariableSyntaxError,

  VariableNotInContext,
  ObjectReturnTypeInvalid,
  CompileFunctionError
};

class Exception
{
public:
  Exception( Error errorCode, const QString &what )
      : m_errorCode( errorCode ), m_what( what ) {}

  virtual ~Exception() throw() {}

  virtual const QString what() const throw() {
    return m_what;
  }

  Error errorCode() const {
    return m_errorCode;
  }

private:
  Error m_errorCode;
  QString m_what;
};

}

#endif

