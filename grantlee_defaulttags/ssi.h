/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef SSINODE_H
#define SSINODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class SsiNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  SsiNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);
};

class SsiNode : public Node
{
  Q_OBJECT
public:
  SsiNode(const QString &filename, bool parse);

  QString render(Context *c);

private:
  QString m_filename;
  bool m_parse;

};

#endif
