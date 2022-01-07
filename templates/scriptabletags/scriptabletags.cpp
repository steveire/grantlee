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

#include <QtQml/QJSEngine>
#include <QtQml/QJSValueIterator>

#include "nodebuiltins_p.h"

#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "scriptablefilter.h"
#include "scriptablefilterexpression.h"
#include "scriptablenode.h"
#include "scriptablesafestring.h"
#include "scriptabletemplate.h"
#include "scriptablevariable.h"

#include "token.h"
#include "util.h"

Q_DECLARE_METATYPE(Token)

using namespace Grantlee;

QJSValue ScriptableHelperFunctions::markSafeFunction(QJSValue inputValue)
{
  if (inputValue.isQObject()) {
    auto obj = inputValue.toQObject();
    auto ssObj = qobject_cast<ScriptableSafeString *>(obj);
    if (!ssObj)
      return QJSValue::NullValue;

    ssObj->setSafety(true);
    return m_scriptEngine->newQObject(ssObj);
  }
  if (inputValue.isString()) {
    auto str = inputValue.toString();
    auto ssObj = new ScriptableSafeString(m_scriptEngine);
    ssObj->setContent(markSafe(str));
    return m_scriptEngine->newQObject(ssObj);
  }
  return QJSValue::NullValue;
}

QJSValue ScriptableHelperFunctions::ScriptableFilterExpressionConstructor(
    QString name, QObject *parserObj)
{
  auto object = new ScriptableFilterExpression(m_scriptEngine);

  auto p = qobject_cast<Parser *>(parserObj);

  object->init(name, p);

  return m_scriptEngine->newQObject(object);
}

QJSValue
ScriptableHelperFunctions::ScriptableNodeConstructor(QJSValue callContext)
{
  QJSValueIterator it(callContext);
  it.next();
  auto scriptableNodeName = it.value().toString();
  auto concreteNode
      = m_scriptEngine->globalObject().property(scriptableNodeName);

  QJSValueList args;
  while (it.next())
    args << it.value();

  concreteNode = concreteNode.callAsConstructor(args);

  auto renderMethod = concreteNode.property(QStringLiteral("render"));

  auto object = new ScriptableNode(m_scriptEngine);
  object->setObjectName(scriptableNodeName);
  object->setScriptEngine(m_scriptEngine);
  object->init(concreteNode, renderMethod);
  return m_scriptEngine->newQObject(object);
}

QJSValue ScriptableHelperFunctions::ScriptableTemplateConstructor(
    QString content, QString name, QObject *parent)
{
  auto templateEngine
      = m_scriptEngine->property("templateEngine").value<Engine *>();

  if (!templateEngine)
    return {};

  auto t = templateEngine->newTemplate(content, name);

  auto object = new ScriptableTemplate(t, parent);
  return m_scriptEngine->newQObject(object);
}

QJSValue ScriptableHelperFunctions::ScriptableVariableConstructor(QString name)
{
  // TODO: Decide what the parent should be;
  // It should be the owning scriptableNode. I think I can get that from the
  // scriptContext.

  QObject *parent = nullptr;
  auto object = new ScriptableVariable(m_scriptEngine, parent);
  object->setContent(name);

  return m_scriptEngine->newQObject(object);
}

ScriptableTagLibrary::ScriptableTagLibrary(QObject *parent)
    : QObject(parent), m_scriptEngine(new QJSEngine(this)),
      m_functions(m_scriptEngine->newQObject(
          new ScriptableHelperFunctions(m_scriptEngine)))
{
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("internalGrantleeFunctions"), m_functions);

  // Make Node new-able
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("Node"),
      m_scriptEngine->evaluate(QStringLiteral(R"javascript(
            (function() {
              return internalGrantleeFunctions.ScriptableNodeConstructor(
                Array.prototype.slice.call(arguments));
            })
          )javascript")));

  // Make Variable new-able
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("Variable"),
      m_functions.property(QStringLiteral("ScriptableVariableConstructor")));

  // Make FilterExpression new-able
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("FilterExpression"),
      m_functions.property(
          QStringLiteral("ScriptableFilterExpressionConstructor")));

  // Make Template new-able
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("Template"),
      m_functions.property(QStringLiteral("ScriptableTemplateConstructor")));

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
  m_scriptEngine->globalObject().setProperty(
      QStringLiteral("mark_safe"),
      m_functions.property(QStringLiteral("markSafeFunction")));
}

bool ScriptableTagLibrary::evaluateScript(const QString &name)
{
  QFile scriptFile(name);

  if (!scriptFile.exists()
      || !scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream fstream(&scriptFile);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  fstream.setCodec("UTF-8");
#else
  fstream.setEncoding(QStringConverter::Utf8);
#endif
  const auto fileContent = fstream.readAll();

  scriptFile.close();

  QJSValue result = m_scriptEngine->evaluate(fileContent);
  if (result.isError())
    throw Grantlee::Exception(TagSyntaxError, result.toString());

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
  for (auto it = m_factoryNames.begin(), end = m_factoryNames.end(); it != end;
       ++it) {
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

  for (auto &filterNameString : m_filterNames) {
    auto filterObject
        = m_scriptEngine->globalObject().property(filterNameString);
    auto filterName
        = filterObject.property(QStringLiteral("filterName")).toString();
    auto filter = new ScriptableFilter(filterObject, m_scriptEngine);
    filters.insert(filterName, filter);
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
