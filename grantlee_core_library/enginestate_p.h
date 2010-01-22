
#ifndef ENGINE_STATE_P_H
#define ENGINE_STATE_P_H

#include "enginestate.h"
#include "templateloader.h"

namespace Grantlee {

class EngineStateImplPrivate
{
  Q_DECLARE_PUBLIC( EngineStateImpl );
  EngineStateImpl *q_ptr;


  QList<AbstractTemplateLoader::Ptr> m_loaders;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;

  qint64 id;

  friend class Engine;
  friend class EnginePrivate;

};

}

#endif
