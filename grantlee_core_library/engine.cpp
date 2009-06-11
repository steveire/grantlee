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

    QList<AbstractTemplateLoader*> list;
    foreach(AbstractTemplateLoader *loader, m_loaders)
    {
      list << loader;
    }
    state->m_loaders = list;
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
      : q_ptr( engine ), m_settingsToken( 0 ) {

    m_state = new EngineState();
    m_states.insert( m_settingsToken, m_state );
  }

  void setState( qint64 settingsToken ) {
    if ( m_states.contains( settingsToken ) )
    {
      m_state = m_states.value( settingsToken );
    }
    else
      m_state = m_states.value( 0 );
    m_settingsToken = settingsToken;
  }

  qint64 m_settingsToken;
  EngineState *m_state;
  QHash<qint64, EngineState*> m_states;



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
  Q_D( Engine );
}

Engine::~Engine()
{
  delete d_ptr;
}

QList<AbstractTemplateLoader*> Engine::templateLoaders()
{
  Q_D( Engine );
  return d->m_state->m_loaders;
}

void Engine::addTemplateLoader( AbstractTemplateLoader* loader )
{
  Q_D( Engine );
  d->m_state->m_loaders << loader;
}

void Engine::removeTemplateLoader(int index)
{
  Q_D( Engine );
  d->m_state->m_loaders.removeAt(index);
}

void Engine::setPluginDirs( const QStringList &dirs )
{
  Q_D( Engine );
  d->m_state->m_pluginDirs = dirs;
}

QStringList Engine::pluginDirs()
{
  Q_D( Engine );
  return d->m_state->m_pluginDirs;
}

QStringList Engine::defaultLibraries() const
{
  Q_D( const Engine );
  return d->m_state->m_defaultLibraries;
}

void Engine::setDefaultLibraries( const QStringList &list )
{
  Q_D( Engine );
  d->m_state->m_defaultLibraries = list;
}

void Engine::addDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_state->m_defaultLibraries << libName;
}

void Engine::removeDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_state->m_defaultLibraries.removeAll( libName );
}

Template* Engine::loadByName( const QString &name, QObject *parent ) const
{
  Q_D( const Engine );
  QListIterator<AbstractTemplateLoader*> it( d->m_state->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader* loader = it.next();
    Template *t = loader->loadByName( name );
    if ( t ) {
      t->setSettingsToken( d->m_settingsToken );
      t->setParent( parent );
      return t;
    }
  }
  return 0;
}

MutableTemplate* Engine::loadMutableByName( const QString &name, QObject *parent ) const
{

  Q_D( const Engine );
  QListIterator<AbstractTemplateLoader*> it( d->m_state->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader* loader = it.next();
    MutableTemplate *t = loader->loadMutableByName( name );
    if ( t ) {
      t->setSettingsToken( d->m_settingsToken );
      t->setParent( parent );
      return t;
    }
  }
  return 0;
}

MutableTemplate* Engine::newMutableTemplate( const QString &content, QObject *parent )
{
  Q_D( Engine );
  MutableTemplate *t = new MutableTemplate( parent );
  EngineState *state = d->m_state->clone();
  d->m_states.insert( t->settingsToken(), state );
  t->setContent( content );
  return t;
}

Template* Engine::newTemplate( const QString &content, QObject *parent )
{
  Q_D( Engine );
  Template *t = new Template(parent);
  EngineState *state = d->m_state->clone();
  d->m_states.insert( t->settingsToken(), state );
  t->setContent( content );
  return t;
}

void Engine::setSettingsToken( qint64 settingsToken )
{
  Q_D( Engine );
  d->setState(settingsToken);
}

