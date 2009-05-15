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
class Template;
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
  ExtendsNode(NodeList list, const QString &filename, FilterExpression fe);

  QString render(Context *c);

  void appendNode(Node* node);

  Template *getParent(Context *c);

private:
  FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;
};

#endif
