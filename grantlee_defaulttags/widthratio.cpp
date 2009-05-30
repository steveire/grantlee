/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "widthratio.h"

#include <QStringList>
#include "parser.h"



WidthRatioNodeFactory::WidthRatioNodeFactory()
{

}

Node* WidthRatioNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  if (expr.size() != 4)
  {
    error(TagSyntaxError, "widthratio takes three arguments");
    return 0;
  }
  FilterExpression valExpr(expr.at(1), p);
  FilterExpression maxExpr(expr.at(2), p);
  FilterExpression maxWidth(expr.at(3), p);

  return new WidthRatioNode(valExpr, maxExpr, maxWidth, parent);
}


WidthRatioNode::WidthRatioNode(FilterExpression valExpr, FilterExpression maxExpr, FilterExpression maxWidth, QObject *parent)
  : Node(parent)
{
  m_valExpr = valExpr;
  m_maxExpr = maxExpr;
  m_maxWidth = maxWidth;
}

int WidthRatioNode::round(qreal number)
{
  int intPart = (int)number;
  if (number < (intPart + 0.5))
    return intPart;
  return intPart + 1;
}

QString WidthRatioNode::render(Context *c)
{
  QVariant thisVal = m_valExpr.resolve(c);
  QVariant maxVal = m_maxExpr.resolve(c);
  if (!thisVal.isValid() || !maxVal.isValid())
    return QString("");

  qreal tv = thisVal.toDouble();
  qreal mv = maxVal.toDouble();

  if (mv == 0)
    return QString("");

  int maxWidth = m_maxWidth.resolve(c).toInt();

  qreal result = (tv / mv) * maxWidth;

  result = round(result);

  return QString::number(result);
}

