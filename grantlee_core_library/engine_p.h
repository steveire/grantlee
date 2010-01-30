
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
#include "enginestate_p.h"

namespace Grantlee
{

class EnginePrivate
{
  EnginePrivate( Engine *engine );

  static EngineState staticEmptyState();

  TagLibraryInterface* loadScriptableLibrary( const QString &name, const EngineState &state = EngineState() );
  TagLibraryInterface* loadCppLibrary( const QString& name, const EngineState &state = EngineState() );

  Q_DECLARE_PUBLIC( Engine )
  Engine *q_ptr;

  EngineState m_currentState;

  TagLibraryInterface *m_scriptableTagLibrary;
  QHash<QString, TagLibraryInterface*> m_libraries;
  QList<TagLibraryInterface*> m_scriptableLibraries;

  friend class EngineStateImpl;
};

}

#endif
