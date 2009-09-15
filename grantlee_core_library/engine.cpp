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

#include "engine.h"
#include "engine_p.h"

#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QPluginLoader>

#include "interfaces/taglibraryinterface.h"
#include "enginestate_p.h"
#include "template_p.h"
#include "templateloader.h"
#include "grantlee_version.h"

static const char * __scriptableLibName = "grantlee_scriptabletags_library";

class ScriptableLibraryContainer : public TagLibraryInterface
{
public:
  ScriptableLibraryContainer( QHash<QString, AbstractNodeFactory*> factories, QHash<QString, Filter *> filters )
      : m_nodeFactories( factories ), m_filters( filters ) {

  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );
    return m_nodeFactories;
  }

  QHash<QString, Filter*> filters( const QString &name = QString() ) {
    Q_UNUSED( name );
    return m_filters;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QHash<QString, Filter*> m_filters;

};

Engine* Engine::m_instance = 0;
Engine* Engine::instance()
{
  if ( !m_instance ) {
    m_instance = new Engine();
  }
  return m_instance;
}

Engine::Engine()
    : d_ptr( new EnginePrivate( this ) )
{
}

Engine::~Engine()
{
  qDeleteAll( d_ptr->m_scriptableLibraries );
  delete d_ptr->m_scriptableTagLibrary;
  qDeleteAll( d_ptr->m_libraries );
  delete d_ptr;
  m_instance = 0;
}

QList<AbstractTemplateLoader::Ptr> Engine::templateLoaders( qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    return p->m_loaders;
  }
  return QList<AbstractTemplateLoader::Ptr>();
}

void Engine::addTemplateLoader( AbstractTemplateLoader::Ptr loader, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_loaders << loader;
  }
}

void Engine::removeTemplateLoader( int index, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_loaders.removeAt( index );
  }
}

QString Engine::mediaUri( const QString &fileName, qint64 settingsToken ) const
{
  Q_D( const Engine );

  if ( !settingsToken ) {
    settingsToken  = d->m_mostRecentState;
  }

  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    QListIterator<AbstractTemplateLoader::Ptr> it( p->m_loaders );

    QString uri;
    while ( it.hasNext() ) {
      AbstractTemplateLoader::Ptr loader = it.next();
      uri = loader->getMediaUri( fileName );
      if ( !uri.isEmpty() )
        break;
    }
    return uri;
  }
  return QString();
}

void Engine::setPluginDirs( const QStringList &dirs, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_pluginDirs = dirs;
  }
}

QStringList Engine::pluginDirs( qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    return p->m_pluginDirs;
  }
  return QStringList();
}

QStringList Engine::defaultLibraries( qint64 settingsToken ) const
{
  Q_D( const Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;

  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    return p->m_defaultLibraries;
  }
  return QStringList();
}

void Engine::setDefaultLibraries( const QStringList &list, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_defaultLibraries = list;
  }
}

void Engine::addDefaultLibrary( const QString &libName, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;

  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_defaultLibraries << libName;
  }
}

void Engine::removeDefaultLibrary( const QString &libName, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;

  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    p->m_defaultLibraries.removeAll( libName );
  }
}

QList<TagLibraryInterface*> Engine::loadDefaultLibraries( qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  QList<TagLibraryInterface*> list;

  if ( EngineState p = d->m_states.value( settingsToken ).toStrongRef() )
  {
    // Make sure we can load default scriptable libraries if we're supposed to.
    if ( p->m_defaultLibraries.contains( __scriptableLibName ) && !d->m_scriptableTagLibrary ) {
      d->m_scriptableTagLibrary = loadLibrary( __scriptableLibName, settingsToken );
    }

    foreach( const QString &libName, p->m_defaultLibraries ) {
      if ( libName == __scriptableLibName )
        continue;

      TagLibraryInterface *library = loadLibrary( libName, settingsToken );
      if ( !library )
        continue;

      d->m_libraries.insert( libName, library );
      list << library;
    }
  }
  return list;
}

TagLibraryInterface* Engine::loadLibrary( const QString &name, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;

  // already loaded by the engine.
  if ( d->m_libraries.contains( name ) )
    return d->m_libraries.value( name );

  TagLibraryInterface* scriptableLibrary = d->loadScriptableLibrary( name, settingsToken );

  if ( scriptableLibrary )
    return scriptableLibrary;

  // else this is not a scriptable library.

  return d->loadCppLibrary( name, settingsToken );
}

EnginePrivate::EnginePrivate( Engine *engine )
    : q_ptr( engine ),
    m_mostRecentState( 0 ),
    m_scriptableTagLibrary( 0 )
{
  m_defaultStatePtr = EngineStateImpl::getPtr();
  m_defaultStatePtr->id = m_mostRecentState;
  m_states.insert( m_mostRecentState, m_defaultStatePtr.toWeakRef() );
}

TagLibraryInterface* EnginePrivate::loadScriptableLibrary( const QString &name, qint64 settingsToken )
{
  int pluginIndex = 0;
  if ( !settingsToken )
    settingsToken = m_mostRecentState;
  QString libFileName;

  if ( EngineState p = m_states.value( settingsToken ).toStrongRef() )
  {
    if ( m_scriptableTagLibrary ) {
      while ( p->m_pluginDirs.size() > pluginIndex ) {
        QString nextDir = p->m_pluginDirs.at( pluginIndex++ );
        libFileName = nextDir + GRANTLEE_MAJOR_MINOR_VERSION_STRING + "/" + name + ".qs";
        QFile file( libFileName );
        if ( !file.exists() )
          continue;

        QHash<QString, AbstractNodeFactory*> factories = m_scriptableTagLibrary->nodeFactories( libFileName );
        QHash<QString, Filter*> filters = m_scriptableTagLibrary->filters( libFileName );

        TagLibraryInterface *library = new ScriptableLibraryContainer( factories, filters );
        m_scriptableLibraries << library;
        return library;
      }
    }
  }
  return 0;
}

TagLibraryInterface* EnginePrivate::loadCppLibrary( const QString &name, qint64 settingsToken )
{
  if ( !settingsToken )
    settingsToken = m_mostRecentState;

  if ( EngineState p = m_states.value( settingsToken ).toStrongRef() )
  {
    int pluginIndex = 0;
    QString libFileName;

    QObject *plugin = 0;
    while ( p->m_pluginDirs.size() > pluginIndex ) {
      QString nextDir = p->m_pluginDirs.at( pluginIndex++ );
      libFileName = nextDir + GRANTLEE_MAJOR_MINOR_VERSION_STRING + "/" + "lib" + name + ".so";
      QFile file( libFileName );
      if ( !file.exists() )
        continue;

      QPluginLoader loader( libFileName );

      plugin = loader.instance();
      if ( plugin )
        break;
    }
    if ( !plugin )
      return 0;

    return qobject_cast<TagLibraryInterface*>( plugin );
  }
  return 0;
}

Template Engine::loadByName( const QString &name, qint64 settingsToken ) const
{
  Q_D( const Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken ) {
    d->m_mostRecentState = settingsToken;
  }

  if ( EngineState p = d->m_states.value( d->m_mostRecentState ).toStrongRef() )
  {
    QListIterator<AbstractTemplateLoader::Ptr> it( p->m_loaders );
    while ( it.hasNext() ) {
      AbstractTemplateLoader::Ptr loader = it.next();

      if ( !loader->canLoadTemplate( name ) )
        continue;

      Template t = loader->loadByName( name );

      if ( t ) {
        if ( !settingsToken ) {
          d->m_mostRecentState = t->settingsToken();
        } else
          t->setSettingsToken( settingsToken );
        if ( settingsToken )
          d->m_mostRecentState = currentSettingsToken;
        return t;
      }
    }
  }
  if ( settingsToken )
    d->m_mostRecentState = currentSettingsToken;
  throw Grantlee::Exception( TagSyntaxError, QString( "Most recent state is invalid." ) );
}

MutableTemplate Engine::loadMutableByName( const QString &name, qint64 settingsToken ) const
{
  Q_D( const Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken ) {
    d->m_mostRecentState = settingsToken;
  }

  if ( EngineState p = d->m_states.value( d->m_mostRecentState ).toStrongRef() )
  {
    QListIterator<AbstractTemplateLoader::Ptr> it( p->m_loaders );

    while ( it.hasNext() ) {
      AbstractTemplateLoader::Ptr loader = it.next();
      MutableTemplate t = loader->loadMutableByName( name );
      if ( t ) {
        if ( !settingsToken ) {
          d->m_mostRecentState = t->settingsToken();
        } else
          t->setSettingsToken( settingsToken );
        if ( settingsToken )
          d->m_mostRecentState = currentSettingsToken;
        return t;
      }
    }
  }
  if ( settingsToken )
    d->m_mostRecentState = currentSettingsToken;
  throw Grantlee::Exception( TagSyntaxError, QString( "Most recent state is invalid." ) );
}

MutableTemplate Engine::newMutableTemplate( const QString &content, const QString &name, qint64 settingsToken )
{
  Q_D( Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken ) {
    d->m_mostRecentState = settingsToken;
  }
  MutableTemplate t = MutableTemplate( new MutableTemplateImpl() );
  t->setObjectName( name );
  if ( !settingsToken ) {
    if ( EngineState p = d->m_states.value( d->m_mostRecentState ).toStrongRef() )
    {
      EngineState statePtr = p->clone();
      statePtr->id = t->settingsToken();
      t->d_ptr->m_state = statePtr;
      d->m_mostRecentState = t->settingsToken();
      d->m_states.insert( d->m_mostRecentState, statePtr.toWeakRef() );
    } else {
      Q_ASSERT(false); // state for this template no longer exists.
    }

  } else {
    t->setSettingsToken( settingsToken );
  }
  t->setContent( content );
  if ( settingsToken ) {
    d->m_mostRecentState = currentSettingsToken;
  }
  return t;
}

Template Engine::newTemplate( const QString &content, const QString &name, qint64 settingsToken )
{
  Q_D( Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken ) {
    d->m_mostRecentState = settingsToken;
  }
  Template t = Template( new TemplateImpl() );

  t->setObjectName( name );
  if ( !settingsToken ) {
    if ( EngineState p = d->m_states.value( d->m_mostRecentState ).toStrongRef() )
    {
      EngineState statePtr = p->clone();
      statePtr->id = t->settingsToken();
      t->d_ptr->m_state = statePtr;
      d->m_mostRecentState = t->settingsToken();
      d->m_states.insert( d->m_mostRecentState, statePtr.toWeakRef() );
    } else {
      Q_ASSERT(false); // state for this template no longer exists.
    }
  } else {
    t->setSettingsToken( settingsToken );
  }
  t->setContent( content );
  if ( settingsToken ) {
    d->m_mostRecentState = currentSettingsToken;
  }
  return t;
}
