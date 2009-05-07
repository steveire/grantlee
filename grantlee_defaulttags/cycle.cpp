/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "cycle.h"

#include <QStringList>
#include "parser.h"

#include <QDebug>

CycleNodeFactory::CycleNodeFactory()
{

}

Node* CycleNodeFactory::getNode(const QString &tagContent, Parser *p)
{

  // be different here? put the cycle value into the context? Otherwise I could use a QObject::property if necesary.

  QStringList expr = smartSplit(tagContent);
  expr.takeAt(0);

  if (expr.size() < 1 )
  {
    emit error(Parser::TagSyntaxError, QString("%1 expects at least one argument").arg(expr.at(0)));
  }

  QList<FilterExpression> list;
  foreach(const QString &var, expr)
  {
    FilterExpression fe(var, p);
    list << fe;
  }

  return new CycleNode(list);
}


CycleNode::CycleNode(QList<FilterExpression> list, const QString &name)
{
  m_filterExpressionList = list;
  m_name = name;
}

QString CycleNode::render(Context *c)
{
  return QString();
//   foreach(FilterExpression fe, m_filterExpressionList)
//   {
//     QVariant value = fe.resolve(c);
//     if (value.isValid())
//     {
//       return value.toString();
//     }
//   }
//   return QString();
}

