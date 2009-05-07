/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef NODE_H
#define NODE_H

#include "context.h"

#include <QStringList>
#include "filter.h"

#include "grantlee_export.h"

// The two sides of template tags:
// Tag and Node. The Tag object creates a Node from a tag.
// The Node object knows how to render itself given the arguments to the tag.

namespace Grantlee
{

/**
Abstract base class for nodes.
*/
class GRANTLEE_EXPORT Node : public QObject
{
  Q_OBJECT
public:
  Node(QObject *parent = 0);
  virtual ~Node();

  virtual QString render(Context *c) = 0;

};


class GRANTLEE_EXPORT NodeList : public QList<Node*>
{
public:
  NodeList()
  : m_containsNonText(false)
  {

  }

  //append changes m_containsNonText?

  QString render(Context *c);

private:
  bool m_containsNonText;
  QStringList m_templateSearchDirs;
};


/**
Base class for tags. Downstreams can use this to create custom tags.
This can also be inherited by qtscript objects to define tags in script.

Each tag is essentially a factory class for the type of node it creates.

Maybe this should be AbstractNodeFactory ... IfNodeFactory.

Users implement AbstractNodeFactory to create new Nodes. and register their factory with the library?

Alternatively I could use a prototype pattern?
*/
class GRANTLEE_EXPORT AbstractNodeFactory : public QObject
{
  Q_OBJECT
public:
  virtual ~AbstractNodeFactory();

  /**
  Advances the parser as needed for block style tags.
  */
  virtual Node* getNode(const QString &tagContent, Parser *p) = 0;

protected:
  QStringList smartSplit(const QString &str);

signals:
  void error(int type, const QString &message);

};

class GRANTLEE_EXPORT TextNode : public Node
{
public:
  TextNode(const QString &content)
  {
    m_content = content;
  }

  QString render(Context *c)
  {
    Q_UNUSED(c);
    return m_content;
  }
private:
  QString m_content;
};

class GRANTLEE_EXPORT VariableNode : public Node
{
public:
  VariableNode(const FilterExpression &fe);

  QString render(Context *c);

private:
  FilterExpression m_filterExpression;

};

// class ErrorNode : pubic Node
// {
//
// };


// TODO: figure out if I can use the same QMetaType tricks described in qt:qtscript for QPoint.
// Define a macro to wrap non-qobject cpp classes.

}

#endif
