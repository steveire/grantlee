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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QStringList>
#include <QSharedPointer>

#include "node.h"
#include "grantlee_export.h"
#include "global.h"
#include "enginestate.h"

namespace Grantlee
{
class Context;
class TemplateImpl;

typedef QWeakPointer<TemplateImpl> TemplateWeakPtr;
typedef QSharedPointer<TemplateImpl> Template;

class TemplatePrivate;

class GRANTLEE_EXPORT TemplateImpl : public QObject
{
  Q_OBJECT
public:
  ~TemplateImpl();
  virtual QString render( Context *c );

  NodeList nodeList() const;

  void setNodeList( const NodeList &list );

  Error error();
  QString errorString();

  EngineState state() const;

protected:
  TemplateImpl( QObject *parent = 0 );

  void setContent( const QString &templateString );

private:
  Q_DECLARE_PRIVATE( Template )
  TemplatePrivate *d_ptr;
  friend class Engine;
};

}

Q_DECLARE_METATYPE(Grantlee::Template)

#endif

