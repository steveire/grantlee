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

#ifndef SCRIPTABLE_TEMPLATE_H
#define SCRIPTABLE_TEMPLATE_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <QtScript/QScriptValue>

#include "template.h"

namespace Grantlee
{
class Node;
}

class QScriptContext;

class ScriptableContext;

using namespace Grantlee;

QScriptValue ScriptableTemplateConstructor( QScriptContext *context,
    QScriptEngine *engine );

class ScriptableTemplate : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableTemplate( Template t, QObject* parent = 0 );

public Q_SLOTS:
  QString render( ScriptableContext *c );

  QObjectList nodeList() const;

  void setNodeList( const QObjectList &list );

private:
  Template m_template;

};

#endif

