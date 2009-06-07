
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
