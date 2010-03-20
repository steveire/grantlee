/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_ENGINE_H
#define GRANTLEE_ENGINE_H

#include "mutabletemplate.h"
#include "template.h"
#include "templateloader.h"

namespace Grantlee
{
class TagLibraryInterface;

class EnginePrivate;

/**
  @brief Grantlee::Engine is the main entry point for creating %Grantlee Templates.

  The Grantlee::Engine is a singleton responsible for configuring and creating Template and MutableTemplate objects.
  In typical use, one or more TemplateLoader objects will be added to the Engine to load template objects, and
  plugin directories will be set to enable finding template tags and filters.

  @code
    Engine *engine = new Engine();

    FileSystemTemplateLoader::Ptr loader = FileSystemTemplateLoader::Ptr( new FileSystemTemplateLoader() );
    loader->setTemplateDirs( QStringList() << "/usr/share/myapp/templates" );
    engine->addTemplateLoader( loader );

    engine->setPluginDirs( QStringList() << GRANTLEE_PLUGIN_PATH );
    Template template = engine->newTemplate( "Template content", "template name" );
  @endcode

  Once it is configured, the engine can be used to create new templates by name by loading the templates with the loadByName method,
  or by defining the content in the newTemplate method.

  By default the builtin tags and filters distributed with grantlee are available in all templates without using the "{% load %}"
  tag in the template. These pre-loaded libraries may be configured if appropriate to the application. For example, an application
  which defines its own tags and filters may want them to be always available, or it may be desirable to restrict the features
  available to template authors by removing built in libraries.

  Different Engine objects can be used to create templates with differing configurations.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_CORE_EXPORT Engine : public QObject
{
  Q_OBJECT
public:
  /**
    Retrieve an instance of an Engine.
  */
  Engine( QObject *parent = 0 );

  /**
    Destructor.
  */
  ~Engine();

  /**
    Returns the TemplateLoaders currently configured on the Engine.
  */
  QList<AbstractTemplateLoader::Ptr> templateLoaders();

  /**
    Adds @p loader to the TemplateLoaders currently configured on the Engine.
  */
  void addTemplateLoader( AbstractTemplateLoader::Ptr loader );

  /**
    Sets the plugin dirs currently configured on the Engine to @p dirs.
  */
  void setPluginDirs( const QStringList &dirs );

  /**
    Returns a URI for a media item with the name @p name.

    Typically this will be used for images. For example the media URI for the image
    "header_logo.png" may be "/home/user/common/header_logo.png" or "/home/user/some_theme/header_logo.png"
    depending on the templateLoaders configured.

    This method will not usually be called by application code.
    To load media in a template, use the {% media_finder %} template tag.
  */
  QString mediaUri( const QString &fileName ) const;

  /**
    Resets the state of the Engine to the default.

    This is used if an application requires multiple engine states for creating templates.
    @see template_factories
  */
  void resetState();

  /**
    Load the Template identified by @p name with the optionally supplied EngineState.

    The Templates and plugins loaded and will be determined by the Engine or the EngineState @p state.
  */
  Template loadByName( const QString &name ) const;

  /**
    Create a new Template with the content @p content identified by @p name with the optionally supplied EngineState.

    The secondary Templates and plugins loaded will be determined by the Engine or the EngineState @p state.
  */
  Template newTemplate( const QString &content, const QString &name ) const;

  /**
    Load the MutableTemplate identified by @p name with the optionally supplied EngineState.

    The Templates and plugins loaded and will be determined by the Engine or the EngineState @p state.
  */
  MutableTemplate loadMutableByName( const QString &name ) const;

  /**
    Create a new MutableTemplate with the content @p content identified by @p name with the optionally supplied EngineState.

    The secondary Templates and plugins loaded will be determined by the Engine or the EngineState @p state.
  */
  MutableTemplate newMutableTemplate( const QString &content, const QString &name ) const;

  /**
    Returns the libraries available by default to new Templates.
  */
  QStringList defaultLibraries() const;

  /**
    Adds the library named @p libName to the libraries available by default to new Templates.
  */
  void addDefaultLibrary( const QString &libName );

  /**
    Removes the library named @p libName from the libraries available by default to new Templates.
  */
  void removeDefaultLibrary( const QString &libName );

  /**
    @internal

    Loads and returns the libraries specified in defaultLibraries or @p state.
  */
  void loadDefaultLibraries();

  /**
    @internal

    Loads and returns the library specified by @p name in the current Engine configuration or @p state.

    Templates wishing to load a library should use the "{% load %}" tag.
  */
  TagLibraryInterface* loadLibrary( const QString &name );

  Q_DECLARE_PRIVATE( Engine )
  EnginePrivate * const d_ptr;
};

}

#endif

