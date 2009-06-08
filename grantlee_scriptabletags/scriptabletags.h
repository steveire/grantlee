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

#ifndef SCRIPTABLETAGS_H
#define SCRIPTABLETAGS_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

// #include <QScriptEngine>

// #include <QSharedPointer>
// typedef QSharedPointer<QScriptEngine> ScriptEnginePointer;

class QScriptEngine;

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class ScriptableTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  ScriptableTagLibrary( QObject *parent = 0 );

  virtual QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() );

  virtual QHash<QString, Filter*> filters( const QString &name = QString() );

public slots:
  void addFactory( const QString &factoryName );
  void addFilter( const QString &filterName );

protected:
  bool evaluateScript( const QString &name );
  QHash<QString, AbstractNodeFactory*> getFactories();
  QHash<QString, Filter*> getFilters();

private:
//   ScriptEnginePointer m_scriptEngine;
  QScriptEngine *m_scriptEngine;
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QStringList m_factoryNames;
  QStringList m_filterNames;
  QHash<QString, Filter*> m_filters;

};

#endif

