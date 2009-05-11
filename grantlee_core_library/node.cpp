/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "node.h"
#include "text_util.h"

#include <QDebug>

using namespace Grantlee;

Node::Node(QObject *parent) : QObject(parent)
{

}

Node::~Node()
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

QStringList AbstractNodeFactory::smartSplit(const QString &str)
{
  return Grantlee::TextUtil::smartSplit(str);
}

VariableNode::VariableNode(const FilterExpression &fe)
{
  m_filterExpression = fe;
}

QString VariableNode::render(Context *c)
{
  QVariant v = m_filterExpression.resolve(c);
  if (!v.isValid())
    return QString("");
  return v.toString();
}

