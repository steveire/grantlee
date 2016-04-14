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

#include "engine.h"
#include "engine_p.h"

#include "exception.h"
#include "grantlee_config_p.h"
#include "grantlee_version.h"
#ifdef QT_SCRIPT_LIB
#include "scriptabletags.h"
#endif
#include "template_p.h"
#include "templateloader.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QTextStream>

using namespace Grantlee;

static const char __scriptableLibName[] = "grantlee_scriptabletags";

Engine::Engine(QObject *parent)
    : QObject(parent), d_ptr(new EnginePrivate(this))
{
  QMetaType::registerComparators<SafeString>();
  d_ptr->m_defaultLibraries << QStringLiteral("grantlee_defaulttags")
                            << QStringLiteral("grantlee_loadertags")
                            << QStringLiteral("grantlee_defaultfilters");

  d_ptr->m_pluginDirs = QCoreApplication::instance()->libraryPaths();
  d_ptr->m_pluginDirs << QString::fromLocal8Bit(GRANTLEE_PLUGIN_PATH);
}

Engine::~Engine()
{
#ifdef QT_SCRIPT_LIB
  qDeleteAll(d_ptr->m_scriptableLibraries);
#endif
  d_ptr->m_libraries.clear();
  delete d_ptr;
}

QList<QSharedPointer<AbstractTemplateLoader>> Engine::templateLoaders()
{
  Q_D(Engine);
  return d->m_loaders;
}

void Engine::addTemplateLoader(QSharedPointer<AbstractTemplateLoader> loader)
{
  Q_D(Engine);
  d->m_loaders << loader;
}

QPair<QString, QString> Engine::mediaUri(const QString &fileName) const
{
  Q_D(const Engine);
  QListIterator<QSharedPointer<AbstractTemplateLoader>> it(d->m_loaders);

  QPair<QString, QString> uri;
  while (it.hasNext()) {
    const auto loader = it.next();
    uri = loader->getMediaUri(fileName);
    if (!uri.second.isEmpty())
      break;
  }
  return uri;
}

void Engine::setPluginPaths(const QStringList &dirs)
{
  Q_D(Engine);
  d->m_pluginDirs = dirs;
}

void Engine::addPluginPath(const QString &dir)
{
  Q_D(Engine);
  QStringList temp;
  temp << dir;
  temp << d->m_pluginDirs;
  d->m_pluginDirs = temp;
}

void Engine::removePluginPath(const QString &dir)
{
  Q_D(Engine);
  d->m_pluginDirs.removeAll(dir);
}

QStringList Engine::pluginPaths() const
{
  Q_D(const Engine);
  return d->m_pluginDirs;
}

QStringList Engine::defaultLibraries() const
{
  Q_D(const Engine);
  return d->m_defaultLibraries;
}

void Engine::addDefaultLibrary(const QString &libName)
{
  Q_D(Engine);
  d->m_defaultLibraries << libName;
}

void Engine::removeDefaultLibrary(const QString &libName)
{
  Q_D(Engine);
  d->m_defaultLibraries.removeAll(libName);
}

template <uint v> bool acceptableVersion(uint minorVersion)
{
  return minorVersion >= v;
}

template <> bool acceptableVersion<0>(uint) { return true; }

void Engine::loadDefaultLibraries()
{
  Q_D(Engine);

#ifdef QT_SCRIPT_LIB
  // Make sure we can load default scriptable libraries if we're supposed to.
  if (d->m_defaultLibraries.contains(QLatin1String(__scriptableLibName))
      && !d->m_scriptableTagLibrary) {
    d->m_scriptableTagLibrary = new ScriptableTagLibrary(this);

// It would be better to load this as a plugin, but that is not currently
// possible with webkit/javascriptcore
// so we new the library directly.
// https://bugs.webkit.org/show_bug.cgi?id=38193
#if 0
    d->loadCppLibrary( __scriptableLibName, GRANTLEE_VERSION_MINOR );
    PluginPointer<TagLibraryInterface> library = d->loadCppLibrary( __scriptableLibName, GRANTLEE_VERSION_MINOR );
    if ( !library )
      throw Grantlee::Exception( TagSyntaxError, QStringLiteral("Could not load scriptable tags library") );
#endif
  }
#endif

  Q_FOREACH (const QString &libName, d->m_defaultLibraries) {
    if (libName == QLatin1String(__scriptableLibName))
      continue;

    // already loaded by the engine.
    if (d->m_libraries.contains(libName))
      continue;

    // Warning. We load C++ plugins in this method, but not plugins written
    // in
    // QtScript.
    // This should be a better situation in Grantlee 0.2 when the
    // TagLibraryInterface
    // can have shared pointers instead of raw pointers in its API. The
    // whole
    // scriptable
    // thing likely needs to be redesigned too.
    // The reason for explicitly not loading scripted plugins here is that
    // they
    // create new
    // NodeFactory and Filter instances with each call to loadLibrary.
    // NodeFactories are memory-managed by the Parser, as are Filters to an
    // extent, because it
    // creates shared pointers for Filters.
    // Because this method loads the libraries but doesn't actually use them
    // or
    // send them
    // back to the Parser the scriptable NodeFactories and Filters but be
    // deleted immediately.
    // The C++ plugins are different because although they are loaded, their
    // NodeFactories
    // and Filters are accessed only by the Parser, which manages them after
    // that.
    uint minorVersion = GRANTLEE_VERSION_MINOR;
    while (acceptableVersion<GRANTLEE_MIN_PLUGIN_VERSION>(minorVersion)) {
#ifdef QT_SCRIPT_LIB
      // Although we don't use scripted libaries here, we need to
      // recognize them
      // being first
      // in the search path and not load a c++ plugin of the same name in
      // that
      // case.
      auto scriptableLibrary = d->loadScriptableLibrary(libName, minorVersion);
      if (scriptableLibrary) {
        scriptableLibrary->clear();
        break;
      }
#endif

      auto library = d->loadCppLibrary(libName, minorVersion);
      if (minorVersion == 0)
        break;
      minorVersion--;
      if (library)
        break;
    }
  }
}

TagLibraryInterface *Engine::loadLibrary(const QString &name)
{
  Q_D(Engine);

#ifdef QT_SCRIPT_LIB
  if (name == QLatin1String(__scriptableLibName))
    return 0;
#endif

  // already loaded by the engine.
  if (d->m_libraries.contains(name))
    return d->m_libraries.value(name).data();

  uint minorVersion = GRANTLEE_VERSION_MINOR;
  while (acceptableVersion<GRANTLEE_MIN_PLUGIN_VERSION>(minorVersion)) {
    auto library = d->loadLibrary(name, minorVersion);
    if (library)
      return library;
    if (minorVersion == 0)
      break;
    minorVersion--;
  }
  throw Grantlee::Exception(
      TagSyntaxError,
      QStringLiteral("Plugin library '%1' not found.").arg(name));
  return 0;
}

TagLibraryInterface *EnginePrivate::loadLibrary(const QString &name,
                                                uint minorVersion)
{
#ifdef QT_SCRIPT_LIB
  auto scriptableLibrary = loadScriptableLibrary(name, minorVersion);
  if (scriptableLibrary)
    return scriptableLibrary;

// else this is not a scriptable library.
#endif

  return loadCppLibrary(name, minorVersion).data();
}

EnginePrivate::EnginePrivate(Engine *engine)
    : q_ptr(engine)
#ifdef QT_SCRIPT_LIB
      ,
      m_scriptableTagLibrary(0)
#endif
      ,
      m_smartTrimEnabled(false)
{
}

QString EnginePrivate::getScriptLibraryName(const QString &name,
                                            uint minorVersion) const
{
  auto pluginIndex = 0;
  const QString prefix
      = QStringLiteral("/grantlee/") + QString::number(GRANTLEE_VERSION_MAJOR)
        + QLatin1Char('.') + QString::number(minorVersion) + QLatin1Char('/');
  while (m_pluginDirs.size() > pluginIndex) {
    const auto nextDir = m_pluginDirs.at(pluginIndex++);
    const QString libFileName = nextDir + prefix + name + QStringLiteral(".qs");

    const QFile file(libFileName);
    if (!file.exists())
      continue;
    return libFileName;
  }
  auto it = m_loaders.constBegin();
  const auto end = m_loaders.constEnd();
  for (; it != end; ++it) {
    const auto pair = (*it)->getMediaUri(prefix + name + QStringLiteral(".qs"));

    if (!pair.first.isEmpty() && !pair.second.isEmpty()) {
      return pair.first + pair.second;
    }
  }
  return QString();
}

#ifdef QT_SCRIPT_LIB
ScriptableLibraryContainer *
EnginePrivate::loadScriptableLibrary(const QString &name, uint minorVersion)
{
  if (!m_scriptableTagLibrary)
    return 0;

#if 0
  if ( !m_libraries.contains( __scriptableLibName ) )
    return 0;
#endif

  const auto libFileName = getScriptLibraryName(name, minorVersion);

  if (libFileName.isEmpty())
    return 0;

  if (m_scriptableLibraries.contains(libFileName)) {
    auto library = m_scriptableLibraries.value(libFileName);
    library->setNodeFactories(
        m_scriptableTagLibrary->nodeFactories(libFileName));
    library->setFilters(m_scriptableTagLibrary->filters(libFileName));
    return library;
  }
#if 0
  PluginPointer<TagLibraryInterface> scriptableTagLibrary = m_libraries.value( __scriptableLibName );
#endif

  const auto factories = m_scriptableTagLibrary->nodeFactories(libFileName);
  const auto filters = m_scriptableTagLibrary->filters(libFileName);

  auto library = new ScriptableLibraryContainer(factories, filters);
  m_scriptableLibraries.insert(libFileName, library);
  return library;
}
#endif

PluginPointer<TagLibraryInterface>
EnginePrivate::loadCppLibrary(const QString &name, uint minorVersion)
{
  auto pluginIndex = 0;

  while (m_pluginDirs.size() > pluginIndex) {
    const auto nextDir = m_pluginDirs.at(pluginIndex++);
    const QString pluginDirString
        = nextDir + QStringLiteral("/grantlee/")
          + QString::number(GRANTLEE_VERSION_MAJOR) + QLatin1Char('.')
          + QString::number(minorVersion) + QLatin1Char('/');

    const QDir pluginDir(pluginDirString);

    if (!pluginDir.exists())
      continue;

    auto list = pluginDir.entryList(QStringList(name
#if PLUGINS_PREFER_DEBUG_POSTFIX
                                                + QLatin1Char('d')
#endif
                                                + QLatin1Char('*')));

    if (list.isEmpty()) {
#if PLUGINS_PREFER_DEBUG_POSTFIX
      list = pluginDir.entryList(QStringList(name + QLatin1Char('*')));
      if (list.isEmpty())
        continue;
#else
      continue;
#endif
    }

    auto pluginPath = pluginDir.absoluteFilePath(list.first());
    auto plugin = PluginPointer<TagLibraryInterface>(pluginPath);

    if (plugin) {
#ifdef __COVERAGESCANNER__
      __coveragescanner_register_library(pluginPath.toLatin1().data());
#endif
      m_libraries.insert(name, plugin);
      return plugin;
    }
  }
  return 0;
}

Template Engine::loadByName(const QString &name) const
{
  Q_D(const Engine);

  QListIterator<QSharedPointer<AbstractTemplateLoader>> it(d->m_loaders);
  while (it.hasNext()) {
    const auto loader = it.next();

    if (!loader->canLoadTemplate(name))
      continue;

    const auto t = loader->loadByName(name, this);

    if (t) {
      return t;
    }
  }
  auto t = Template(new TemplateImpl(this));
  t->setObjectName(name);
  t->d_ptr->m_error = TagSyntaxError;
  t->d_ptr->m_errorString = QStringLiteral("Template not found, %1").arg(name);
  return t;
}

Template Engine::newTemplate(const QString &content, const QString &name) const
{
  Q_D(const Engine);
  auto t = Template(new TemplateImpl(this, d->m_smartTrimEnabled));
  t->setObjectName(name);
  t->setContent(content);
  return t;
}

void Engine::setSmartTrimEnabled(bool enabled)
{
  Q_D(Engine);
  d->m_smartTrimEnabled = enabled;
}

bool Engine::smartTrimEnabled() const
{
  Q_D(const Engine);
  return d->m_smartTrimEnabled;
}
