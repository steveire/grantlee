/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_NODE_H
#define GRANTLEE_NODE_H

// krazy:excludeall=dpointer

#include <QtCore/QStringList>

// Need these for inheriting from QList<T> to work
// http://lists.trolltech.com/qt-interest/2008-01/thread00578-0.html
#include <QtCore/QSet>
#include <QtCore/QVector>

#include "context.h"
#include "filterexpression.h"
#include "outputstream.h"
#include "safestring.h"

#include "grantlee_core_export.h"

namespace Grantlee
{

class NodeList;
class TemplateImpl;

class NodePrivate;

/// @headerfile node.h grantlee/node.h

/**
  @brief Base class for all nodes.

  The Node class can be implemented to make additional functionality available to Templates.

  A node is represented in template markup as content surrounded by percent signed tokens.

  @code
    text content
    {% some_tag arg1 arg2 %}
      text content
    {% some_other_tag arg1 arg2 %}
      text content
    {% end_some_other_tag %}
    text content
  @endcode

  This is parsed into a tree of Node objects by an implementation of AbstractNodeFactory. The Node objects can then later be rendered by their render method.

  Rendering a Node will usually mean writing some output to the stream. The content written to the stream could be determined by the arguments to the tag, or by the content of child nodes between a start and end tag, or both.

  @see FilterExpression
  @see @ref tags

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT Node : public QObject
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
  virtual void render( OutputStream *stream, Context *c ) = 0;

  /**
    Reimplement this to return whether the node is persistent.
    @see mutable_templates
  */
  virtual bool isPersistent() { // krazy:exclude:inline
    return isRepeatable();
  }

  /**
    Reimplement this to return whether the node is repeatable.
    @see mutable_templates
  */
  virtual bool isRepeatable() { // krazy:exclude:inline
    return false;
  }

#ifndef Q_QDOC
  /**
    @internal
  */
  virtual bool mustBeFirst() { // krazy:exclude:inline
    return false;
  }
#endif

protected:
  /**
    Renders the value @p input in the Context @p c. This will involve escaping @p input
    if necessary.

    This is only relevant to developing template tags.
  */
  void streamValueInContext( OutputStream *stream, const QVariant &input, Grantlee::Context *c );

  /**
    Returns a raw pointer to the Template this Node is in.
  */
  TemplateImpl* containerTemplate() const;

private:
  Q_DECLARE_PRIVATE( Node )
  NodePrivate * const d_ptr;
};

/// @headerfile node.h grantlee/node.h

/**
  @brief A list of Nodes with some convenience API for rendering them.

  Typically, tags which have an end tag will create and later render a list of child nodes.

  This class contains API such as append and render to make creating such list easily.

  The findChildren method behaves similarly to the QObject::findChildren method, returning a list of nodes of
  a particular type from the Node objects contained in the list (and their children).

  @see @ref tags_with_end_tags
*/
class GRANTLEE_CORE_EXPORT NodeList : public QList<Grantlee::Node*>
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
  void render( OutputStream *stream, Context *c );

protected:
  /**
    Renders the list of Nodes in the Context @p c, possibly mutating the nodes contained in it.
  */
  void mutableRender( OutputStream *stream, Context *c );

private:
  bool m_containsNonText;
};

class AbstractNodeFactoryPrivate;

/// @headerfile node.h grantlee/node.h

/**
  @brief Base class for all NodeFactories

  This class can be used to make custom tags available to templates.
  The getNode method should be implemented to return a Node to be rendered.

  A node is represented in template markup as content surrounded by percent signed tokens.

  @code
    text content
    {% some_tag arg1 arg2 %}
      text content
    {% some_other_tag arg1 arg2 %}
      text content
    {% end_some_other_tag %}
    text content
  @endcode

  It is the responsibility of an AbstractNodeFactory implementation to process the contents of a tag and return a Node implementation from its getNode method.

  The @ref getNode method would for example be called with the tagContent "some_tag arg1 arg2". That content could then be split up, the arguments processed and a Node created

  @code
    Node* SomeTagFactory::getNode(const QString &tagContent, Parser *p) {
      QStringList parts = smartSplit( tagContent );

      parts.removeFirst(); // Remove the "some_tag" part.

      FilterExpression arg1( parts.at( 0 ), p );
      FilterExpression arg2( parts.at( 1 ), p );

      return new SomeTagNode( arg1, arg2, p );
    }
  @endcode

  The @ref getNode implementation might also advance the parser. For example if we had a <tt>times</tt> tag which rendered content the amount of times it was given in its argument, it could be used like this:

  @code
    Some text content.
    {% times 5 %}
      the bit to be repeated
    {% end_times %}
    End text content
  @endcode

  The argument to <tt>times</tt> might not be a simple number, but could be a FilterExpression such as <tt>someobject.some_property|getDigit:1</tt>.

  The implementation could look like

  @code
    Node* SomeOtherTagFactory::getNode(const QString &tagContent, Parser *p) {
      QStringList parts = smartSplit( tagContent );

      parts.removeFirst(); // Remove the "times" part.

      FilterExpression arg( parts.at( 0 ), p );

      SomeTagNode *node = new SomeTagNode( arg, p );
      NodeList childNodes = p->parse( node, "end_times" );
      node->setChildNodes( childNodes );
      p->removeNextToken();

      return node;
    }
  @endcode

  Note that it is necessary to invoke the parser to create the child nodes only after creating the Node to return. That node must be passed to the Parser to perform as the parent QObject to the child nodes.

  @see Parser::parse
*/
class GRANTLEE_CORE_EXPORT AbstractNodeFactory : public QObject
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
    if the template content is &quot;{% my_tag arg1 arg2 %}&quot;, the tagContent will be &quot;my_tag arg1 arg2&quot;.

    The Parser @p p is available and can be advanced if appropriate. For example, if the tag has an
    end tag, the parser can be advanced to the end tag.
    @see tags
  */
  virtual Node* getNode( const QString &tagContent, Parser *p ) const = 0;

protected:
  /**
    Splits @p str into a list, taking quote marks into account.

    This is typically used in the implementation of getNode with the tagContent.

    If @p str is 'one &quot;two three&quot; four 'five &quot; six' seven', the returned list will contain the following strings:

    - one
    - &quot;two three&quot;
    - four
    - five &quot; six
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
  AbstractNodeFactoryPrivate * const d_ptr;
};

/**
  @internal

  A Node for plain text. Plain text is everything between variables, comments and template tags.
*/
class GRANTLEE_CORE_EXPORT TextNode : public Node
{
  Q_OBJECT
public:
  explicit TextNode( const QString &content, QObject *parent = 0 );

  /* reimp */ void render( OutputStream *stream, Context *c ) { // krazy:exclude:inline
    Q_UNUSED( c );
    ( *stream ) << m_content;
  }

  void appendContent( const QString &content ) { // krazy:exclude:inline
    m_content += content;
  }

private:
  QString m_content;
};

/**
  @internal

  A node for a variable or filter expression substitution.
*/
class GRANTLEE_CORE_EXPORT VariableNode : public Node
{
  Q_OBJECT
public:
  explicit VariableNode( const FilterExpression &fe, QObject *parent = 0 );

  /* reimp */ void render( OutputStream *stream, Context *c );

private:
  FilterExpression m_filterExpression;
};

}

#endif

