/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "firstof.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"

#include <QDebug>

FirstOfNodeFactory::FirstOfNodeFactory()
{
}

Node* FirstOfNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = smartSplit(tagContent);

  expr.takeAt(0);

  if (expr.size() <= 0 )
  {
    emit error(TagSyntaxError, QString("%1 expects at least one argument").arg("expr.at(0)"));
    return 0;
  }

  QList<Variable> list;
  foreach(const QString &var, expr)
  {
    Variable v(var);
    list << v;
  }

  return new FirstOfNode(list);
}


FirstOfNode::FirstOfNode(QList<Variable> list)
{
  m_variableList = list;
}

QString FirstOfNode::render(Context *c)
{
  foreach(Variable v, m_variableList)
  {
    if (v.isTrue(c))
    {
      return v.resolve(c).toString();
    }
  }
  return QString("");
}

