/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef WITHNODE_H
#define WITHNODE_H

#include "node.h"

using namespace Grantlee;

class WithNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
    WithNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;

};

class WithNode : public Node
{
  Q_OBJECT
public:
  WithNode(FilterExpression, const QString &name, NodeList nodeList, QObject *parent );

  QString render(Context *c);

  NodeList getNodesByType(const char * className);

private:
  FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;

};

#endif
