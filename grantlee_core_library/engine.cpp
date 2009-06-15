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
  MutableTemplate *t = new MutableTemplate();
  t->setContent( content );
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
  Template *t = new Template();
  t->setContent( content );
  return t;
}

void InMemoryTemplateLoader::setTemplate( const QString &name, const QString &content )
{
  m_namedTemplates.insert( name, content );
}

Template* InMemoryTemplateLoader::loadByName( const QString& name ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    Template *t = new Template();
    t->setContent( m_namedTemplates.value( name ) );
    return t;
  }
  return 0;
}

MutableTemplate* InMemoryTemplateLoader::loadMutableByName( const QString& name ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    MutableTemplate *t = new MutableTemplate();
    t->setContent( m_namedTemplates.value( name ) );
    return t;
  }
  return 0;
}

namespace Grantlee
{

class EnginePrivate
{
  EnginePrivate( Engine *engine )
      : q_ptr( engine ) {

  }

  QList<AbstractTemplateLoader*> m_loaders;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;

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
  d->m_defaultLibraries << "grantlee_defaulttags_library"
  << "grantlee_loadertags_library"
  << "grantlee_defaultfilters_library"
  << "grantlee_scriptabletags_library";
}

QList<AbstractTemplateLoader*> Engine::templateLoaders()
{
  Q_D( Engine );
  return d->m_loaders;
}

void Engine::addTemplateLoader( AbstractTemplateLoader* loader )
{
  Q_D( Engine );
  d->m_loaders << loader;
}

void Engine::setPluginDirs( const QStringList &dirs )
{
  Q_D( Engine );
  d->m_pluginDirs = dirs;
}

QStringList Engine::pluginDirs()
{
  Q_D( Engine );
  return d->m_pluginDirs;
}

QStringList Engine::defaultLibraries() const
{
  Q_D( const Engine );
  return d->m_defaultLibraries;
}

void Engine::setDefaultLibraries( const QStringList &list )
{
  Q_D( Engine );
  d->m_defaultLibraries = list;
}

void Engine::addDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_defaultLibraries << libName;
}

void Engine::removeDefaultLibrary( const QString &libName )
{
  Q_D( Engine );
  d->m_defaultLibraries.removeAll( libName );
}

Template* Engine::loadByName( const QString &name, QObject *parent ) const
{
  Q_D( const Engine );
  QListIterator<AbstractTemplateLoader*> it( d->m_loaders );

  while ( it.hasNext() ) {
    AbstractTemplateLoader* loader = it.next();
    Template *t = loader->loadByName( name );
    if ( t ) {
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
      t->setSettingsToken(d->m_settingsToken);
      t->setParent( parent );
      return t;
    }
  }
  return 0;
}

MutableTemplate* Engine::newMutableTemplate(QObject *parent)
{
  Q_D( Engine );
  MutableTemplate *t = new MutableTemplate(parent);
  EngineState *state = d->m_state->clone();
  d->m_states.insert( t->settingsToken(), state );
  return t;
}

