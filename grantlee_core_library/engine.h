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

#ifndef ENGINE_H
#define ENGINE_H

#include "template.h"
#include "mutabletemplate.h"
#include "interfaces/taglibraryinterface.h"
#include "templateloader.h"

namespace Grantlee
{

class EnginePrivate;

class GRANTLEE_EXPORT Engine
{
public:
  static Engine* instance();

  ~Engine();

  QList<AbstractTemplateLoader::Ptr> templateLoaders( const EngineState &state = EngineState() );
  void addTemplateLoader( AbstractTemplateLoader::Ptr loader, const EngineState &state = EngineState() );
  void removeTemplateLoader( int index, const EngineState &state = EngineState() );

  void setPluginDirs( const QStringList &dirs, const EngineState &state = EngineState() );
  QStringList pluginDirs( const EngineState &state = EngineState() );

  QString mediaUri( const QString &fileName, const EngineState &state = EngineState() ) const;

  void resetState();

  /**
  Causes a state transition if settingsToken is 0.
  */
  Template loadByName( const QString &name, const EngineState &state = EngineState() ) const;

  /**
  Causes a state transition if settingsToken is 0.
  */
  Template newTemplate( const QString &content, const QString &name, const EngineState &state = EngineState() );

  /**
  Causes a state transition if settingsToken is 0.
  */
  MutableTemplate loadMutableByName( const QString &name, const EngineState &state = EngineState() ) const;

  /**
  Causes a state transition if settingsToken is 0.
  */
  MutableTemplate newMutableTemplate( const QString &content, const QString &name, const EngineState &state = EngineState() );

  QStringList defaultLibraries( const EngineState &state = EngineState() ) const;
  void setDefaultLibraries( const QStringList &list, const EngineState &state = EngineState() );
  void addDefaultLibrary( const QString &libName, const EngineState &state = EngineState() );
  void removeDefaultLibrary( const QString &libName, const EngineState &state = EngineState() );

  QList<TagLibraryInterface*> loadDefaultLibraries( const EngineState &state = EngineState() );

  TagLibraryInterface* loadLibrary( const QString &name, const EngineState &state = EngineState() );

  /**
    Returns the current state of the Engine.
  */
  EngineState state();

private:
  Engine();

  Q_DECLARE_PRIVATE( Engine )
  EnginePrivate *d_ptr;

  static Engine* m_instance;
};

}

#endif

