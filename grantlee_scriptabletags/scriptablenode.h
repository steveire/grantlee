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

#ifndef SCRIPTABLE_NODE_H
#define SCRIPTABLE_NODE_H

#include <QScriptValue>

#include <QSharedPointer>

#include "node.h"

class QScriptEngine;
class QScriptContext;

namespace Grantlee
{
class Context;
}

typedef QSharedPointer<QScriptEngine> ScriptEnginePointer;

using namespace Grantlee;

Q_DECLARE_METATYPE( Grantlee::Node* )

QScriptValue ScriptableNodeConstructor( QScriptContext *context,
                                        QScriptEngine *engine );


QScriptValue nodeToScriptValue( QScriptEngine *engine, Node* const &node );

void nodeFromScriptValue( const QScriptValue &object, Node* &out );

class ScriptableNode : public Node
{
  Q_OBJECT
public:
  ScriptableNode( QObject* parent = 0 );
  void setEngine( QScriptEngine* engine );
  void init( const QScriptValue &concreteNode,
             const QScriptValue &renderMethod );

  QScriptEngine* engine();

  QString render( Context *c );

public slots:
  QObjectList scriptableNodesbyType( const char * className );

private:
  QScriptEngine* m_scriptEngine;
  QScriptValue m_concreteNode;
  QScriptValue m_renderMethod;
};

class ScriptableNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ScriptableNodeFactory( QObject* parent = 0 );
  void setEngine( QScriptEngine *engine );
  void setFactory( QScriptValue factoryMethod );

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent = 0 ) const;

private:
  QScriptEngine* m_scriptEngine;
  QScriptValue m_factoryMethod;
};


#endif

