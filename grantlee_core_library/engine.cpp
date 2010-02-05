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
#include "exception.h"

using namespace Grantlee;

static const char * __scriptableLibName = "grantlee_scriptabletags";

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

QList<AbstractTemplateLoader::Ptr> Engine::templateLoaders()
{
  Q_D( Engine );
  return d->m_currentState->d_ptr->m_loaders;
}

void Engine::addTemplateLoader( AbstractTemplateLoader::Ptr loader )
{
  Q_D( Engine );
  d->m_currentState->d_ptr->m_loaders << loader;
}

QString Engine::mediaUri( const QString &fileName, const EngineState &_state ) const
{
  Q_D( const Engine );
  EngineState state = _state ? _state : d->m_currentState;
  QListIterator<AbstractTemplateLoader::Ptr> it( state->d_ptr->m_loaders );

  QString uri;
  while ( it.hasNext() ) {
    AbstractTemplateLoader::Ptr loader = it.next();
    uri = loader->getMediaUri( fileName );
    if ( !uri.isEmpty() )
      break;
  }
  return uri;
}

void Engine::setPluginDirs( const QStringList &dirs )
{
  Q_D( Engine );
  d->m_currentState->d_ptr->m_pluginDirs = dirs;
}

QStringList Engine::defaultLibraries() const
{
  Q_D( const Engine );
  return d->m_currentState->d_ptr->m_defaultLibraries;
}

void Engine::addDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_currentState->d_ptr->m_defaultLibraries << libName;
}

void Engine::removeDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_currentState->d_ptr->m_defaultLibraries.removeAll( libName );
}

QList<TagLibraryInterface*> Engine::loadDefaultLibraries( const EngineState &_state )
{
  Q_D( Engine );
  EngineState state = _state ? _state : d->m_currentState;
  QList<TagLibraryInterface*> list;
  // Make sure we can load default scriptable libraries if we're supposed to.
  if ( state->d_ptr->m_defaultLibraries.contains( __scriptableLibName ) && !d->m_scriptableTagLibrary ) {
    d->m_scriptableTagLibrary = loadLibrary( __scriptableLibName, state );
  }

  foreach( const QString &libName, state->d_ptr->m_defaultLibraries ) {
    if ( libName == __scriptableLibName )
      continue;

    TagLibraryInterface *library = loadLibrary( libName, state );
    if ( !library )
      continue;

    d->m_libraries.insert( libName, library );
    list << library;
  }
  return list;
}

TagLibraryInterface* Engine::loadLibrary( const QString &name, const EngineState &_state )
{
  Q_D( Engine );
  EngineState state = _state ? _state : d->m_currentState;

  if ( name == __scriptableLibName )
    return 0;

  // already loaded by the engine.
  if ( d->m_libraries.contains( name ) )
    return d->m_libraries.value( name );

  TagLibraryInterface* scriptableLibrary = d->loadScriptableLibrary( name, state );
  if ( scriptableLibrary )
    return scriptableLibrary;

  // else this is not a scriptable library.

  return d->loadCppLibrary( name, state );
}

EnginePrivate::EnginePrivate( Engine *engine )
    : q_ptr( engine ),
    m_scriptableTagLibrary( 0 )
{
  m_currentState = staticEmptyState();
}

TagLibraryInterface* EnginePrivate::loadScriptableLibrary( const QString &name, const EngineState &_state )
{
  EngineState state = _state ? _state : m_currentState;

  int pluginIndex = 0;
  QString libFileName;
  if ( m_scriptableTagLibrary ) {
    while ( state->d_ptr->m_pluginDirs.size() > pluginIndex ) {
      QString nextDir = state->d_ptr->m_pluginDirs.at( pluginIndex++ );
      libFileName = nextDir + GRANTLEE_MAJOR_MINOR_VERSION_STRING + '/' + name + ".qs";
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

  return 0;
}

TagLibraryInterface* EnginePrivate::loadCppLibrary( const QString &name, const EngineState &_state )
{
  EngineState state = _state ? _state : m_currentState;

  int pluginIndex = 0;
  QString libFileName;

  QObject *plugin = 0;
  while ( state->d_ptr->m_pluginDirs.size() > pluginIndex ) {
    QString nextDir = state->d_ptr->m_pluginDirs.at( pluginIndex++ );
    QDir pluginDir( nextDir + GRANTLEE_MAJOR_MINOR_VERSION_STRING + '/' );

    if ( !pluginDir.exists() )
      continue;

    QStringList list = pluginDir.entryList( QStringList( name + "*" ) );

    if (list.isEmpty())
      continue;

    QPluginLoader loader( pluginDir.absoluteFilePath( list.first() ) );

    plugin = loader.instance();
    if ( plugin )
      break;
  }
  if ( !plugin )
    return 0;

  return qobject_cast<TagLibraryInterface*>( plugin );
}

Template Engine::loadByName( const QString &name, const EngineState &_state ) const
{
  Q_D( const Engine );

  EngineState state = _state ? _state : d->m_currentState;

  QListIterator<AbstractTemplateLoader::Ptr> it( state->d_ptr->m_loaders );
  while ( it.hasNext() ) {
    AbstractTemplateLoader::Ptr loader = it.next();

    if ( !loader->canLoadTemplate( name ) )
      continue;

    Template t = loader->loadByName( name );

    if ( t ) {
      t->d_ptr->m_state = state;
      return t;
    }
  }
  throw Grantlee::Exception( TagSyntaxError, QString( "Most recent state is invalid." ) );
}

MutableTemplate Engine::loadMutableByName( const QString &name, const EngineState &_state ) const
{
  Q_D( const Engine );

  EngineState state = _state ? _state : d->m_currentState;

  QListIterator<AbstractTemplateLoader::Ptr> it( state->d_ptr->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader::Ptr loader = it.next();
    MutableTemplate t = loader->loadMutableByName( name );
    if ( t ) {
      t->d_ptr->m_state = state;
      return t;
    }
  }
  throw Grantlee::Exception( TagSyntaxError, QString( "Most recent state is invalid." ) );
}

MutableTemplate Engine::newMutableTemplate( const QString &content, const QString &name, const EngineState &_state ) const
{
  Q_D( const Engine );
  EngineState state = _state ? _state : d->m_currentState;

  MutableTemplate t = MutableTemplate( new MutableTemplateImpl() );
  t->setObjectName( name );
  t->d_ptr->m_state = state;
  t->setContent( content );
  return t;
}

Template Engine::newTemplate( const QString &content, const QString &name, const EngineState &_state ) const
{
  Q_D( const Engine );
  EngineState state = _state ? _state : d->m_currentState;

  Template t = Template( new TemplateImpl() );

  t->setObjectName( name );
  t->d_ptr->m_state = state;
  t->setContent( content );
  return t;
}

void Engine::resetState()
{
  Q_D( Engine );
  // set the default empty state.
  d->m_currentState = d->staticEmptyState();
}

EngineState EnginePrivate::staticEmptyState()
{
  static EngineState state = EngineState( new EngineStateImpl() );
  return state;
}

