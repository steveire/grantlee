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

#ifndef GRANTLEE_TEMPLATE_H
#define GRANTLEE_TEMPLATE_H

#include "exception.h"
#include "grantlee_core_export.h"
#include "node.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

namespace Grantlee
{
class Context;
class Engine;
class TemplateImpl;
class OutputStream;

#ifdef Q_QDOC
#define TemplateImpl Template
#else
typedef QSharedPointer<TemplateImpl> Template;
#endif

class TemplatePrivate;

/// @headerfile template.h grantlee/template.h

/**
  @brief The Template class is a tree of nodes which may be rendered.

  All Templates are created through the Grantlee::Engine class.
  A template is created by parsing some text markup passed into the Engine, or by
  reading it from a file.

  Note that Template is actually a typedef for a QSharedPointer<TemplateImpl>, so all of its members should be accessed with operator->().

  The result of parsing is a Template object which can be rendered multiple times with multiple different Contexts.

  @code
    Engine *engine = getEngine();
    Template t = engine->newTemplate( "{{ name }} is aged {{ age }}", "simple template" );
    if ( t->error() )
    {
      // Tokenizing or parsing error, or couldn't find custom tags or filters.
      qDebug() << t->errorString();
      return;
    }
    QTextStream textStream( stdout );
    OutputStream stream( textStream );

    for ( ... )
    {
      Context c;
      // ... c.insert
      t->render( stream, c );

      if (t->error())
      {
        // Rendering error.
        qDebug() << t->errorString();
      }
    }
  @endcode

  If there is an error in parsing or rendering, the error and errorString methods can be used to check the source of the error.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT TemplateImpl : public QObject
{
  Q_OBJECT
public:
  ~TemplateImpl();

  /**
    Renders the template to a string given the Context @p c.
  */
  virtual QString render( Context *c );

  /**
    Renders the Template to the OutputStream @p stream given the Context c.
  */
  // ### BIC make this const and non-virtual.
  virtual OutputStream* render( OutputStream *stream, Context *c );

#ifndef Q_QDOC
  /**
    @internal
  */
  NodeList nodeList() const;

  /**
    @internal
  */
  void setNodeList( const NodeList &list );
#endif

  /**
    Returns an error code for the error encountered.
  */
  Error error();

  /**
    Returns more information to developers in the form of a string.
  */
  QString errorString();

  /**
    Returns the Engine that created this Template.
  */
  Engine const * engine() const;

#ifndef Q_QDOC
protected:
  TemplateImpl( Engine const *engine, QObject *parent = 0 );

  void setContent( const QString &templateString );
#endif

private:
  //Don't allow setting the parent on a Template, which is memory managed as a QSharedPointer.
  using QObject::setParent;

private:
  Q_DECLARE_PRIVATE( Template )
  TemplatePrivate * const d_ptr;
#ifndef Q_QDOC
  friend class Engine;
  friend class Parser;
#endif
};

}

Q_DECLARE_METATYPE( Grantlee::Template )

#endif

