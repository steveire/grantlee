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

#ifndef GRANTLEE_CONTEXT_H
#define GRANTLEE_CONTEXT_H

#include <QtCore/QVariantHash>

#include "grantlee_core_export.h"

namespace Grantlee
{

class ContextPrivate;

/**
  @brief The Context class holds the context to render a template with.

  For application developers, using the Context class is a matter of inserting keys and
  values as appropriate for rendering a template using the insert method.

  @code
    Template t = Engine::instance()->newTemplate( "Name is {% name %} and age is {% age %}.", "some_template" );

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
  @see custom_objects

  @section context_stack Context Stack.

  For template tag developers, some other Context API is relevant.

  It is possible to push and pop layers of context while a template is being rendered. This is useful if your template
  tag makes additional variables temporarily available in a part of a template. Template tags should only modify layers of context
  that they push themselves, and should pop any layers created before finishing its rendering step.

  See for example the "{% with %}" tag. In a template such as
  @code
    Some content
    {% with person.name|toUpper as lowerName %}
      Name is {% lowerName %}
    {% endwith %}
  @endcode

  In this case, lowerName is available in the context only between the "{% with %}" and "{% endwith %}" tags. The implementation of
  the "{% with %}" tag render method is:

  @code
    QString WithNode::render( Context *c )
    {
      c->push();
      // {% with m_filterExpression as m_name %}
      c->insert( m_name, m_filterExpression.resolve( c ) );
      QString ret = m_list.render( c );
      c->pop(); // The section of context defining m_name is removed.
      return ret;
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
    @internal

    Whether to automatically escape all context content. This is not usually used directly. Use the "{% autoescape %}" tag instead.
  */
  bool autoescape() const;

  /**
    @internal

    Sets whether to automatically escape all context content. This is not usually used directly. Use the "{% autoescape %}" tag instead.
  */
  void setAutoescape( bool autoescape );

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
    @see context_stack
  */
  void push();

  /**
    Pops the context.
    @see context_stack
  */
  void pop();

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

private:
  Q_DECLARE_PRIVATE( Context )
  ContextPrivate * const d_ptr;
};

}

#endif
