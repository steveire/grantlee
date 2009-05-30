/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef IFNODE_H
#define IFNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "grantlee_export.h"


using namespace Grantlee;


class IfNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;
};

class IfNode : public Node
{
  Q_OBJECT
public:
  enum LinkType
  {
    OrLink,
    AndLink
  };

  /**
  The expression.
  Nodes to render if the expression is true
  Nodes to render if the expression is false
  */
  IfNode(QList<QPair<bool, FilterExpression > > boolVars, NodeList trueList, NodeList falseList, int linkType, QObject *parent);

  QString render(Context *c);

  NodeList getNodesByType(const char * className);

protected:
  QString renderTrueList(Context *c);
  QString renderFalseList(Context *c);

private:
  QList<QPair<bool, FilterExpression > > m_boolVars;
  NodeList m_trueList;
  NodeList m_falseList;
  int m_linkType;

};

#endif
