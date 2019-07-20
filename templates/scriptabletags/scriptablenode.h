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

#ifndef SCRIPTABLE_NODE_H
#define SCRIPTABLE_NODE_H

#include <QtQml/QJSValue>

#include "node.h"

class QJSEngine;

namespace Grantlee
{
class Context;
class Engine;
}

using namespace Grantlee;

class ScriptableNode : public Node
{
  Q_OBJECT
public:
  ScriptableNode(QObject *parent = {});
  void setScriptEngine(QJSEngine *engine);
  void init(const QJSValue &concreteNode, const QJSValue &renderMethod);

  QJSEngine *engine();

  void render(OutputStream *stream, Context *c) const override;

private:
  QJSEngine *m_scriptEngine;
  QJSValue m_concreteNode;
  QJSValue m_renderMethod;

public Q_SLOTS:
  void setNodeList(const QString &name, const QList<QObject *> &);
};

class ScriptableNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ScriptableNodeFactory(QObject *parent = {});
  void setScriptEngine(QJSEngine *engine);

  /* reimp */ void setEngine(Grantlee::Engine *engine) override;
  void setFactory(const QJSValue &factoryMethod);

  Node *getNode(const QString &tagContent, Parser *p = {}) const override;

private:
  QJSEngine *m_scriptEngine;
  QJSValue m_factoryMethod;
};

#endif
