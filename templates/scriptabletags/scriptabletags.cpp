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

#include "scriptabletags.h"

#include <QtCore/QFile>
#include <QtPlugin>

#include <QtScript/QScriptEngine>

#include "nodebuiltins_p.h"

#include "exception.h"
#include "scriptablefilter.h"
#include "scriptablefilterexpression.h"
#include "scriptablenode.h"
#include "scriptablesafestring.h"
#include "scriptabletemplate.h"
#include "scriptablevariable.h"

#include "token.h"

Q_DECLARE_METATYPE(Token)

using namespace Grantlee;

QScriptValue tokenToScriptValue(QScriptEngine *engine, const Token &t)
{
  auto obj = engine->newObject();
  obj.setProperty(QStringLiteral("tokenType"), t.tokenType);
  obj.setProperty(QStringLiteral("content"), t.content);
  return obj;
}

void tokenFromScriptValue(const QScriptValue &obj, Token &t)
{
  t.tokenType = obj.property(QStringLiteral("tokenType")).toInt32();
  t.content = obj.property(QStringLiteral("content")).toString();
}

ScriptableTagLibrary::ScriptableTagLibrary(QObject *parent)
    : QObject(parent), m_scriptEngine(0)
{
  m_scriptEngine = new QScriptEngine(this);

  qScriptRegisterMetaType(m_scriptEngine, tokenToScriptValue,
                          tokenFromScriptValue);
  qScriptRegisterMetaType(m_scriptEngine, nodeToScriptValue,
                          nodeFromScriptValue);

  // Make Node new-able
  auto nodeCtor = m_scriptEngine->newFunction(ScriptableNodeConstructor);
  auto nodeMetaObject = m_scriptEngine->newQMetaObject(
      &ScriptableNode::staticMetaObject, nodeCtor);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("Node"),
                                             nodeMetaObject);

  // Make Variable new-able
  auto variableCtor
      = m_scriptEngine->newFunction(ScriptableVariableConstructor);
  auto variableMetaObject = m_scriptEngine->newQMetaObject(
      &VariableNode::staticMetaObject, variableCtor);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("Variable"),
                                             variableMetaObject);

  // Make FilterExpression new-able
  auto filterExpressionCtor
      = m_scriptEngine->newFunction(ScriptableFilterExpressionConstructor);
  auto filterExpressionMetaObject = m_scriptEngine->newQMetaObject(
      &ScriptableFilterExpression::staticMetaObject, filterExpressionCtor);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("FilterExpression"),
                                             filterExpressionMetaObject);

  // Make Template new-able
  auto templateCtor
      = m_scriptEngine->newFunction(ScriptableTemplateConstructor);
  auto templateMetaObject = m_scriptEngine->newQMetaObject(
      &ScriptableTemplate::staticMetaObject, templateCtor);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("Template"),
                                             templateMetaObject);

  // Create a global Library object
  auto libraryObject = m_scriptEngine->newQObject(this);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("Library"),
                                             libraryObject);

  // Create a global AbstractNodeFactory object to make smartSplit available.
  auto nodeFactory = new ScriptableNodeFactory(this);
  auto nodeFactoryObject = m_scriptEngine->newQObject(nodeFactory);
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("AbstractNodeFactory"), nodeFactoryObject);

  // Make mark_safe a globally available object.
  auto markSafeFunctionObject = m_scriptEngine->newFunction(markSafeFunction);
  m_scriptEngine->globalObject().setProperty(QStringLiteral("mark_safe"),
                                             markSafeFunctionObject);
}

bool ScriptableTagLibrary::evaluateScript(const QString &name)
{
  QFile scriptFile(name);

  if (!scriptFile.exists()
      || !scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fstream(&scriptFile);
  fstream.setCodec("UTF-8");
  const auto fileContent = fstream.readAll();

  scriptFile.close();

  m_scriptEngine->evaluate(fileContent);

  if (m_scriptEngine->hasUncaughtException()) {
    throw Grantlee::Exception(TagSyntaxError,
                              m_scriptEngine->uncaughtExceptionBacktrace().join(
                                  QChar::fromLatin1(' ')));
  }
  return true;
}

QHash<QString, AbstractNodeFactory *>
ScriptableTagLibrary::nodeFactories(const QString &name)
{
  m_factoryNames.clear();
  m_nodeFactories.clear();
  QHash<QString, AbstractNodeFactory *> h;

  if (!evaluateScript(name)) {
    return h;
  }

  return getFactories();
}

QHash<QString, Filter *> ScriptableTagLibrary::filters(const QString &name)
{
  m_filterNames.clear();
  m_filters.clear();
  QHash<QString, Filter *> filters;

  if (!evaluateScript(name)) {
    return filters;
  }

  return getFilters();
}

QHash<QString, AbstractNodeFactory *> ScriptableTagLibrary::getFactories()
{
  QHash<QString, AbstractNodeFactory *> factories;
  QHashIterator<QString, QString> it(m_factoryNames);
  while (it.hasNext()) {
    it.next();
    auto factoryName = it.value();
    auto tagName = it.key();

    auto factoryObject = m_scriptEngine->globalObject().property(factoryName);

    auto snf = new ScriptableNodeFactory();
    snf->setScriptEngine(m_scriptEngine);
    snf->setFactory(factoryObject);

    factories.insert(tagName, snf);
  }

  return factories;
}

QHash<QString, Filter *> ScriptableTagLibrary::getFilters()
{
  QHash<QString, Filter *> filters;

  QListIterator<QString> it(m_filterNames);
  while (it.hasNext()) {
    auto filterObject = m_scriptEngine->globalObject().property(it.next());
    auto filterName
        = filterObject.property(QStringLiteral("filterName")).toString();
    auto filter = new ScriptableFilter(filterObject, m_scriptEngine);
    filters.insert(filterName, filter);
  }
  if (m_scriptEngine->hasUncaughtException()) {
    throw Grantlee::Exception(TagSyntaxError,
                              m_scriptEngine->uncaughtExceptionBacktrace().join(
                                  QChar::fromLatin1(' ')));
  }

  return filters;
}

void ScriptableTagLibrary::addFactory(const QString &factoryName,
                                      const QString &tagName)
{
  m_factoryNames.insert(tagName, factoryName);
}

void ScriptableTagLibrary::addFilter(const QString &filterName)
{
  m_filterNames << filterName;
}
