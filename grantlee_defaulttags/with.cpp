/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "with.h"

#include <QStringList>
#include "parser.h"

#include <QDebug>

WithNodeFactory::WithNodeFactory()
{

}

Node* WithNodeFactory::getNode(const QString &tagContent, Parser *p)
{

  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  if (expr.size() != 4 or expr.at(2) != "as")
  {
    emit error(Parser::TagSyntaxError, QString("%1 expected format is 'value as name'").arg(expr.at(0)));
  }

  FilterExpression fe(expr.at(1), p);
  QString name(expr.at(3));

  NodeList nodeList = p->parse(QStringList() << "endwith");
  p->nextToken();

  return new WithNode(fe, name, nodeList);
}


WithNode::WithNode(FilterExpression fe, const QString &name, NodeList list)
{
  m_filterExpression = fe;
  m_name = name;
  m_list = list;
}

QString WithNode::render(Context *c)
{
  c->push();
  c->insert(m_name, m_filterExpression.resolve(c));
  QString ret = m_list.render(c);
  c->pop();
  return ret;
}

