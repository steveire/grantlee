/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "node.h"

#include <QDebug>

using namespace Grantlee;

Node::Node(QObject *parent) : QObject(parent)
{

}

Node::~Node()
{
}

NodeList::NodeList()
  : m_containsNonText(false)
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

AbstractNodeFactory::~AbstractNodeFactory()
{

}

QList< Variable > AbstractNodeFactory::getVariableList(QStringList list)
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

QStringList AbstractNodeFactory::smartSplit(const QString &str)
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

