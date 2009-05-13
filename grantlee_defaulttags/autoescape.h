/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef AUTOESCAPENODE_H
#define AUTOESCAPENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class AutoescapeNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  AutoescapeNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);
};

class AutoescapeNode : public Node
{
  Q_OBJECT
public:

  enum State
  {
    On,
    Off
  };

  AutoescapeNode(int state, NodeList list);

  QString render(Context *c);

private:
  NodeList m_list;
  int m_state;

};

#endif
