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

#include "templateloader.h"
#include "engine.h"
#include "exception.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

using namespace Grantlee;

AbstractTemplateLoader::~AbstractTemplateLoader()
{

}

FileSystemTemplateLoader::FileSystemTemplateLoader()
    : AbstractTemplateLoader()
{

}

FileSystemTemplateLoader::~FileSystemTemplateLoader()
{

}

InMemoryTemplateLoader::InMemoryTemplateLoader()
    : AbstractTemplateLoader()
{

}

InMemoryTemplateLoader::~InMemoryTemplateLoader()
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

bool FileSystemTemplateLoader::canLoadTemplate( const QString &name ) const
{
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + '/' + m_themeName + '/' + name );
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return false;
  }
  file.close();
  return true;
}

// TODO Refactor these two.
MutableTemplate FileSystemTemplateLoader::loadMutableByName( const QString &fileName, Engine const *engine ) const
{
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + '/' + m_themeName + '/' + fileName );

    QFileInfo fi( file );
    if ( !fi.canonicalFilePath().contains( QDir( m_templateDirs.at( i ) ).canonicalPath() ) )
      return MutableTemplate();
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "Couldn't load template from %1. File does not exist." ).arg( fileName ) );
  }

  QString content;
  content = file.readAll();

  MutableTemplate t = engine->newMutableTemplate( content, fileName );
  return t;
}

Template FileSystemTemplateLoader::loadByName( const QString &fileName, Engine const *engine ) const
{
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + '/' + m_themeName + '/' + fileName );
    QFileInfo fi( file );
    if ( !fi.canonicalFilePath().contains( QDir( m_templateDirs.at( i ) ).canonicalPath() ) )
      return Template();
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "Couldn't load template from %1. File does not exist." ).arg( fileName ) );
  }

  QString content;
  content = file.readAll();
  Template t = engine->newTemplate( content, fileName );
  return t;
}

QString FileSystemTemplateLoader::getMediaUri( const QString& fileName ) const
{
  int i = 0;
  QFile file;
  while ( !file.exists() ) {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName( m_templateDirs.at( i ) + '/' + m_themeName + '/' + fileName );

    QFileInfo fi( file );
    if ( !fi.canonicalFilePath().contains( QDir( m_templateDirs.at( i ) ).canonicalPath() ) )
      return QString();

    if ( file.exists() ) {
      QFileInfo fi( file );
      return fi.absoluteFilePath();
    }
    ++i;
  }
  return QString();
}


void InMemoryTemplateLoader::setTemplate( const QString &name, const QString &content )
{
  m_namedTemplates.insert( name, content );
}

bool InMemoryTemplateLoader::canLoadTemplate( const QString &name ) const
{
  return m_namedTemplates.contains( name );
}

Template InMemoryTemplateLoader::loadByName( const QString& name, Engine const *engine ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    Template t = engine->newTemplate( m_namedTemplates.value( name ), name );
    return t;
  }
  throw Grantlee::Exception( TagSyntaxError, QString( "Couldn't load template %1. Template does not exist." ).arg( name ) );
}

MutableTemplate InMemoryTemplateLoader::loadMutableByName( const QString& name, Engine const *engine ) const
{
  if ( m_namedTemplates.contains( name ) ) {
    MutableTemplate t = engine->newMutableTemplate( m_namedTemplates.value( name ), name );
    return t;
  }
  throw Grantlee::Exception( TagSyntaxError, QString( "Couldn't load template %1. Template does not exist." ).arg( name ) );
}

QString InMemoryTemplateLoader::getMediaUri( const QString& fileName ) const
{
  Q_UNUSED( fileName )
  // This loader doesn't make any media available yet.
  return QString();
}


