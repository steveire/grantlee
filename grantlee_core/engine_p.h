
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

#ifndef GRANTLEE_ENGINE_P_H
#define GRANTLEE_ENGINE_P_H

#include "engine.h"

class QPluginLoader;

class QPluginLoader;

namespace Grantlee
{

class EnginePrivate
{
  EnginePrivate( Engine *engine );

  TagLibraryInterface* loadLibrary( const QString &name, uint minorVersion );
  TagLibraryInterface* loadScriptableLibrary( const QString &name, uint minorVersion );
  TagLibraryInterface* loadCppLibrary( const QString& name, uint minorVersion );

  Q_DECLARE_PUBLIC( Engine )
  Engine *q_ptr;

  QList<QPluginLoader*> m_pluginLoaders;

  QHash<QString, TagLibraryInterface*> m_libraries;
  QList<TagLibraryInterface*> m_scriptableLibraries;

  QList<AbstractTemplateLoader::Ptr> m_loaders;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;
};

}

#endif
