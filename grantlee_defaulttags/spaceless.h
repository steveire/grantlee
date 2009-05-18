/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef SPACELESSNODE_H
#define SPACELESSNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class SpacelessNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  SpacelessNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p, QObject *parent);

};

class SpacelessNode : public Node
{
  Q_OBJECT
public:
  SpacelessNode(NodeList nodeList, QObject *parent);

  QString render(Context *c);

private:
  QString stripSpacesBetweenTags(const QString &input);

  NodeList m_nodeList;

};

#endif
