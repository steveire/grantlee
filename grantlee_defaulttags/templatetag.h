/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TEMPLATETAGNODE_H
#define TEMPLATETAGNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "grantlee_export.h"


using namespace Grantlee;


class TemplateTagNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  TemplateTagNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;
};

class TemplateTagNode : public Node
{
  Q_OBJECT
public:

  /**
  The expression.
  Nodes to render if the expression is true
  Nodes to render if the expression is false
  */
  TemplateTagNode( const QString &tagName, QObject *parent );

  QString render( Context *c );

  static bool isKeyword( const QString &name );

private:
  QString m_name;
};

#endif
