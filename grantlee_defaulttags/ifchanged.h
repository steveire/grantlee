/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef IFCHANGEDNODE_H
#define IFCHANGEDNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class IfChangedNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfChangedNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;
};

class IfChangedNode : public Node
{
  Q_OBJECT
public:
  IfChangedNode( NodeList trueList, NodeList falseList, QList<FilterExpression> feList, QObject *parent );

  QString render( Context *c );

  NodeList getNodesByType( const char * className );

private:
  NodeList m_trueList;
  NodeList m_falseList;
  QList<FilterExpression> m_filterExpressions;
  QVariant m_lastSeen;
  QString m_id;

};

#endif
