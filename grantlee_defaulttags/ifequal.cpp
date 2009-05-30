/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "ifequal.h"

#include <QStringList>

#include "parser.h"

#include <QDebug>


IfEqualNodeFactory::IfEqualNodeFactory()
{

}

Node* IfEqualNodeFactory::do_getNode(const QString &tagContent, Parser *p, bool negate, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  QList<QString> vars;

  FilterExpression val1(expr.at(1), p);
  FilterExpression val2(expr.at(2), p);

  const QString endTag("end" + expr.at(0));
  NodeList trueList = p->parse(QStringList() << "else" << endTag, parent);
  NodeList falseList;
  if (p->nextToken().content.trimmed() == "else")
  {
    falseList = p->parse(QStringList() << endTag, parent);
    p->deleteNextToken();
  } // else empty falseList.

  return new IfEqualNode(val1 ,val2, trueList, falseList, negate, parent);
}

Node* IfEqualNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const

{
  return do_getNode(tagContent, p, false, parent);
}


IfNotEqualNodeFactory::IfNotEqualNodeFactory()
{

}

Node* IfNotEqualNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  return do_getNode(tagContent, p, true, parent);
}

IfEqualNode::IfEqualNode(FilterExpression val1, FilterExpression val2, NodeList trueList, NodeList falseList, bool negate, QObject *parent)
  : Node(parent)
{
  m_var1 = val1;
  m_var2 = val2;
  m_trueList = trueList;
  m_falseList = falseList;
  m_negate = negate;
}

QString IfEqualNode::render(Context *c)
{
  QVariant var1 = m_var1.resolve(c);
  QVariant var2 = m_var2.resolve(c);

  bool equal = ((var1 == var2) && (var1.type() == var2.type()));

  if ( ( ( m_negate && !equal ) || ( !m_negate && equal ) ) )
    return m_trueList.render(c);
  return m_falseList.render(c);

}

NodeList IfEqualNode::getNodesByType(const char* className)
{
  NodeList list;
  list << m_trueList.getNodesByType(className);
  list << m_falseList.getNodesByType(className);
  return list;
}

