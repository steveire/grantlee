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

#include "enginestate.h"
#include "enginestate_p.h"

#include "engine.h"
#include "engine_p.h"

using namespace Grantlee;

EngineStateImpl::EngineStateImpl()
  : d_ptr( new EngineStateImplPrivate() )
{
  Q_D( EngineStateImpl );
  d->m_defaultLibraries << "grantlee_defaulttags"
                        << "grantlee_loadertags"
                        << "grantlee_defaultfilters"
                        << "grantlee_scriptabletags";
}
