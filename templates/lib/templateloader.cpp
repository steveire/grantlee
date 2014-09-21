/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "templateloader.h"

#include "engine.h"
#include "exception.h"
#include "nulllocalizer_p.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

using namespace Grantlee;

AbstractTemplateLoader::~AbstractTemplateLoader()
{

}

namespace Grantlee {
class FileSystemTemplateLoaderPrivate
{
    FileSystemTemplateLoaderPrivate( FileSystemTemplateLoader *loader )
        : q_ptr( loader ) {

    }
    Q_DECLARE_PUBLIC( FileSystemTemplateLoader )
    FileSystemTemplateLoader * const q_ptr;

    QString m_themeName;
    QStringList m_templateDirs;
};
}

FileSystemTemplateLoader::FileSystemTemplateLoader()
  : AbstractTemplateLoader(),
  d_ptr( new FileSystemTemplateLoaderPrivate( this ) )
{

}

FileSystemTemplateLoader::~FileSystemTemplateLoader()
{
  delete d_ptr;
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
  Q_D(FileSystemTemplateLoader);
  d->m_themeName = themeName;
}

QString FileSystemTemplateLoader::themeName() const
{
  Q_D(const FileSystemTemplateLoader);
  return d->m_themeName;
}

void FileSystemTemplateLoader::setTemplateDirs( const QStringList &dirs )
{
  Q_D(FileSystemTemplateLoader);
  d->m_templateDirs = dirs;
}

QStringList FileSystemTemplateLoader::templateDirs() const
{
  Q_D(const FileSystemTemplateLoader);
  return d->m_templateDirs;
}

bool FileSystemTemplateLoader::canLoadTemplate( const QString &name ) const
{
  Q_D(const FileSystemTemplateLoader);
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= d->m_templateDirs.size() )
      break;

    file.setFileName( d->m_templateDirs.at( i ) + QLatin1Char( '/' ) + d->m_themeName + QLatin1Char( '/' ) + name );
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return false;
  }
  file.close();
  return true;
}

Template FileSystemTemplateLoader::loadByName( const QString &fileName, Engine const *engine ) const
{
  Q_D(const FileSystemTemplateLoader);
  int i = 0;
  QFile file;

  while ( !file.exists() ) {
    if ( i >= d->m_templateDirs.size() )
      break;

    file.setFileName( d->m_templateDirs.at( i ) + QLatin1Char( '/' ) + d->m_themeName + QLatin1Char( '/' ) + fileName );
    const QFileInfo fi( file );

    if ( file.exists() &&
          !fi.canonicalFilePath().contains( QDir( d->m_templateDirs.at( i ) ).canonicalPath() ) )
      return Template();
    ++i;
  }

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return Template();
  }

  QTextStream fstream( &file );
  fstream.setCodec( "UTF-8" );
  const QString fileContent = fstream.readAll();

  return engine->newTemplate( fileContent, fileName );
}

QPair<QString, QString> FileSystemTemplateLoader::getMediaUri( const QString& fileName ) const
{
  Q_D(const FileSystemTemplateLoader);
  int i = 0;
  QFile file;
  while ( !file.exists() ) {
    if ( i >= d->m_templateDirs.size() )
      break;

    file.setFileName( d->m_templateDirs.at( i ) + QLatin1Char( '/' ) + d->m_themeName + QLatin1Char( '/' ) + fileName );

    const QFileInfo fi( file );
    if ( !fi.canonicalFilePath().contains( QDir( d->m_templateDirs.at( i ) ).canonicalPath() ) ) {
      ++i;
      continue;
    }

    if ( file.exists() ) {
      QString path = fi.absoluteFilePath();
      path.chop( fileName.size() );
      return qMakePair( path, fileName );
    }
    ++i;
  }
  return QPair<QString, QString>();
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
    return engine->newTemplate( m_namedTemplates.value( name ), name );
  }
  throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "Couldn't load template %1. Template does not exist." ).arg( name ) );
}

QPair<QString, QString> InMemoryTemplateLoader::getMediaUri( const QString& fileName ) const
{
  Q_UNUSED( fileName )
  // This loader doesn't make any media available yet.
  return QPair<QString, QString>();
}

namespace Grantlee
{

class LocalizedFileSystemTemplateLoaderPrivate
{
  LocalizedFileSystemTemplateLoaderPrivate( LocalizedFileSystemTemplateLoader *qq, AbstractLocalizer::Ptr localizer )
    : q_ptr( qq ), m_localizer( localizer ? localizer : AbstractLocalizer::Ptr( new NullLocalizer ) )
  {

  }
  Q_DECLARE_PUBLIC( LocalizedFileSystemTemplateLoader )
  LocalizedFileSystemTemplateLoader * const q_ptr;
  const AbstractLocalizer::Ptr m_localizer;
};

}

LocalizedFileSystemTemplateLoader::LocalizedFileSystemTemplateLoader( const AbstractLocalizer::Ptr localizer )
  : FileSystemTemplateLoader(),
    d_ptr( new LocalizedFileSystemTemplateLoaderPrivate( this, localizer ) )
{

}

LocalizedFileSystemTemplateLoader::~LocalizedFileSystemTemplateLoader()
{
  Q_FOREACH( const QString &dir, templateDirs() )
    d_ptr->m_localizer->unloadCatalog( dir + QLatin1Char( '/' ) + themeName() );
  delete d_ptr;
}

void LocalizedFileSystemTemplateLoader::setTemplateDirs( const QStringList& dirs )
{
  Q_D( LocalizedFileSystemTemplateLoader );
  Q_FOREACH( const QString &dir, templateDirs() )
    d->m_localizer->unloadCatalog( dir + QLatin1Char( '/' ) + themeName() );
  FileSystemTemplateLoader::setTemplateDirs( dirs );
  Q_FOREACH( const QString &dir, templateDirs() )
    d->m_localizer->loadCatalog( dir + QLatin1Char( '/' ) + themeName(), themeName() );
}

void LocalizedFileSystemTemplateLoader::setTheme( const QString& _themeName )
{
  Q_D( LocalizedFileSystemTemplateLoader );
  Q_FOREACH( const QString &dir, templateDirs() )
    d->m_localizer->unloadCatalog( dir + QLatin1Char( '/' ) + themeName() );
  FileSystemTemplateLoader::setTheme( _themeName );
  Q_FOREACH( const QString &dir, templateDirs() )
    d->m_localizer->loadCatalog( dir + QLatin1Char( '/' ) + _themeName, _themeName );
}
