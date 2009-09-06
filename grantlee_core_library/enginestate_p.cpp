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

#include "enginestate_p.h"

#include <QSharedPointer>

#include "engine.h"
#include "engine_p.h"

using namespace Grantlee;

void EngineStateImpl::deleteAndRemove( Grantlee::EngineStateImpl *engineState )
{
  if ( Engine::m_instance && Engine::m_instance->d_ptr )
  {
    Engine::m_instance->d_ptr->m_states.remove( engineState->id );
    Engine::m_instance->d_ptr->m_mostRecentState = 0;
  }
  delete engineState;
  engineState = 0;
}

EngineState EngineStateImpl::getPtr()
{
  return EngineState( new EngineStateImpl(), &deleteAndRemove );
}

EngineStateImpl::EngineStateImpl()
{
  m_defaultLibraries << "grantlee_defaulttags_library"
                     << "grantlee_loadertags_library"
                     << "grantlee_defaultfilters_library"
                     << "grantlee_scriptabletags_library";
}

EngineState EngineStateImpl::clone()
{
  EngineState state = getPtr();
  state->m_pluginDirs = m_pluginDirs;
  state->m_defaultLibraries = m_defaultLibraries;
  state->m_loaders = m_loaders;
  return state;
}
