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

#ifndef SCRIPTABLE_VARIABLE_H
#define SCRIPTABLE_VARIABLE_H

#include <QtCore/QObject>
#include <QtQml/QJSValue>

#include "scriptablecontext.h"
#include "variable.h"

using namespace Grantlee;

class ScriptableVariable : public QObject
{
  Q_OBJECT
public:
  ScriptableVariable(QObject *parent = {});
  ScriptableVariable(QJSEngine *engine, QObject *parent = {});

  void setContent(const QString &content);

public Q_SLOTS:
  QVariant resolve(ScriptableContext *c);

  bool isTrue(ScriptableContext *c);

private:
  Variable m_variable;
  QJSEngine *m_engine;
};

#endif
