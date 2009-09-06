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

#ifndef ENGINE_STATE_P_H
#define ENGINE_STATE_P_H

#include <QSharedPointer>

#include "engine.h"

namespace Grantlee
{

struct EngineStateImpl;

typedef QWeakPointer<EngineStateImpl> EngineStateWeakPtr;
typedef QSharedPointer<EngineStateImpl> EngineState;

struct EngineStateImpl
{
  static void deleteAndRemove( Grantlee::EngineStateImpl *engineState );

  static EngineState getPtr();

  EngineState clone();

  QList<AbstractTemplateLoader*> m_loaders;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;

  qint64 id;

private:
  EngineStateImpl();
};

}

#endif
