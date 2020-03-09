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

#ifndef GRANTLEE_TEMPLATELOADER_H
#define GRANTLEE_TEMPLATELOADER_H

#include "grantlee_templates_export.h"
#include "template.h"

#include <QtCore/QSharedPointer>

namespace Grantlee
{

class AbstractLocalizer;

/// @headerfile templateloader.h grantlee/templateloader.h

/**
  @brief An retrieval interface to a storage location for Template objects.

  This interface can be implemented to define new ways of retrieving the content
  of Templates.

  The interface of this class should not be called directly from applications.
  TemplateLoaders will typically be created, configured and added to the
  Grantlee::Engine which will call the appropriate API.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TEMPLATES_EXPORT AbstractTemplateLoader
{
public:
  /**
    Destructor
  */
  virtual ~AbstractTemplateLoader();

  /**
    Load a Template called @p name. Return an invalid Template if no content
    by that name exists.
  */
  virtual Template loadByName(const QString &name,
                              Engine const *engine) const = 0;

  /**
    Return a complete URI for media identified by fileName.
  */
  virtual QPair<QString, QString>
  getMediaUri(const QString &fileName) const = 0;

  /**
    Return true if a Template identified by @p name exists and can be loaded.
  */
  virtual bool canLoadTemplate(const QString &name) const = 0;
};

/// @headerfile templateloader.h grantlee/templateloader.h

class FileSystemTemplateLoaderPrivate;

/**
  @brief The **%FileSystemTemplateLoader** loads Templates from the file system.

  This template loader works by traversing a list of directories to find
  templates. Directories are checked in order, and the first match hit is parsed
  and returned.

  @code
    loader->setTemplateDirs({
        "/home/user/app/templates",
        "/usr/local/share/app/templates"
    });
    engine->setTemplateLoader( loader );

    // This will try /home/user/app/templates/mytemplate.html
    // followed by /usr/local/share/app/templates/mytemplate.html
    engine->loadByName( "mytemplate.html" );
  @endcode

  Additionally, a themeName may be set on the template loader, which will be
  appended to search paths before the template name.

  @code
    loader->setTemplateDirs({
      "/home/user/app/templates" <<
      "/usr/local/share/app/templates"
    });
    loader->setTheme( "simple_theme" );
    engine->setTemplateLoader( loader );

    // This will try /home/user/app/templates/simple_theme/mytemplate.html
    // followed by /usr/local/share/app/templates/simple_theme/mytemplate.html
    engine->loadByName( "mytemplate.html" );
  @endcode

  Media URIs may be retrieved for media relative to the directories searched
  queried for templates.

  @code
    loader->setTemplateDirs({
      "/home/user/app/templates",
      "/usr/local/share/app/templates"
    });
    loader->setTheme( "simple_theme" );
    engine->setTemplateLoader( loader );

    // This will try /home/user/app/templates/simple_theme/logo.png
    // followed by /usr/local/share/app/templates/simple_theme/logo.png
    // and return the first one that exists.
    engine->mediaUri( "logo.png" );
  @endcode

  The template files loaded by a %**FileSystemTemplateLoader** must be UTF-8
  encoded.

  @see @ref deploying_templates

*/
class GRANTLEE_TEMPLATES_EXPORT FileSystemTemplateLoader
    : public AbstractTemplateLoader
{
public:
  /**
    Constructor
  */
  FileSystemTemplateLoader(const QSharedPointer<AbstractLocalizer> localizer
                           = {});

  /**
    Destructor
  */
  ~FileSystemTemplateLoader() override;

  Template loadByName(const QString &name, Engine const *engine) const override;

  bool canLoadTemplate(const QString &name) const override;

  QPair<QString, QString> getMediaUri(const QString &fileName) const override;

  /**
    Sets the theme of this loader to @p themeName
  */
  void setTheme(const QString &themeName);

  /**
    The themeName of this TemplateLoader
  */
  QString themeName() const;

  /**
    Sets the directories to look for template files to @p dirs.
  */
  void setTemplateDirs(const QStringList &dirs);

  /**
    The directories this TemplateLoader looks in for template files.
   */
  QStringList templateDirs() const;

private:
  Q_DECLARE_PRIVATE(FileSystemTemplateLoader)
  FileSystemTemplateLoaderPrivate *const d_ptr;
};

/// @headerfile templateloader.h grantlee/templateloader.h

/**
  @brief The **%InMemoryTemplateLoader** loads Templates set dynamically in
  memory

  This class is mostly used for testing purposes, but can also be used for
  simple uses of %Grantlee.

  Templates can be made available using the @ref setTemplate method, and will
  then be retrieved by the Grantlee::Engine as appropriate.
*/
class GRANTLEE_TEMPLATES_EXPORT InMemoryTemplateLoader
    : public AbstractTemplateLoader
{
public:
  InMemoryTemplateLoader();
  ~InMemoryTemplateLoader() override;

  Template loadByName(const QString &name, Engine const *engine) const override;

  bool canLoadTemplate(const QString &name) const override;

  QPair<QString, QString> getMediaUri(const QString &fileName) const override;

  /**
    Add a template content to this Loader.

    Example:

    @code
      auto loader = QSharedPointer<InMemoryTemplateLoader::create();
      loader->setTemplate( "name_template", "My name is {{ name }}" );
      loader->setTemplate( "age_template", "My age is {{ age }}" );
      engine->addTemplateLoader( loader );

      // Both templates may now be retrieved by calling Engine::loadByName.
    @endcode
  */
  void setTemplate(const QString &name, const QString &content);

private:
  QHash<QString, QString> m_namedTemplates;
};
}

#endif
