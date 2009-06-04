/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "filtertag.h"
#include "parser.h"
#include "filterexpression.h"

#include "util_p.h"

FilterNodeFactory::FilterNodeFactory()
{

}

Grantlee::Node* FilterNodeFactory::getNode(const QString& tagContent, Grantlee::Parser* p, QObject *parent) const
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  expr.removeFirst();

  QString expression = expr.join(" ");
  FilterExpression fe(QString("var|%1").arg(expression), p);

  NodeList filterNodes = p->parse(QStringList() << "endfilter", parent );
  p->deleteNextToken();

  return new FilterNode(fe, filterNodes, parent);
}

FilterNode::FilterNode(FilterExpression fe, NodeList filterList, QObject *parent)
  : Node(parent), m_fe(fe), m_filterList(filterList)
{

}

QString FilterNode::render(Context* c)
{
  QString output = m_filterList.render(c);
  c->push();
  c->insert("var", output);
  QString filtered = Util::getSafeString(m_fe.resolve(c)).rawString();
  c->pop();
  return filtered;
}

NodeList FilterNode::getNodesByType(const char* className)
{
  return m_filterList.getNodesByType(className);
}




