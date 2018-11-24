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

#ifndef SCRIPTABLETAGS_H
#define SCRIPTABLETAGS_H

#include "node.h"
#include "taglibraryinterface.h"

#include <QtScript/QScriptValue>

class QScriptEngine;

namespace Grantlee
{
class Engine;
class Parser;

class ScriptableHelperFunctions : public QObject
{
  Q_OBJECT
  QScriptEngine *m_scriptEngine;

public:
  ScriptableHelperFunctions(QScriptEngine *scriptEngine)
      : m_scriptEngine(scriptEngine)
  {
  }

  Q_INVOKABLE QScriptValue markSafeFunction(QScriptValue inputValue);
  Q_INVOKABLE QScriptValue ScriptableFilterExpressionConstructor(
      QString name, QObject *parserObj = nullptr);
  Q_INVOKABLE QScriptValue ScriptableNodeConstructor(QScriptValue callContext);
  Q_INVOKABLE QScriptValue ScriptableVariableConstructor(QString name);
  Q_INVOKABLE QScriptValue ScriptableTemplateConstructor(QString content,
                                                         QString name,
                                                         QObject *parent);
};

class ScriptableTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES(Grantlee::TagLibraryInterface)
  Q_PLUGIN_METADATA(IID "org.grantlee.TagLibraryInterface")
public:
  ScriptableTagLibrary(QObject *parent = 0);

  QHash<QString, AbstractNodeFactory *> nodeFactories(const QString &name
                                                      = QString()) override;

  QHash<QString, Filter *> filters(const QString &name = QString()) override;

public Q_SLOTS:
  void addFactory(const QString &factoryName, const QString &tagname);
  void addFilter(const QString &filterName);

protected:
  bool evaluateScript(const QString &name);
  QHash<QString, AbstractNodeFactory *> getFactories();
  QHash<QString, Filter *> getFilters();

private:
  QScriptEngine *m_scriptEngine;
  QScriptValue m_functions;
  QHash<QString, AbstractNodeFactory *> m_nodeFactories;
  QHash<QString, QString> m_factoryNames;
  QStringList m_filterNames;
  QHash<QString, Filter *> m_filters;
};
}

#endif
