/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef IFEQUALNODE_H
#define IFEQUALNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class IfEqualNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfEqualNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);

protected:
  Node* do_getNode(const QString& tagContent, Parser *p, bool negate);
};

class IfNotEqualNodeFactory : public IfEqualNodeFactory
{
  Q_OBJECT
public:
  IfNotEqualNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);
};

class IfEqualNode : public Node
{
  Q_OBJECT
public:
  IfEqualNode(Variable val11, Variable val2, NodeList trueList, NodeList falseList, bool negate);

  QString render(Context *c);

private:
  Variable m_var1;
  Variable m_var2;
  NodeList m_trueList;
  NodeList m_falseList;
  bool m_negate;

};

#endif
