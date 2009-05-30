/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "debug.h"

#include <QStringList>
#include "parser.h"
#include "grantlee.h"


DebugNodeFactory::DebugNodeFactory()
{

}

Node* DebugNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  return new DebugNode(parent);
}


DebugNode::DebugNode(QObject *parent)
  : Node(parent)
{

}

QString DebugNode::render(Context *c)
{
  QString ret;
  int i= 0;
  QHash<QString, QVariant> h = c->stackHash(i++);

  ret += "\n\nContext:\n";
  while (!h.isEmpty())
  {
    QHashIterator<QString, QVariant> it(h);
    while (it.hasNext())
    {
      it.next();
      ret += "key " + it.key() + ", ";
      ret += "type ";
      ret.append(it.value().typeName());
      ret += "\n";
    }
    h = c->stackHash(i++);
  }

  ret += "End context:\n\n";

  return ret;
}

