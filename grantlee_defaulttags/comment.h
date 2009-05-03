/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef COMMENTNODE_H
#define COMMENTNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


class Parser;

class CommentNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
    CommentNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p);

};


class CommentNode : public Node
{
  Q_OBJECT
public:
  CommentNode();

  QString render( Context *c );
};

#endif

