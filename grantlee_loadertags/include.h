/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef INCLUDENODE_H
#define INCLUDENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


class Parser;


class IncludeNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IncludeNodeFactory();

  Node* getNode(const QString &tagContent, Parser *p);

};

class IncludeNode : public Node
{
  Q_OBJECT
public:
  IncludeNode(const QString &filename);
  QString render(Context *c);

private:
  FilterExpression m_filterExpression;

};

class ConstantIncludeNode : public Node
{
  Q_OBJECT
public:
  ConstantIncludeNode(const QString &filename);
  QString render(Context *c);

private:
  QString m_name;

};

#endif
