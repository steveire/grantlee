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

#ifndef GRANTLEE_ENGINE_P_H
#define GRANTLEE_ENGINE_P_H

#include "engine.h"
#include "filter.h"
#include "pluginpointer_p.h"
#include "taglibraryinterface.h"

class QPluginLoader;

namespace Grantlee
{

class ScriptableTagLibrary;

class ScriptableLibraryContainer : public TagLibraryInterface
{
public:
  ScriptableLibraryContainer( const QHash<QString, AbstractNodeFactory*>& factories,
                              const QHash<QString, Filter *> &filters )
      : m_nodeFactories( factories ), m_filters( filters ) {
  }

  void setNodeFactories( const QHash<QString, AbstractNodeFactory*>& factories )
  {
    m_nodeFactories = factories;
  }

  void setFilters( const QHash<QString, Filter*> &filters )
  {
    m_filters = filters;
  }

  // Warning: should only be called by Engine::loadDefaultLibraries
  void clear()
  {
    qDeleteAll( m_nodeFactories );
    qDeleteAll( m_filters );
    m_nodeFactories.clear();
    m_filters.clear();
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) override {
    Q_UNUSED( name );
    return m_nodeFactories;
  }

  QHash<QString, Filter*> filters( const QString &name = QString() ) override {
    Q_UNUSED( name );
    return m_filters;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QHash<QString, Filter*> m_filters;

};

class EnginePrivate
{
  EnginePrivate( Engine *engine );

  TagLibraryInterface* loadLibrary( const QString &name, uint minorVersion );
  QString getScriptLibraryName( const QString &name, uint minorVersion ) const;
#ifdef QT_SCRIPT_LIB
  ScriptableLibraryContainer* loadScriptableLibrary( const QString &name, uint minorVersion );
#endif
  PluginPointer<TagLibraryInterface> loadCppLibrary( const QString& name, uint minorVersion );

  Q_DECLARE_PUBLIC( Engine )
  Engine * const q_ptr;

  QHash<QString, PluginPointer<TagLibraryInterface> > m_libraries;
#ifdef QT_SCRIPT_LIB
  QHash<QString, ScriptableLibraryContainer*> m_scriptableLibraries;
#endif

  QList<QSharedPointer<AbstractTemplateLoader> > m_loaders;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;
#ifdef QT_SCRIPT_LIB
  ScriptableTagLibrary *m_scriptableTagLibrary;
#endif
  bool m_smartTrimEnabled;
};

}

#endif
