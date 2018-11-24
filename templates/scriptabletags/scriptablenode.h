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

#include <QtScript/QScriptValue>

#include "node.h"

class QScriptEngine;
class QScriptContext;

namespace Grantlee
{
class Context;
class Engine;
}

using namespace Grantlee;

QScriptValue ScriptableNodeConstructor(QScriptContext *context,
                                       QScriptEngine *engine);

QScriptValue nodeToScriptValue(QScriptEngine *engine, Node *const &node);

void nodeFromScriptValue(const QScriptValue &object, Node *&out);

class ScriptableNode : public Node
{
  Q_OBJECT
public:
  ScriptableNode(QObject *parent = 0);
  void setScriptEngine(QScriptEngine *engine);
  void init(const QScriptValue &concreteNode, const QScriptValue &renderMethod);

  QScriptEngine *engine();

  void render(OutputStream *stream, Context *c) const override;

private:
  QScriptEngine *m_scriptEngine;
  QScriptValue m_concreteNode;
  QScriptValue m_renderMethod;

public Q_SLOTS:
  void setNodeList(const QString &name, const QObjectList &);
};

class ScriptableNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ScriptableNodeFactory(QObject *parent = 0);
  void setScriptEngine(QScriptEngine *engine);

  /* reimp */ void setEngine(Grantlee::Engine *engine) override;
  void setFactory(const QScriptValue &factoryMethod);

  Node *getNode(const QString &tagContent, Parser *p = 0) const override;

private:
  QScriptEngine *m_scriptEngine;
  QScriptValue m_factoryMethod;
};

#endif
