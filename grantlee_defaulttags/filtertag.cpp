
#include "filtertag.h"
#include "parser.h"
#include "filterexpression.h"

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
  p->nextToken();

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
  QString filtered = m_fe.resolve(c).toString();
  c->pop();
  return filtered;
}



