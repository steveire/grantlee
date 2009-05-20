/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef WIDTHRATIONODE_H
#define WIDTHRATIONODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class WidthRatioNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  WidthRatioNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;

};

class WidthRatioNode : public Node
{
  Q_OBJECT
public:
  WidthRatioNode(FilterExpression valExpr, FilterExpression maxExpr, int maxWidth, QObject *parent);

  QString render(Context *c);

private:
  int round(qreal);

  FilterExpression m_valExpr;
  FilterExpression m_maxExpr;
  int m_maxWidth;

};

#endif
