
#include "scriptablecontext.h"

#include "context.h"
#include "node.h"


ScriptableContext::ScriptableContext(Context *c, QObject *parent)
  : QObject(parent), m_c(c)
{

}


QVariant ScriptableContext::lookup(const QString &name)
{
  return m_c->lookup(name);
}

void ScriptableContext::insert(const QString &name, const QVariant &variant)
{
  m_c->insert(name, variant);
}

void ScriptableContext::push()
{
  m_c->push();
}

void ScriptableContext::pop()
{
  m_c->pop();
}

QString ScriptableContext::render(const QObjectList &list)
{
  NodeList nodeList;
  QListIterator<QObject *> it(list);
  while (it.hasNext())
  {
    Node *node = qobject_cast<Node*>(it.next());
    if (node)
      nodeList << node;
  }
  return nodeList.render(m_c);
}
