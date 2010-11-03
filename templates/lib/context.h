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

#ifndef GRANTLEE_CONTEXT_H
#define GRANTLEE_CONTEXT_H

#include "abstractlocalizer.h"
#include "grantlee_core_export.h"

#include <QtCore/QVariantHash>

namespace Grantlee
{

class RenderContext;

class ContextPrivate;

/// @headerfile context.h grantlee/context.h

/**
  @brief The Context class holds the context to render a template with.

  For application developers, using the Context class is a matter of inserting keys and
  values as appropriate for rendering a template using the insert method.

  @code
    Template t = engine->newTemplate( "Name is {% name %} and age is {% age %}.", "some_template" );

    Context c1;
    c1.insert( "name", "Tom" );
    c1.insert( "age", 34 );

    Context c2;
    c2.insert( "name", "Harry" );
    c2.insert( "age", 43 );

    t->render(c1); // Returns "Name is Tom and age is 43."
    t->render(c2); // Returns "Name is Harry and age is 34."
  @endcode

  Note that one Template may be rendered multiple times with different contexts. Note also that any QVariant may be inserted
  into a Context object. Most commonly, QObjects will be used here.
  @see @ref custom_objects

  @section context_stack Context Stack.

  For template tag developers, some other Context API is relevant.

  It is possible to push and pop layers of context while a template is being rendered. This is useful if your template
  tag makes additional variables temporarily available in a part of a template. Template tags should only modify layers of context
  that they push themselves, and should pop any layers created before finishing its rendering step.

  See for example the @gr_tag{with} tag. In a template such as
  @code
    Some content
    {% with person.name|toUpper as lowerName %}
      Name is {% lowerName %}
    {% endwith %}
  @endcode

  In this case, lowerName is available in the context only between the @gr_tag{with} and @gr_tag{endwith} tags. The implementation of
  the @gr_tag{with} tag render method is:

  @code
    void WithNode::render( OutputStream *stream, Context *c )
    {
      c->push();
      // {% with m_filterExpression as m_name %}
      c->insert( m_name, m_filterExpression.resolve( c ) );
      m_list.render( stream, c );
      c->pop(); // The section of context defining m_name is removed.
    }
  @endcode

  Note that a context may temporarily override a variable in a parent context. This is why it is important to push a new context when
  adding items to context and pop it when finished.

  @code
    Some content
    {% with "foo" as var %}
      Var is {% var %}         // Var is "foo"
      {% with "bar" as var %}
        Var is {% var %}       // Var is "bar"
      {% endwith %}
      Var is {% var %}         // Var is "foo"
    {% endwith %}
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT Context
{
public:

  /**
    Creates an empty context
  */
  Context();
  /**
    Sets every key in the hash as a property name with the variant as the value.
  */
  explicit Context( const QVariantHash &hash );


  /**
    Copy Constructor
  */
  Context( const Context &other );

  /**
    Assignmant operator
  */
  Context& operator =( const Context &other );

#ifndef Q_QDOC
  /**
    @internal

    Whether to automatically escape all context content. This is not usually used directly. Use the @gr_tag{autoescape} tag instead.
  */
  bool autoEscape() const;

  /**
    @internal

    Sets whether to automatically escape all context content. This is not usually used directly. Use the @gr_tag{autoescape} tag instead.
  */
  void setAutoEscape( bool autoescape );
#endif
  /**
    Destructor
  */
  ~Context();

  /**
    Returns the context object identified by the key @p str
  */
  QVariant lookup( const QString &str ) const;

  /**
    Insert the context object @p variant identified by @p name into the Context.
  */
  void insert( const QString &name, const QVariant &variant );

  /**
    Pushes a new context.
    @see @ref context_stack
  */
  void push();

  /**
    Pops the context.
    @see @ref context_stack
  */
  void pop();

#ifndef Q_QDOC
  /**
    @internal Returns the context hash at depth @p depth.
  */
  QVariantHash stackHash( int depth ) const;

  /**
    @internal
    Returns whether template being rendered is being mutated.
  */
  bool isMutating() const;

  /**
    @internal
    Sets whether template being rendered is being mutated to @p mutating.
  */
  void setMutating( bool mutating );

  /**
    @internal
  */
  void addExternalMedia( const QString &absolutePart, const QString &relativePart );

  /**
    @internal
  */
  void clearExternalMedia();
#endif

  /**
    Sets the localizer to be used.

    The Context takes ownerwhip of the localizer.
  */
  void setLocalizer( AbstractLocalizer::Ptr localizer );

  /**
    Returns the localizer currently in use.
  */
  AbstractLocalizer::Ptr localizer() const;

  /**
    Returns the external media encountered in the Template while rendering.
  */
  QList<QPair<QString, QString> > externalMedia() const;

  /**
    The type of urls to external media that should be put in the template.
  */
  enum UrlType
  {
    AbsoluteUrls,   ///< Absolute URLs should be put in the template.
    RelativeUrls    ///< Relative URLs should be put in the template.
  };

  /**
    Sets the type of external media URL to be used in the template to @p type.
    @see @ref media_finder_tag
  */
  void setUrlType( UrlType type );

  /**
    The type of URL used in the template.
  */
  UrlType urlType() const;

  /**
    Sets the relative path to external media to be used in templates to @p relativePath
    @see @ref media_finder_tag
  */
  void setRelativeMediaPath( const QString &relativePath );

  /**
    The relative path to external media to be used in templates.
  */
  QString relativeMediaPath() const;

  /**
   * Returns a modifiable RenderContext for the Node @p scopeNode. This may be used to make
   * Template rendering threadsafe so that render state does not need to be stored in the
   * Node implementation itself.
   */
  RenderContext* renderContext() const;

private:
  Q_DECLARE_PRIVATE( Context )
  ContextPrivate * const d_ptr;
};

}

#endif

