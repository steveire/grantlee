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

    Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;

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

  ForNode(QStringList loopVars, FilterExpression fe, int reversed, NodeList loopNodeList, NodeList emptyList, QObject *parent);

  QString render(Context *c);

  NodeList getNodesByType(const char * className);

private:
  void insertLoopVariables(Context *c, int listSize, int i);
  QString iterateMap(Context *c, QVariantMap varMap, bool unpack);
  QString renderLoop(Context *c);
  QString handleMapItem(Context *c, QString key, QVariant value, int listSize, int i, bool unpack);

  FilterExpression m_filterExpression;
  QStringList m_loopVars;
  NodeList m_loopNodeList;
  NodeList m_emptyNodeList;
  int m_isReversed;
};


#endif
