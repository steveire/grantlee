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

    Node* getNode(const QString &tagContent, Parser *p, QObject *parent);

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

  ForNode(QList<QString> loopVars, FilterExpression fe, int reversed, NodeList loopNodeList, NodeList emptyList, QObject *parent);

  QString render(Context *c);

private:
  void insertLoopVariables(Context *c, int listSize, int i);
  QString iterateMap(Context *c, QVariantMap varMap, bool unpack);
  QString renderLoop(Context *c);
  QString handleMapItem(Context *c, QString key, QVariant value, int listSize, int i, bool unpack);

  FilterExpression m_filterExpression;
  QList<QString> m_loopVars;
  NodeList m_loopNodeList;
  int m_isReversed;
};


#endif
