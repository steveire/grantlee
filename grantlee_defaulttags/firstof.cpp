/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "firstof.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"


FirstOfNodeFactory::FirstOfNodeFactory()
{
}

Node* FirstOfNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  expr.takeAt(0);

  if (expr.size() <= 0 )
  {
    emit error(TagSyntaxError, QString("%1 expects at least one argument").arg("expr.at(0)"));
    return 0;
  }

  return new FirstOfNode(getFilterExpressionList(expr, p), parent);
}


FirstOfNode::FirstOfNode(QList<FilterExpression> list, QObject *parent)
  : Node(parent), m_variableList(list)
{
}

QString FirstOfNode::render(Context *c)
{
  foreach(FilterExpression fe, m_variableList)
  {
    if (fe.isTrue(c))
    {
      return fe.resolve(c).toString();
    }
  }
  return QString("");
}

