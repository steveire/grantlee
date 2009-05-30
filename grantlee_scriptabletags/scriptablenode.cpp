
#include "scriptablenode.h"

#include <QtScript/QScriptEngine>

#include "context.h"
#include "scriptablecontext.h"
#include "scriptableparser.h"
#include "parser.h"


QScriptValue nodeToScriptValue(QScriptEngine *engine, Node* const &node)
{
  return engine->newQObject(node);
}

void nodeFromScriptValue(const QScriptValue &object, Node* &out)
{
  out = qobject_cast<Node*>(object.toQObject());
}


Q_SCRIPT_DECLARE_QMETAOBJECT(ScriptableNode, Node*)

QScriptValue ScriptableNodeConstructor(QScriptContext *context,
                                        QScriptEngine *engine)
{
  QObject *parent = context->argument(context->argumentCount() - 1).toQObject();

  QScriptValue concreteNode = engine->globalObject().property(context->argument(0).toString());

  QScriptValueList args;
  // First is the node type
  for (int i = 1; i < context->argumentCount(); ++i)
  {
    args << context->argument(i);
  }

  concreteNode.call(concreteNode, args);

  QScriptValue renderMethod = concreteNode.property("render");

  ScriptableNode *object = new ScriptableNode(parent);
  object->setEngine(engine);
  object->init(concreteNode, renderMethod);
  return engine->newQObject(object);
}


ScriptableNode::ScriptableNode(QObject* parent): Node(parent), m_scriptEngine(0)
{
}


void ScriptableNode::setEngine(QScriptEngine *engine)
{
  m_scriptEngine = engine;
}

void ScriptableNode::init(const QScriptValue &concreteNode,
                          const QScriptValue &renderMethod)
{
  m_concreteNode = concreteNode;
  m_renderMethod = renderMethod;
}

QString ScriptableNode::render(Context *c)
{
  ScriptableContext sc(c);
  QScriptValue contextObject = m_scriptEngine->newQObject(&sc);

  QScriptValueList args;
  args << contextObject;

  // Call the render method in the context of the concreteNode
  return m_renderMethod.call(m_concreteNode, args).toString();
}

QObjectList ScriptableNode::scriptableNodesbyType(const char * className)
{
  NodeList nodeList = getNodesByType(className);

  QObjectList objList;
  QListIterator<Node*> it(nodeList);
  while (it.hasNext())
  {
    objList << it.next();
  }
  return objList;
}

ScriptableNodeFactory::ScriptableNodeFactory(QObject* parent)
  : AbstractNodeFactory(parent), m_scriptEngine(0)
{

}

void ScriptableNodeFactory::setEngine(QScriptEngine *engine)
{
  m_scriptEngine = engine;
}

void ScriptableNodeFactory::setFactory(QScriptValue factoryMethod)
{
  m_factoryMethod = factoryMethod;
}

Node* ScriptableNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  ScriptableParser *sp = new ScriptableParser(p);
  QScriptValue parserObject = m_scriptEngine->newQObject(sp);
  QScriptValue parentObject = m_scriptEngine->newQObject(parent);

  QScriptValueList args;
  args << tagContent;
  args << parserObject;
  args << parentObject;

  QScriptValue factory = m_factoryMethod;

  QScriptValue scriptNode = factory.call(factory, args);

  Node* node = qscriptvalue_cast<Node*>(scriptNode);
  ScriptableNode *sn = qobject_cast<ScriptableNode *>(node);
  return node;

}

QScriptEngine* ScriptableNode::engine()
{
  return m_scriptEngine;
}
