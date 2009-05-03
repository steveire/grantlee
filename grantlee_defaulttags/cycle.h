/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef CYCLENODE_H
#define CYCLENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


class Parser;


class CycleNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  CycleNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);

};

class CycleNode : public Node
{
  Q_OBJECT
public:
  CycleNode(QList<FilterExpression> list, const QString &name = QString());

  QString render(Context *c);

private:
  QList<FilterExpression> m_filterExpressionList;
  QString m_name;
  int m_index;

};

#endif
