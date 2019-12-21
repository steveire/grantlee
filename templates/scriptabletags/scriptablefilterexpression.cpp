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

#include "scriptablefilterexpression.h"

#include <QtQml/QJSEngine>

#include "parser.h"
#include "scriptablesafestring.h"
#include "util.h"

using namespace Grantlee;

ScriptableFilterExpression::ScriptableFilterExpression(QObject *parent)
    : QObject(parent), m_engine(nullptr)
{
}

ScriptableFilterExpression::ScriptableFilterExpression(QJSEngine *engine,
                                                       QObject *parent)
    : QObject(parent), m_engine(engine)
{
}

void ScriptableFilterExpression::init(const QString &content,
                                      Grantlee::Parser *parser)
{
  m_filterExpression = FilterExpression(content, parser);
}

QVariant ScriptableFilterExpression::resolve(ScriptableContext *c)
{
  auto var = m_filterExpression.resolve(c->context());

  if (Grantlee::isSafeString(var)) {
    auto ssObj = new ScriptableSafeString(m_engine);
    ssObj->setContent(getSafeString(var));
    return m_engine->newQObject(ssObj).toVariant();
  }
  return var;
}

bool ScriptableFilterExpression::isTrue(ScriptableContext *c)
{
  return m_filterExpression.isTrue(c->context());
}

bool ScriptableFilterExpression::equals(ScriptableFilterExpression *other,
                                        ScriptableContext *scriptableC)
{
  auto c = scriptableC->context();
  return Grantlee::equals(m_filterExpression.resolve(c),
                          other->m_filterExpression.resolve(c));
}
