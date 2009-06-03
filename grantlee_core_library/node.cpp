/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "node.h"


using namespace Grantlee;

namespace Grantlee
{

class NodePrivate
{
  NodePrivate(Node *node)
    : q_ptr(node), m_error(NoError)
  {

  }
  Q_DECLARE_PUBLIC(Node)
  Node *q_ptr;

  Error m_error;
  QString m_errorString;

};

class AbstractNodeFactoryPrivate
{
  AbstractNodeFactoryPrivate(AbstractNodeFactory *factory)
  : q_ptr(factory), m_error(NoError)
  {

  }

  mutable Error m_error;
  mutable QString m_errorString;

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

QString Node::renderValueInContext(const QVariant& input, Context* c)
{
  return input.toString();
}

void Node::setError(Error type, const QString &message)
{
  Q_D(Node);
  d->m_error = type;
  d->m_errorString = message;
}

Error Node::error() const
{
  Q_D(const Node);
  return d->m_error;
}

QString Node::errorString() const
{
  Q_D(const Node);
  return d->m_errorString;
}

NodeList Node::getNodesByType(const char * className)
{
  return NodeList();
}

NodeList::NodeList()
  : QList<Grantlee::Node*>(), m_error(NoError)
{

}

NodeList NodeList::getNodesByType(const char * className)
{
  NodeList list;
  QListIterator<Grantlee::Node *> it(*this);
  while (it.hasNext())
  {
    Grantlee::Node *n = it.next();
    if (n->metaObject()->className() == className)
      list << n;
    list << n->getNodesByType(className);
  }
  return list;
}

NodeList::NodeList(const NodeList &list)
  : QList<Grantlee::Node*>(list)
{
  m_error = list.error();
  m_errorString = list.errorString();
}

NodeList::NodeList(const QList<Grantlee::Node *> &list)
  : QList<Grantlee::Node*>(list), m_error(NoError)
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
    ret += this->at(i)->render(c);
    if (this->at(i)->error() != NoError)
    {
      setError(this->at(i)->error(), this->at(i)->errorString());
      break;
    }
  }

  return ret;

}

void NodeList::setError(Error type, const QString &message)
{
  m_error = type;
  m_errorString = message;
}

Error NodeList::error() const
{
  return m_error;
}

QString NodeList::errorString() const
{
  return m_errorString;
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

QList< FilterExpression > AbstractNodeFactory::getFilterExpressionList(const QStringList &list, Parser *p) const
{
  QList<FilterExpression> fes;
  QListIterator<QString> it(list);
  while (it.hasNext())
  {
    QString varString = it.next();
    fes << FilterExpression(varString, p);
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

void AbstractNodeFactory::setError(Error type, const QString &message) const
{
  Q_D(const AbstractNodeFactory);
  d->m_error = type;
  d->m_errorString = message;
}

Error AbstractNodeFactory::error() const
{
  Q_D(const AbstractNodeFactory);
  return d->m_error;
}

QString AbstractNodeFactory::errorString() const
{
  Q_D(const AbstractNodeFactory);
  return d->m_errorString;
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
    return QString();
  return renderValueInContext(v, c);
}

