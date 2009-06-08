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

#ifndef SCRIPTABLE_VARIABLE_H
#define SCRIPTABLE_VARIABLE_H

#include <QObject>
#include <QScriptValue>

#include "variable.h"

class QScriptContext;

class ScriptableContext;

using namespace Grantlee;

QScriptValue ScriptableVariableConstructor( QScriptContext *context,
    QScriptEngine *engine );

class ScriptableVariable : public QObject
{
  Q_OBJECT
public:
  ScriptableVariable( QObject *parent = 0 );

  void setContent( const QString &content );

public slots:
  QVariant resolve( ScriptableContext *c );

  bool isTrue( ScriptableContext *c );

private:
  Variable m_variable;
};

#endif

