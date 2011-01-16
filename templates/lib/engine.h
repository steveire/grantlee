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

#ifndef GRANTLEE_ENGINE_H
#define GRANTLEE_ENGINE_H

#include "template.h"
#include "templateloader.h"

namespace Grantlee
{
class TagLibraryInterface;

class EnginePrivate;

/// @headerfile engine.h grantlee/engine.h

/**
  @brief Grantlee::Engine is the main entry point for creating %Grantlee Templates.

  The Grantlee::Engine is responsible for configuring and creating Template objects.
  In typical use, one or more TemplateLoader objects will be added to the Engine to load template objects, and
  plugin directories will be set to enable finding template tags and filters.

  @code
    Engine *engine = new Engine();

    FileSystemTemplateLoader::Ptr loader = FileSystemTemplateLoader::Ptr( new FileSystemTemplateLoader() );
    loader->setTemplateDirs( QStringList() << "/usr/share/myapp/templates" );
    engine->addTemplateLoader( loader );

    engine->addPluginPath( "/usr/lib/myapp" );

    Template template1 = engine->newTemplate( "Template content", "template name" );

    Template template2 = engine->loadByName( "templatefile.html" );
  @endcode

  Once it is configured, the engine can be used to create new templates by name by loading the templates with the loadByName method,
  or by defining the content in the newTemplate method.

  By default the builtin tags and filters distributed with %Grantlee are available in all templates without using the @gr_tag{load}
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
    Constructor
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

    @warning This overwrites the default paths. You normally want addPluginPath.

    @see @ref finding_plugins
  */
  void setPluginPaths( const QStringList &dirs );

  /**
    Prepend @p path to the list of plugin dirs.
  */
  void addPluginPath( const QString &dir );

  /**
    Removes all instances of @p dir from the list of plugin dirs.
  */
  void removePluginPath( const QString &dir );

  /**
    Returns the currently configured plugin dirs
  */
  QStringList pluginPaths() const;

  /**
    Returns a URI for a media item with the name @p name.

    Typically this will be used for images. For example the media URI for the image
    <tt>"header_logo.png"</tt> may be <tt>"/home/user/common/header_logo.png"</tt> or <tt>"/home/user/some_theme/header_logo.png"</tt>
    depending on the templateLoaders configured.

    This method will not usually be called by application code.
    To load media in a template, use the @gr_tag{media_finder} template tag.
  */
  QPair<QString, QString> mediaUri( const QString &fileName ) const;

  /**
    Load the Template identified by @p name.

    The Templates and plugins loaded will be determined by the Engine configuration.
  */
  Template loadByName( const QString &name ) const;

  /**
    Create a new Template with the content @p content identified by @p name.

    The secondary Templates and plugins loaded will be determined by the Engine configuration.
  */
  Template newTemplate( const QString &content, const QString &name ) const;

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

  bool smartTrimEnabled() const;
  void setSmartTrimEnabled( bool enabled );

#ifndef Q_QDOC
  /**
    @internal

    Loads and returns the libraries specified in defaultLibraries or @p state.
  */
  void loadDefaultLibraries();

  /**
    @internal

    Loads and returns the library specified by @p name in the current Engine configuration or @p state.

    Templates wishing to load a library should use the @gr_tag{load} tag.
  */
  TagLibraryInterface* loadLibrary( const QString &name );
#endif

private:
  Q_DECLARE_PRIVATE( Engine )
  EnginePrivate * const d_ptr;
};

}

#endif

