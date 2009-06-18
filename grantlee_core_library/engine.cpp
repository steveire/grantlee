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

#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QFile>


AbstractTemplateLoader::AbstractTemplateLoader( QObject* parent )
    : QObject( parent )
{

}

AbstractTemplateLoader::~AbstractTemplateLoader()
{

}

FileSystemTemplateLoader::FileSystemTemplateLoader( QObject* parent )
    : AbstractTemplateLoader( parent )
{

}

InMemoryTemplateLoader::InMemoryTemplateLoader( QObject* parent )
    : AbstractTemplateLoader( parent )
{

}

void FileSystemTemplateLoader::setTheme( const QString &themeName )
{
  m_themeName = themeName;
}

QString FileSystemTemplateLoader::themeName() const
{
  return m_themeName;
}

void FileSystemTemplateLoader::setTemplateDirs( const QStringList &dirs )
{
  m_templateDirs = dirs;
}

// TODO Refactor these two.
MutableTemplate* FileSystemTemplateLoader::loadMutableByName( const QString &fileName ) const
{
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + "/" + m_themeName + "/" + fileName );
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return 0;
  }

  QString content;
  content = file.readAll();

  MutableTemplate *t = Engine::instance()->newMutableTemplate( content );
  return t;
}

Template* FileSystemTemplateLoader::loadByName( const QString &fileName ) const
{
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + "/" + m_themeName + "/" + fileName );
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return 0;
  }

  QString content;
  content = file.readAll();
  Template *t = Engine::instance()->newTemplate( content );
  return t;
}

void InMemoryTemplateLoader::setTemplate( const QString &name, const QString &content )
{
  m_namedTemplates.insert( name, content );
}

namespace Grantlee
{
struct EngineState
{
  EngineState() {
    m_defaultLibraries << "grantlee_defaulttags_library"
                       << "grantlee_loadertags_library"
                       << "grantlee_defaultfilters_library"
                       << "grantlee_scriptabletags_library";

  }

  QList<AbstractTemplateLoader*> m_loaders;

  EngineState* clone()
  {
    EngineState *state = new EngineState();
    state->m_pluginDirs = m_pluginDirs;
    state->m_defaultLibraries = m_defaultLibraries;
    state->m_loaders = m_loaders;
    return state;
  }

  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;

};

}

Template* InMemoryTemplateLoader::loadByName( const QString& name ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    Template *t = Engine::instance()->newTemplate( m_namedTemplates.value( name ) );
    return t;
  }
  return 0;
}

MutableTemplate* InMemoryTemplateLoader::loadMutableByName( const QString& name ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    MutableTemplate *t = Engine::instance()->newMutableTemplate( m_namedTemplates.value( name ) );
    return t;
  }
  return 0;
}

namespace Grantlee
{

class EnginePrivate
{
  EnginePrivate( Engine *engine )
      : q_ptr( engine ),
      m_mostRecentState( 0 ) {

    EngineState *initialState = new EngineState();
    m_states.insert( m_mostRecentState, initialState );
  }

  mutable qint64 m_mostRecentState;
  mutable QHash<qint64, EngineState*> m_states;

  Q_DECLARE_PUBLIC( Engine );
  Engine *q_ptr;
};

}

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
  qDeleteAll(d_ptr->m_states);
  delete d_ptr;
}

QList<AbstractTemplateLoader*> Engine::templateLoaders( qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  return d->m_states.value( settingsToken )->m_loaders;
}

void Engine::addTemplateLoader( AbstractTemplateLoader* loader, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_loaders << loader;
}

void Engine::removeTemplateLoader( int index, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_loaders.removeAt( index );
}

void Engine::setPluginDirs( const QStringList &dirs, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_pluginDirs = dirs;
}

QStringList Engine::pluginDirs( qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  return d->m_states.value( settingsToken )->m_pluginDirs;
}

QStringList Engine::defaultLibraries( qint64 settingsToken ) const
{
  Q_D( const Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  return d->m_states.value( settingsToken )->m_defaultLibraries;
}

void Engine::setDefaultLibraries( const QStringList &list, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_defaultLibraries = list;
}

void Engine::addDefaultLibrary( const QString &libName, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_defaultLibraries << libName;
}

void Engine::removeDefaultLibrary( const QString &libName, qint64 settingsToken )
{
  Q_D( Engine );
  if ( !settingsToken )
    settingsToken = d->m_mostRecentState;
  d->m_states.value( settingsToken )->m_defaultLibraries.removeAll( libName );
}

Template* Engine::loadByName( const QString &name, QObject *parent, qint64 settingsToken ) const
{
  Q_D( const Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if (settingsToken)
  {
    d->m_mostRecentState = settingsToken;
  }

  QListIterator<AbstractTemplateLoader*> it( d->m_states.value( d->m_mostRecentState )->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader* loader = it.next();
    Template *t = loader->loadByName( name );
    if ( t ) {
      if ( !settingsToken )
      {
        EngineState *state = d->m_states.value( d->m_mostRecentState )->clone();
        d->m_mostRecentState = t->settingsToken();
        d->m_states.insert( d->m_mostRecentState, state );
      }
      else
        t->setSettingsToken( settingsToken );
      t->setParent( parent );
      if ( settingsToken )
        d->m_mostRecentState = currentSettingsToken;
      return t;
    }
  }
  if ( settingsToken )
    d->m_mostRecentState = currentSettingsToken;
  return 0;
}

MutableTemplate* Engine::loadMutableByName( const QString &name, QObject *parent, qint64 settingsToken ) const
{
  Q_D( const Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken )
  {
    d->m_mostRecentState = settingsToken;
  }

  QListIterator<AbstractTemplateLoader*> it( d->m_states.value(d->m_mostRecentState)->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader* loader = it.next();
    MutableTemplate *t = loader->loadMutableByName( name );
    if ( t ) {
      if ( !settingsToken )
      {
        EngineState *state = d->m_states.value( d->m_mostRecentState )->clone();
        d->m_mostRecentState = t->settingsToken();
        d->m_states.insert( d->m_mostRecentState, state );
      }
      else
        t->setSettingsToken( settingsToken );
      t->setParent( parent );
      if ( settingsToken )
        d->m_mostRecentState = currentSettingsToken;
      return t;
    }
  }
  if (settingsToken)
    d->m_mostRecentState = currentSettingsToken;
  return 0;
}

MutableTemplate* Engine::newMutableTemplate( const QString &content, QObject *parent, qint64 settingsToken )
{
  Q_D( Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if (settingsToken)
  {
    d->m_mostRecentState = settingsToken;
  }
  MutableTemplate *t = new MutableTemplate( parent );
  if ( !settingsToken )
  {
    EngineState *state = d->m_states.value( d->m_mostRecentState )->clone();
    d->m_mostRecentState = t->settingsToken();
    d->m_states.insert( d->m_mostRecentState, state );
  } else {
    t->setSettingsToken( settingsToken );
  }
  t->setContent( content );
  if ( settingsToken )
  {
    d->m_mostRecentState = currentSettingsToken;
  }
  return t;
}

Template* Engine::newTemplate( const QString &content, QObject *parent, qint64 settingsToken )
{
  Q_D( Engine );
  qint64 currentSettingsToken = d->m_mostRecentState;
  if ( settingsToken )
  {
    d->m_mostRecentState = settingsToken;
  }
  Template *t = new Template(parent);
  if ( !settingsToken )
  {
    EngineState *state = d->m_states.value( d->m_mostRecentState )->clone();
    d->m_mostRecentState = t->settingsToken();
    d->m_states.insert( d->m_mostRecentState, state );
  } else {
    t->setSettingsToken( settingsToken );
  }
  t->setContent( content );
  if ( settingsToken )
  {
    d->m_mostRecentState = currentSettingsToken;
  }
  return t;
}
