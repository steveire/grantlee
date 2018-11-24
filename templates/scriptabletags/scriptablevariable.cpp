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

#include "scriptablevariable.h"

#include <QtScript/QScriptEngine>

#include "scriptablesafestring.h"
#include "util.h"

ScriptableVariable::ScriptableVariable(QObject *parent)
    : QObject(parent), m_engine(0)
{
}

ScriptableVariable::ScriptableVariable(QScriptEngine *engine, QObject *parent)
    : QObject(parent), m_engine(engine)
{
}

void ScriptableVariable::setContent(const QString &content)
{
  m_variable = Variable(content);
}

QVariant ScriptableVariable::resolve(ScriptableContext *c)
{
  auto var = m_variable.resolve(c->context());

  if (Grantlee::isSafeString(var)) {
    auto ssObj = new ScriptableSafeString(m_engine);
    ssObj->setContent(getSafeString(var));
    return m_engine->newQObject(ssObj).toVariant();
  }
  return var;
}

bool ScriptableVariable::isTrue(ScriptableContext *c)
{
  return m_variable.isTrue(c->context());
}
