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

#include "grantlee_export.h"

namespace Grantlee
{

class NodeList;

class NodePrivate;

/**
  @brief Base class for all nodes.

  The Node class can be implemented to make additional functionality available to Templates.
*/
class GRANTLEE_EXPORT Node : public QObject
{
  Q_OBJECT
public:
  /**
    Constructor.
  */
  explicit Node( QObject *parent = 0 );

  /**
    Destructor.
  */
  virtual ~Node();

  /**
    Reimplement this to render the template in the Context @p c.

    This will also involve calling render on and child nodes.
  */
  // This can't be const because CycleNode needs to change on each render.
  virtual QString render( Context *c ) = 0;

  /**
    Reimplement this to return whether the node is persistent.
    @see mutable_templates
  */
  virtual bool isPersistent() {
    return isRepeatable();
  }

  /**
    Reimplement this to return whether the node is repeatable.
    @see mutable_templates
  */
  virtual bool isRepeatable() {
    return false;
  }

  /**
    @internal
  */
  virtual bool mustBeFirst() {
    return false;
  }

protected:
  /**
    Renders the value @p input in the Context @p c. This will involve escaping @p input
    if neccessary.

    This is only relevant to developing template tags.
  */
  QString renderValueInContext( const QVariant &input, Grantlee::Context *c );

private:
  Q_DECLARE_PRIVATE( Node )
  NodePrivate *d_ptr;
};

/**
  @brief A list of Nodes with some convenience API for rendering them.

*/
class GRANTLEE_EXPORT NodeList : public QList<Grantlee::Node*>
{
public:
  /**
    Creates an empty NodeList.
  */
  NodeList();

  /**
    Copy constructor.
  */
  NodeList( const NodeList &list );

  /**
    Convenience constructor
  */
  /* implicit */ NodeList( const QList<Grantlee::Node *> &list );

  /**
    Destructor.
  */
  ~NodeList();

  /**
    Appends @p node to the end of this NodeList.
  */
  void append( Grantlee::Node* node );

  /**
    Appends @p nodeList to the end of this NodeList.
  */
  void append( QList<Grantlee::Node*> nodeList );

  /**
    Returns true if this NodeList contains non-text nodes.
  */
  bool containsNonText() const;

  /**
    A recursive listing of nodes in this tree of type @p T.
  */
  template <typename T>
  QList<T> findChildren() {
    QList<T> children;
    QList<Grantlee::Node*>::const_iterator it;
    const QList<Grantlee::Node*>::const_iterator first = constBegin();
    const QList<Grantlee::Node*>::const_iterator last = constEnd();
    for ( it = first; it != last; ++it ) {
      T *object = qobject_cast<T>( *it );
      if ( object ) {
        children << object;
      }
      children << object->findChildren<T>();
    }
    return children;
  }

  /**
    Renders the list of Nodes in the Context @p c.
  */
  QString render( Context *c );

protected:
  /**
    Renders the list of Nodes in the Context @p c, possibly mutating the nodes contained in it.
  */
  QString mutableRender( Context *c );

private:
  bool m_containsNonText;
};

class AbstractNodeFactoryPrivate;

/**
  @brief Base class for all NodeFactories

  This class can be used to make custom tags available to templates.
  The getNode method should be implemented to return a Node to be rendered.

*/
class GRANTLEE_EXPORT AbstractNodeFactory : public QObject
{
  Q_OBJECT
public:
  /**
    Constructor.
  */
  explicit AbstractNodeFactory( QObject* parent = 0 );

  /**
    Destructor.
  */
  virtual ~AbstractNodeFactory();

  /**
    This method should be reimplemented to return a Node which can be rendered.

    @p tagContent is the content of the tag including the tag name and arguments. For example,
    if the template content is "{% my_tag arg1 arg2 %}", the tagContent will be "my_tag arg1 arg2".

    The Parser @p p is available and can be advanced if appropriate. For example, if the tag has an
    end tag, the parser can be advanced to the end tag.
    @see tags
  */
  virtual Node* getNode( const QString &tagContent, Parser *p ) const = 0;

protected:
  /**
    Splits @p str into a list, taking quote marks into account.

    This is typically used in the implementation of getNode with the tagContent.

    If @p str is 'one "two three" four 'five " six' seven', the returned list will contain the following strings:

    - one
    - "two three"
    - four
    - five " six
    - seven
  */
  Q_INVOKABLE QStringList smartSplit( const QString &str ) const;

protected:
  /**
    Returns a list of FilterExpression objects as described by the content of @p list.

    This is used for convenience when handling the arguments to a tag.
  */
  QList<FilterExpression> getFilterExpressionList( const QStringList &list, Parser *p ) const;

private:
  Q_DECLARE_PRIVATE( AbstractNodeFactory )
  AbstractNodeFactoryPrivate *d_ptr;
};

/**
  @internal

  A Node for plain text. Plain text is everything between variables, comments and template tags.
*/
class GRANTLEE_EXPORT TextNode : public Node
{
  Q_OBJECT
public:
  explicit TextNode( const QString &content, QObject *parent = 0 );

  /* reimp */ QString render( Context *c ) {
    Q_UNUSED( c );
    return m_content;
  }

  void appendContent( const QString &content ) {
    m_content += content;
  }

private:
  QString m_content;
};

/**
  @internal

  A node for a variable or filter expression substitution.
*/
class GRANTLEE_EXPORT VariableNode : public Node
{
  Q_OBJECT
public:
  explicit VariableNode( const FilterExpression &fe, QObject *parent = 0 );

  /* reimp */ QString render( Context *c );

private:
  FilterExpression m_filterExpression;

};

}

#endif
