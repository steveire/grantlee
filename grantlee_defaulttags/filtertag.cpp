
#include "filtertag.h"
#include "parser.h"
#include "filterexpression.h"

FilterNodeFactory::FilterNodeFactory()
{

}

Grantlee::Node* FilterNodeFactory::getNode(const QString& tagContent, Grantlee::Parser* p)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  expr.removeFirst();

  QString expression = expr.join(" ");
  FilterExpression *fe = new FilterExpression(QString("var|%1").arg(expression), p);

  NodeList filterNodes = p->parse(QStringList() << "endfilter" );
  p->nextToken();

  return new FilterNode(fe, filterNodes);
}

FilterNode::FilterNode(FilterExpression* fe, NodeList filterList)
  : m_fe(fe), m_filterList(filterList)
{

}

QString FilterNode::render(Context* c)
{
  QString output = m_filterList.render(c);
  c->push();
  c->insert("var", output);
  QString filtered = m_fe->resolve(c).toString();
  c->pop();
  return filtered;
}



