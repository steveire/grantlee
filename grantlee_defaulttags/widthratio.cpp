/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "widthratio.h"

#include <QStringList>
#include "parser.h"


#include <QDebug>

WidthRatioNodeFactory::WidthRatioNodeFactory()
{

}

Node* WidthRatioNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = smartSplit(tagContent);

  if (expr.size() != 4)
  {
    error(TagSyntaxError, "widthratio takes three arguments");
    return 0;
  }
  FilterExpression valExpr(expr.at(1));
  FilterExpression maxExpr(expr.at(2));
  QVariant v_w(expr.at(3));
  if (!v_w.convert(QVariant::Int))
  {
    error(TagSyntaxError, "widthratio final argument must be an integer");
    return 0;
  }
  int width = v_w.toInt();

  return new WidthRatioNode(valExpr, maxExpr, width);
}


WidthRatioNode::WidthRatioNode(FilterExpression valExpr, FilterExpression maxExpr, int maxWidth)
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

  qreal result = (tv / mv) * m_maxWidth;

  result = round(result);

  return QString::number(result);
}

