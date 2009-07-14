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
#include <QUuid>

#include "node.h"
#include "grantlee_export.h"
#include "grantlee.h"

namespace Grantlee
{
class Context;
}

namespace Grantlee
{

class TemplatePrivate;

class GRANTLEE_EXPORT Template : public QObject
{
  Q_OBJECT
  Q_PROPERTY( qint64 settingsToken READ settingsToken )
public:
  ~Template();
  virtual QString render( Context *c );

  NodeList getNodesByType( const char * className );

  NodeList nodeList() const;

  void setNodeList( const NodeList &list );

  void setSettingsToken( qint64 settingsToken );
  qint64 settingsToken() const;

  Error error();
  QString errorString();

protected:
  Template( QObject *parent = 0 );

  void setContent( const QString &templateString );

private:
  Q_DECLARE_PRIVATE( Template )
  TemplatePrivate *d_ptr;
  friend class Engine;
};


}

#endif

