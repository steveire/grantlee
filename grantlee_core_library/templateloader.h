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

#ifndef GRANTLEE_TEMPLATELOADER_H
#define GRANTLEE_TEMPLATELOADER_H

#include <QtCore/QSharedPointer>

#include "mutabletemplate.h"
#include "template.h"

namespace Grantlee
{

/**
  @brief An retrieval interface to a storage location for Template objects.

  This interface can be implemented to define new ways of retrieving the content of Templates.

  The interface of this class should not be called directly from applications. TemplateLoaders will typically
  be created, configured and added to the Grantlee::Engine which will call the appropriate API.
*/
class AbstractTemplateLoader
{
public:
  typedef QSharedPointer<AbstractTemplateLoader> Ptr;

  /**
    Destructor
  */
  virtual ~AbstractTemplateLoader();

  /**
    Load a MutableTemplate called @p name. Return an invalid Template if no content by that name exists.
  */
  virtual MutableTemplate loadMutableByName( const QString &name ) const = 0;

  /**
    Load a Template called @p name. Return an invalid Template if no content by that name exists.
  */
  virtual Template loadByName( const QString &name ) const = 0;

  /**
    Return a complete URI for media identified by fileName.
  */
  virtual QString getMediaUri( const QString &fileName ) const = 0;

  /**
    Return true if a Template identified by @p name exists and can be loaded.
  */
  virtual bool canLoadTemplate( const QString &name ) const = 0;

};

/**
  @brief The FileSystemTemplateLoader loads Templates from the file system.

  This template loader works by traversing a list of directories to find templates. Directories
  are checked in order, and the first match hit is parsed and returned.

  @code
    loader.setTemplateDirs(QStringList() << "/home/user/app/templates" << "/usr/local/share/app/templates" );
    Engine::instance()->setTemplateLoader( loader );

    // This will try /home/user/app/templates/mytemplate.html
    // followed by /usr/local/share/app/templates/mytemplate.html
    Engine::instance()->loadByName( "mytemplate.html" );
  @endcode

  Additionally, a themeName may be set on the template loader, which will be appended to search paths before the template name.

  @code
    loader.setTemplateDirs(QStringList() << "/home/user/app/templates" << "/usr/local/share/app/templates" );
    loader.setTheme( "simple_theme" );
    Engine::instance()->setTemplateLoader( loader );

    // This will try /home/user/app/templates/simple_theme/mytemplate.html
    // followed by /usr/local/share/app/templates/simple_theme/mytemplate.html
    Engine::instance()->loadByName( "mytemplate.html" );
  @endcode

  Media URIs may be retrieved for media relative to the directories searched queried for templates.

  @code
    loader.setTemplateDirs(QStringList() << "/home/user/app/templates" << "/usr/local/share/app/templates" );
    loader.setTheme( "simple_theme" );
    Engine::instance()->setTemplateLoader( loader );

    // This will try /home/user/app/templates/simple_theme/logo.png
    // followed by /usr/local/share/app/templates/simple_theme/logo.png
    // and return the first one that exists.
    Engine::instance()->mediaUri( "logo.png" );
  @endcode

*/
class FileSystemTemplateLoader : public AbstractTemplateLoader
{
public:
  typedef QSharedPointer<FileSystemTemplateLoader> Ptr;

  /**
    Constructor
  */
  FileSystemTemplateLoader();

  /**
    Destructor
  */
  virtual ~FileSystemTemplateLoader();

  /* reimp */ MutableTemplate loadMutableByName( const QString &name ) const;

  /* reimp */ Template loadByName( const QString &name ) const;

  /* reimp */ bool canLoadTemplate( const QString &name ) const;

  /* reimp */ QString getMediaUri( const QString& fileName ) const;

  void setTheme( const QString &themeName );
  QString themeName() const;
  void setTemplateDirs( const QStringList &dirs );

private:
  QString m_themeName;
  QStringList m_templateDirs;
};

/**
  @brief The InMemoryTemplateLoader loads Templates set dynamically in memory

  This class is mostly used for testing purposes, but can also be used for simple uses of %Grantlee.

  Templates can be made available using the setTemplate method, and will then be retrieved by the Grantlee::Engine as appropriate.
*/
class InMemoryTemplateLoader : public AbstractTemplateLoader
{
public:
  typedef QSharedPointer<InMemoryTemplateLoader> Ptr;

  InMemoryTemplateLoader();
  virtual ~InMemoryTemplateLoader();

  /* reimp */ MutableTemplate loadMutableByName( const QString &name ) const;

  /* reimp */ Template loadByName( const QString &name ) const;

  /* reimp */ bool canLoadTemplate( const QString &name ) const;

  /* reimp */ QString getMediaUri( const QString& fileName ) const;

  /**
    Add a template content to this Loader.

    Example:

    @code
      InMemoryTemplateLoader *loader = new InMemoryTemplateLoader();
      loader->setTemplate( "name_template", "My name is {{ name }}" );
      loader->setTemplate( "age_template", "My age is {{ age }}" );
      Engine::instance()->addTemplateLoader( loader );

      // Both templates may now be retrieved by calling Engine::loadByName.
    @endcode
  */
  void setTemplate( const QString &name, const QString &content );

private:
  QHash<QString, QString> m_namedTemplates;
};

}

#endif
