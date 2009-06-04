/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "firstof.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"

#include "util_p.h"


FirstOfNodeFactory::FirstOfNodeFactory()
{
}

Node* FirstOfNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  expr.takeAt(0);

  if (expr.size() <= 0 )
  {
    setError(TagSyntaxError, QString("%1 expects at least one argument").arg("expr.at(0)"));
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
      return Util::getSafeString(fe.resolve(c)).rawString();
    }
  }
  return QString();
}

