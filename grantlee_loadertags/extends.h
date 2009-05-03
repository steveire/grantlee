/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef EXTENDSNODE_H
#define EXTENDSNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class ExtendsNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ExtendsNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);

};

class ExtendsNode : public Node
{
  Q_OBJECT
public:
  ExtendsNode(NodeList list, const QString &filename);

  QString render(Context *c);

  void appendNode(Node* node);

private:
  FilterExpression m_filterExpression;
  NodeList m_list;
};

#endif
