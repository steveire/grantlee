/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef FORNODE_H
#define FORNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class ForNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
  public:
    ForNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p);

};


class ForNode : public Node
{
  Q_OBJECT
public:
  enum Reversed
  {
    IsNotReversed,
    IsReversed
  };

  ForNode(QList<QString> loopVars, FilterExpression fe, int reversed, NodeList loopNodeList, NodeList emptyList = NodeList());

  QString render(Context *c);

private:
  FilterExpression m_filterExpression;
  QList<QString> m_loopVars;
  NodeList m_loopNodeList;
  int m_isReversed;
};


#endif
