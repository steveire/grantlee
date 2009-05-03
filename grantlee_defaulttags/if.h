/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef IFNODE_H
#define IFNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "grantlee_export.h"

class Parser;


class IfNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);
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
  IfNode(const QString &booleanExpression, NodeList trueList, NodeList falseList);

  QString render(Context *c);

protected:
  bool variantIsTrue( const QVariant &variant );
  QString renderTrueList(Context *c);
  QString renderFalseList(Context *c);

private:
  QString m_booleanExpression;
  QList<QPair<bool, FilterExpression> > m_boolVars;
  NodeList m_trueList;
  NodeList m_falseList;
  int m_linkType;

};

#endif
