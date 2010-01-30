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

#ifndef GRANTLEE_ENGINE_STATE_H
#define GRANTLEE_ENGINE_STATE_H

#include <QSharedPointer>

#include "grantlee_export.h"

namespace Grantlee
{

class EngineStateImplPrivate;

/**
  @brief The EngineStateImpl class stores a particular configuration for an Engine.

  This class deliberately does not have any public API. All EngineStates should be configured and retrieved using API
  on the Engine class.

  @see template_factories
*/
class GRANTLEE_EXPORT EngineStateImpl
{
private:
  friend class Engine;
  friend class EnginePrivate;

  Q_DECLARE_PRIVATE( EngineStateImpl )
  EngineStateImplPrivate * const d_ptr;

  EngineStateImpl();
};

typedef QWeakPointer<EngineStateImpl> EngineStateWeakPtr;
typedef QSharedPointer<EngineStateImpl> EngineState;

}

#endif
