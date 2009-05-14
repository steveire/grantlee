/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef DEBUGNODE_H
#define DEBUGNODE_H

#include "node.h"

using namespace Grantlee;

class DebugNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
    DebugNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p);

};

class DebugNode : public Node
{
  Q_OBJECT
public:
  DebugNode();

  QString render(Context *c);

private:
  FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;

};

#endif
