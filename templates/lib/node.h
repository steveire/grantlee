/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_NODE_H
#define GRANTLEE_NODE_H

// krazy:excludeall=dpointer

#include "context.h"
#include "filterexpression.h"
#include "grantlee_templates_export.h"
#include "outputstream.h"
#include "safestring.h"

#include <QtCore/QStringList>

// Need these for inheriting from QList<T> to work
// http://lists.trolltech.com/qt-interest/2008-01/thread00578-0.html
#include <QtCore/QSet>
#include <QtCore/QVector>

namespace Grantlee
{

class Engine;
class NodeList;
class TemplateImpl;

class NodePrivate;

/// @headerfile node.h grantlee/node.h

/**
  @brief Base class for all nodes.

  The **%Node** class can be implemented to make additional functionality
  available to Templates.

  A node is represented in template markup as content surrounded by percent
  signed tokens.

  @code
    text content
    {% some_tag arg1 arg2 %}
      text content
    {% some_other_tag arg1 arg2 %}
      text content
    {% end_some_other_tag %}
    text content
  @endcode

  This is parsed into a tree of **%Node** objects by an implementation of
  AbstractNodeFactory. The **%Node** objects can then later be rendered by their
  @ref render method.

  Rendering a **%Node** will usually mean writing some output to the stream. The
  content written to the stream could be determined by the arguments to the tag,
  or by the content of child nodes between a start and end tag, or both.

  @see FilterExpression
  @see @ref tags

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TEMPLATES_EXPORT Node : public QObject
{
  Q_OBJECT
public:
  /**
    Constructor.

    @param parent The parent QObject
  */
  explicit Node(QObject *parent = {});

  /**
    Destructor.
  */
  ~Node() override;

  /**
    Reimplement this to render the template in the Context @p c.

    This will also involve calling render on and child nodes.
  */
  virtual void render(OutputStream *stream, Context *c) const = 0;

#ifndef Q_QDOC
  /**
    @internal
  */
  virtual bool mustBeFirst()
  { // krazy:exclude:inline
    return false;
  }
#endif

protected:
  /**
    Renders the value @p input in the Context @p c. This will involve escaping
    @p input if necessary.

    This is only relevant to developing template tags.
  */
  void streamValueInContext(OutputStream *stream, const QVariant &input,
                            Grantlee::Context *c) const;

  /**
    Returns a raw pointer to the Template this **%Node** is in.
  */
  TemplateImpl *containerTemplate() const;

private:
  Q_DECLARE_PRIVATE(Node)
  NodePrivate *const d_ptr;
};

/// @headerfile node.h grantlee/node.h

/**
  @brief A list of Nodes with some convenience API for rendering them.

  Typically, tags which have an end tag will create and later render a list of
  child nodes.

  This class contains API such as @ref append and @ref render to make creating
  such list easily.

  The @ref findChildren method behaves similarly to the QObject::findChildren
  method, returning a list of nodes of a particular type from the Node objects
  contained in the list (and their children).

  @see @ref tags_with_end_tags
*/
class GRANTLEE_TEMPLATES_EXPORT NodeList : public QList<Grantlee::Node *>
{
public:
  /**
    Creates an empty **%NodeList**.
  */
  NodeList();

  /**
    Copy constructor.
  */
  NodeList(const NodeList &list);

  NodeList &operator=(const NodeList &list);

  /**
    Convenience constructor
  */
  /* implicit */ NodeList(const QList<Grantlee::Node *> &list);

  /**
    Destructor.
  */
  ~NodeList();

  /**
    Appends @p node to the end of this **%NodeList**.
  */
  void append(Grantlee::Node *node);

  /**
    Appends @p nodeList to the end of this **%NodeList**.
  */
  void append(QList<Grantlee::Node *> nodeList);

  /**
    Returns true if this **%NodeList** contains non-text nodes.
  */
  bool containsNonText() const;

  /**
    A recursive listing of nodes in this tree of type @p T.
  */
  template <typename T> QList<T> findChildren()
  {
    QList<T> children;
    QList<Grantlee::Node *>::const_iterator it;
    const QList<Grantlee::Node *>::const_iterator first = constBegin();
    const QList<Grantlee::Node *>::const_iterator last = constEnd();
    for (it = first; it != last; ++it) {
      T object = qobject_cast<T>(*it);
      if (object) {
        children << object;
      }
      children << (*it)->findChildren<T>();
    }
    return children;
  }

  /**
    Renders the list of Nodes in the Context @p c.
  */
  void render(OutputStream *stream, Context *c) const;

private:
  bool m_containsNonText;
};

class AbstractNodeFactoryPrivate;

/// @headerfile node.h grantlee/node.h

/**
  @brief Base class for all NodeFactories

  This class can be used to make custom tags available to templates.
  The getNode method should be implemented to return a Node to be rendered.

  A node is represented in template markup as content surrounded by percent
  signed tokens.

  @code
    text content
    {% some_tag arg1 arg2 %}
      text content
    {% some_other_tag arg1 arg2 %}
      text content
    {% end_some_other_tag %}
    text content
  @endcode

  It is the responsibility of an **%AbstractNodeFactory** implementation to
  process the contents of a tag and return a Node implementation from its
  getNode method.

  The @ref getNode method would for example be called with the tagContent
  \"<tt>some_tag arg1 arg2</tt>\". That content could then be split up, the
  arguments processed and a Node created

  @code
    Node* SomeTagFactory::getNode(const QString &tagContent, Parser *p) {
      QStringList parts = smartSplit( tagContent );

      parts.removeFirst(); // Remove the "some_tag" part.

      FilterExpression arg1( parts.first(), p );
      FilterExpression arg2( parts.at( 1 ), p );

      return new SomeTagNode( arg1, arg2, p );
    }
  @endcode

  The @ref getNode implementation might also advance the parser. For example if
  we had a @gr_tag{times} tag which rendered content the amount of times it was
  given in its argument, it could be used like this:

  @code
    Some text content.
    {% times 5 %}
      the bit to be repeated
    {% end_times %}
    End text content
  @endcode

  The argument to @gr_tag{times} might not be a simple number, but could be a
  FilterExpression such as \"<tt>someobject.some_property|getDigit:1</tt>\".

  The implementation could look like

  @code
    Node* SomeOtherTagFactory::getNode(const QString &tagContent, Parser *p) {
      QStringList parts = smartSplit( tagContent );

      parts.removeFirst(); // Remove the "times" part.

      FilterExpression arg( parts.first(), p );

      auto node = new SomeTagNode( arg, p );
      auto childNodes = p->parse( node, "end_times" );
      node->setChildNodes( childNodes );
      p->removeNextToken();

      return node;
    }
  @endcode

  Note that it is necessary to invoke the parser to create the child nodes only
  after creating the Node to return. That node must be passed to the Parser to
  perform as the parent QObject to the child nodes.

  @see Parser::parse
*/
class GRANTLEE_TEMPLATES_EXPORT AbstractNodeFactory : public QObject
{
  Q_OBJECT
public:
  /**
    Constructor.

    @param parent The parent QObject
  */
  explicit AbstractNodeFactory(QObject *parent = {});

  /**
    Destructor.
  */
  ~AbstractNodeFactory() override;

  /**
    This method should be reimplemented to return a Node which can be
    rendered.

    The @p tagContent is the content of the tag including the tag name and
    arguments. For example, if the template content is @gr_tag{my_tag arg1
    arg2}, the tagContent will be &quot;my_tag arg1 arg2&quot;.

    The Parser @p p is available and can be advanced if appropriate. For
    example, if the tag has an end tag, the parser can be advanced to the end
    tag.

    @see tags
  */
  virtual Node *getNode(const QString &tagContent, Parser *p) const = 0;

#ifndef Q_QDOC
  /**
    @internal

    Sets the Engine which created this NodeFactory. Used by the
    ScriptableNodeFactory.
  */
  virtual void setEngine(Engine *) {}
#endif

protected:
  /**
    Splits @p str into a list, taking quote marks into account.

    This is typically used in the implementation of getNode with the
    tagContent.

    If @p str is 'one &quot;two three&quot; four 'five &quot; six' seven', the
    returned list will contain the following strings:

    - one
    - &quot;two three&quot;
    - four
    - five &quot; six
    - seven
  */
  Q_INVOKABLE QStringList smartSplit(const QString &str) const;

protected:
  /**
    Returns a list of FilterExpression objects created with Parser @p p as
    described by the content of @p list.

    This is used for convenience when handling the arguments to a tag.
  */
  QList<FilterExpression> getFilterExpressionList(const QStringList &list,
                                                  Parser *p) const;

private:
  Q_DECLARE_PRIVATE(AbstractNodeFactory)
  AbstractNodeFactoryPrivate *const d_ptr;
};
}

#endif
