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

#include <QtQml/QJSValue>

class QJSEngine;

namespace Grantlee
{
class Engine;
class Parser;

class ScriptableHelperFunctions : public QObject
{
  Q_OBJECT
  QJSEngine *m_scriptEngine;

public:
  ScriptableHelperFunctions(QJSEngine *scriptEngine)
      : m_scriptEngine(scriptEngine)
  {
  }

  Q_INVOKABLE QJSValue markSafeFunction(QJSValue inputValue);
  Q_INVOKABLE QJSValue ScriptableFilterExpressionConstructor(QString name,
                                                             QObject *parserObj
                                                             = {});
  Q_INVOKABLE QJSValue ScriptableNodeConstructor(QJSValue callContext);
  Q_INVOKABLE QJSValue ScriptableVariableConstructor(QString name);
  Q_INVOKABLE QJSValue ScriptableTemplateConstructor(QString content,
                                                     QString name,
                                                     QObject *parent);
};

class ScriptableTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES(Grantlee::TagLibraryInterface)
  Q_PLUGIN_METADATA(IID "org.grantlee.TagLibraryInterface")
public:
  ScriptableTagLibrary(QObject *parent = {});

  QHash<QString, AbstractNodeFactory *> nodeFactories(const QString &name
                                                      = {}) override;

  QHash<QString, Filter *> filters(const QString &name = {}) override;

public Q_SLOTS:
  void addFactory(const QString &factoryName, const QString &tagname);
  void addFilter(const QString &filterName);

protected:
  bool evaluateScript(const QString &name);
  QHash<QString, AbstractNodeFactory *> getFactories();
  QHash<QString, Filter *> getFilters();

private:
  QJSEngine *m_scriptEngine;
  QJSValue m_functions;
  QHash<QString, AbstractNodeFactory *> m_nodeFactories;
  QHash<QString, QString> m_factoryNames;
  QStringList m_filterNames;
  QHash<QString, Filter *> m_filters;
};
}

#endif
