/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "node.h"

#include <QDebug>

using namespace Grantlee;

namespace Grantlee
{

class NodePrivate
{
  NodePrivate(Node *node)
    : q_ptr(node)
  {

  }
  Q_DECLARE_PUBLIC(Node)
  Node *q_ptr;
};

class AbstractNodeFactoryPrivate
{
  AbstractNodeFactoryPrivate(AbstractNodeFactory *factory)
    : q_ptr(factory)
  {

  }

  Q_DECLARE_PUBLIC(AbstractNodeFactory)
  AbstractNodeFactory *q_ptr;
};

}

Node::Node(QObject *parent) : QObject(parent),
  d_ptr(new NodePrivate(this))
{

}

Node::~Node()
{
  delete d_ptr;
}

NodeList::NodeList()
  : QList<Grantlee::Node*>()
{

}

NodeList::NodeList(const NodeList &list)
  : QList<Grantlee::Node*>(list)
{

}

NodeList::NodeList(const QList<Grantlee::Node *> &list)
  : QList<Grantlee::Node*>(list)
{

}

NodeList::~NodeList()
{
}

QString NodeList::render(Context *c)
{
  QString ret;
  
  for (int i = 0; i < this->size(); ++i)
  {
    QString s = this->at(i)->render(c);
    if (s.isNull())
      return QString();
    ret += s;
  }
  
  if (ret.isNull())
    return QString("");
  
  return ret;

}

AbstractNodeFactory::AbstractNodeFactory(QObject *parent)
  : QObject(parent), d_ptr(new AbstractNodeFactoryPrivate(this))
{

}

AbstractNodeFactory::~AbstractNodeFactory()
{
  delete d_ptr;
}

QList< Variable > AbstractNodeFactory::getVariableList(const QStringList &list) const
{
  QList<Variable> variables;
  QListIterator<QString> it(list);
  while (it.hasNext())
  {
    QString varString = it.next();
    variables << Variable(varString);
  }
  return variables;
}

QList< FilterExpression > AbstractNodeFactory::getFilterExpressionList(const QStringList &list) const
{
  QList<FilterExpression> fes;
  QListIterator<QString> it(list);
  while (it.hasNext())
  {
    QString varString = it.next();
    fes << FilterExpression(varString);
  }
  return fes;
}

QStringList AbstractNodeFactory::smartSplit(const QString &str) const
{
  QRegExp r("(\"(?:[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*)\"|\\\'(?:[^\\\'\\\\]*(?:\\\\.[^\\\'\\\\]*)*)\\\'|[^\\s]+)");

  QStringList l;
  int count = 0;
  int pos = 0;
  while ((pos = r.indexIn(str, pos)) != -1) {
      ++count;
      pos += r.matchedLength();
      l << r.capturedTexts().at(0);
  }

  return l;
}

TextNode::TextNode(const QString &content, QObject *parent)
  : Node(parent), m_content(content)
{
}

VariableNode::VariableNode(const FilterExpression &fe, QObject *parent)
  : Node(parent), m_filterExpression(fe)
{
}

QString VariableNode::render(Context *c)
{
  QVariant v = m_filterExpression.resolve(c);
  if (!v.isValid())
    return QString("");
  return v.toString();
}

