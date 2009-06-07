/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef COMMENTNODE_H
#define COMMENTNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;

class CommentNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  CommentNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};


class CommentNode : public Node
{
  Q_OBJECT
public:
  CommentNode( QObject *parent );

  QString render( Context *c );
};

#endif

