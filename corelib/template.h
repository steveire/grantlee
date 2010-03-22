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

#ifndef GRANTLEE_TEMPLATE_H
#define GRANTLEE_TEMPLATE_H

#include <QtCore/QStringList>
#include <QtCore/QSharedPointer>

#include "node.h"

#include "global.h"
#include "grantlee_core_export.h"

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

  The result of parsing is a Template object which can be rendered multiple times with multiple different Contexts.

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

