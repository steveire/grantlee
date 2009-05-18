/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "with.h"

#include <QStringList>
#include "parser.h"
#include "grantlee.h"

#include <QDebug>

WithNodeFactory::WithNodeFactory()
{

}

Node* WithNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  QStringList expr = smartSplit(tagContent);

  if (expr.size() != 4 or expr.at(2) != "as")
  {
    emit error(TagSyntaxError, QString("%1 expected format is 'value as name'").arg(expr.at(0)));
    return 0;
  }

  FilterExpression fe(expr.at(1), p);
  QString name(expr.at(3));

  NodeList nodeList = p->parse(QStringList() << "endwith", parent);
  p->nextToken();

  return new WithNode(fe, name, nodeList, parent);
}


WithNode::WithNode(FilterExpression fe, const QString &name, NodeList list, QObject *parent)
  : Node(parent)
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

