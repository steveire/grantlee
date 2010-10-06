/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_CACHINGLOADERDECORATOR_H
#define GRANTLEE_CACHINGLOADERDECORATOR_H

#include "templateloader.h"

#include "grantlee_core_export.h"

namespace Grantlee
{

class CachingLoaderDecoratorPrivate;

/// @headerfile cachingloaderdecorator.h grantlee/cachingloaderdecorator.h

/**
 * @brief Implements a loader decorator which caches compiled Template objects.
 *
 * The CachingLoaderDecorator can be used with any implementation of Grantlee::AbstractTemplateLoader.
 *
 * @code
 *   Grantlee::FileSystemTemplateLoader::Ptr loader( new Grantlee::FileSystemTemplateLoader() );
 *   loader->setTemplateDirs( QStringList() << "/path/one" << "/path/two" );
 *
 *   Grantlee::CachingLoaderDecorator::Ptr cache( new Grantlee::CachingLoaderDecorator( loader ) );
 *   m_engine->addTemplateLoader( cache );
 * @endcode
 *
 * The effect is that templates do not need to be loaded from the filesystem and compiled into Template
 * objects on each access, but may be returned from the cache instead.
 *
 * This can be significant if loading Templates in a loop, or loading the same Template very often in an application.
 *
 * @code
 *   <ul>
 *   {% for item in list %}
 *     <li>{% include "itemtemplate.html" %}
 *   {% endfor %}
 *   </ul>
 * @endcode
 *
 * If the loading of Templates is a bottleneck in an application, it may make sense to use the caching decorator.
 *
 */
class GRANTLEE_CORE_EXPORT CachingLoaderDecorator : public AbstractTemplateLoader
{
public:
#ifndef Q_QDOC
  typedef QSharedPointer<CachingLoaderDecorator> Ptr;
#endif
  CachingLoaderDecorator( Grantlee::AbstractTemplateLoader::Ptr loader );

  virtual ~CachingLoaderDecorator();

  /* reimp */ bool canLoadTemplate( const QString& name ) const;

  /* reimp */ QPair< QString, QString > getMediaUri( const QString& fileName ) const;

  /* reimp */ Template loadByName( const QString& name, const Grantlee::Engine* engine ) const;

  /**
   * Clears the Templates objects cached in the decorator.
   */
  void clear();

  /**
   * Returns the number of Template objects cached in the decorator.
   */
  int size() const;

  /**
   * Returns whether the cache is empty.
   */
  bool isEmpty() const;

private:
  Q_DECLARE_PRIVATE(CachingLoaderDecorator)
  CachingLoaderDecoratorPrivate * const d_ptr;
};

}


#endif
