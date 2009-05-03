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

QString NodeList::render(Context *c)
{
  QString ret;
  for (int i = 0; i < this->size(); ++i)
  {
    ret += this->at(i)->render(c);
  }
  return ret;

}

AbstractNodeFactory::~AbstractNodeFactory()
{

}

VariableNode::VariableNode(const FilterExpression &fe)
{
  m_filterExpression = fe;
}

QString VariableNode::render(Context *c)
{
  return m_filterExpression.resolve(c).toString();
}