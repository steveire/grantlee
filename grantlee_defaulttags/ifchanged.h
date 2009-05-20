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

  Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;
};

class IfChangedNode : public Node
{
  Q_OBJECT
public:
  IfChangedNode(NodeList trueList, NodeList falseList, QStringList vars, QObject *parent);

  QString render(Context *c);

private:
  NodeList m_trueList;
  NodeList m_falseList;
  QList<Variable> m_variables;
  QVariant m_lastSeen;
  QString m_id;

};

#endif
