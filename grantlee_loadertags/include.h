/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef INCLUDENODE_H
#define INCLUDENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
}


using namespace Grantlee;

class IncludeNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IncludeNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};

class IncludeNode : public Node
{
  Q_OBJECT
public:
  IncludeNode( const FilterExpression &fe, QObject *parent );
  QString render( Context *c );

private:
  FilterExpression m_filterExpression;

};

class ConstantIncludeNode : public Node
{
  Q_OBJECT
public:
  ConstantIncludeNode( const QString &filename, QObject *parent );
  QString render( Context *c );

private:
  QString m_name;

};

#endif
