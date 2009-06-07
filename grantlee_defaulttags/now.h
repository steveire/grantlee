/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef NOWNODE_H
#define NOWNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class NowNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  NowNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;
};

class NowNode : public Node
{
  Q_OBJECT
public:
  NowNode( const QString &formatString, QObject *parent );

  QString render( Context *c );

private:
  QString m_formatString;

};

#endif
