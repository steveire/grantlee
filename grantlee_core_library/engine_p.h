
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

#ifndef ENGINE_P_H
#define ENGINE_P_H

#include "engine.h"
#include "enginestate_p.h"

namespace Grantlee
{

class EnginePrivate
{
  EnginePrivate( Engine *engine );

  TagLibraryInterface* loadScriptableLibrary( const QString &name, qint64 settingsToken );
  TagLibraryInterface* loadCppLibrary( const QString& name, qint64 settingsToken );

  mutable qint64 m_mostRecentState;
  mutable QHash<qint64, EngineStateWeakPtr> m_states;
  EngineState m_defaultStatePtr;

  TagLibraryInterface *m_scriptableTagLibrary;
  QHash<QString, TagLibraryInterface*> m_libraries;
  QList<TagLibraryInterface*> m_scriptableLibraries;

  Q_DECLARE_PUBLIC( Engine )
  Engine *q_ptr;

  friend class EngineStateImpl;
};

}

#endif
