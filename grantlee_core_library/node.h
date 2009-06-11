/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef NODE_H
#define NODE_H

#include "context.h"

#include <QStringList>
#include "filterexpression.h"
#include "safestring.h"

#include "grantlee.h"

#include "grantlee_export.h"

namespace Grantlee
{

class NodeList;

class NodePrivate;

/**
Abstract base class for nodes.
*/
class GRANTLEE_EXPORT Node : public QObject
{
  Q_OBJECT
public:
  Node( QObject *parent = 0 );
  virtual ~Node();

  // This can't be const because CycleNode needs to change on each render.
  virtual QString render( Context *c ) = 0;

  virtual NodeList getNodesByType( const char * className );

  Error error() const;
  QString errorString() const;

  virtual bool isPersistent() { return isRepeatable(); }
  virtual bool isRepeatable() { return false; }

protected:
  void setError( Error type, const QString &message );
  QString renderValueInContext( const QVariant &input, Grantlee::Context *c );

private:
  Q_DECLARE_PRIVATE( Node )
  NodePrivate *d_ptr;
};

class GRANTLEE_EXPORT NodeList : public QList<Grantlee::Node*>
{
public:
  NodeList();
  NodeList( const NodeList &list );
  NodeList( const QList<Grantlee::Node *> &list );
  ~NodeList();

  NodeList getNodesByType( const char * className );

  //append changes m_containsNonText?

  QString render( Context *c );

  Error error() const;
  QString errorString() const;

protected:
  void setError( Error type, const QString &message );

  QString mutableRender(Context *c);

private:
  Error m_error;
  QString m_errorString;
  bool m_containsNonText;
};

class AbstractNodeFactoryPrivate;

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
  AbstractNodeFactory( QObject* parent = 0 );
  virtual ~AbstractNodeFactory();

  /**
  Advances the parser as needed for block style tags.
  */
  virtual Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const = 0;

  Error error() const;
  QString errorString() const;

protected:
  Q_INVOKABLE QStringList smartSplit( const QString &str ) const;

protected:
  QList<Variable> getVariableList( const QStringList &list ) const;
  QList<FilterExpression> getFilterExpressionList( const QStringList &list, Parser *p ) const;

  // Needs to be const so it can be called from const methods.
  void setError( Error type, const QString &message ) const;

private:
  Q_DECLARE_PRIVATE( AbstractNodeFactory )
  AbstractNodeFactoryPrivate *d_ptr;
};

class GRANTLEE_EXPORT TextNode : public Node
{
  Q_OBJECT
public:
  TextNode( const QString &content, QObject *parent = 0 );

  QString render( Context *c ) {
    Q_UNUSED( c );
    return m_content;
  }

  void appendContent(const QString &content) { m_content += content; }

private:
  QString m_content;
};

class GRANTLEE_EXPORT VariableNode : public Node
{
  Q_OBJECT
public:
  VariableNode( const FilterExpression &fe, QObject *parent = 0 );

  QString render( Context *c );

private:
  FilterExpression m_filterExpression;

};

// TODO: figure out if I can use the same QMetaType tricks described in qt:qtscript for QPoint.
// Define a macro to wrap non-qobject cpp classes.

}

#endif

