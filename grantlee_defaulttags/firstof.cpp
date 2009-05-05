/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "firstof.h"

#include <QStringList>
#include "parser.h"

#include <QDebug>

FirstOfNodeFactory::FirstOfNodeFactory()
{

}

Node* FirstOfNodeFactory::getNode(const QString &tagContent, Parser *p)
{

  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);
  expr.takeAt(0);

  if (expr.size() <= 0 )
  {
    emit error(Parser::TagSyntaxError, QString("%1 expects at least one argument").arg(expr.at(0)));
  }

  QList<FilterExpression> list;
  foreach(const QString &var, expr)
  {
    FilterExpression fe(var, p);
    list << fe;
  }

  return new FirstOfNode(list);
}


FirstOfNode::FirstOfNode(QList<FilterExpression> list)
{
  m_filterExpressionList = list;
}

QString FirstOfNode::render(Context *c)
{
  foreach(FilterExpression fe, m_filterExpressionList)
  {
    if (fe.isTrue(c))
    {
      return fe.resolve(c).toString();
    }
  }
  return QString();
}

