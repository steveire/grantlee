/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef REGROUPNODE_H
#define REGROUPNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class RegroupNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  RegroupNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);

};

class RegroupNode : public Node
{
  Q_OBJECT
public:
  RegroupNode(FilterExpression target, FilterExpression expression, const QString &varName);

  QString render(Context *c);

private:
  FilterExpression m_target;
  FilterExpression m_expression;
  QString m_varName;

};

#endif
