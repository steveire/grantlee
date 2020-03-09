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

#ifndef GRANTLEE_PLUGINPOINTER_H
#define GRANTLEE_PLUGINPOINTER_H

#include <QtCore/QPluginLoader>
#include <QtCore/QSharedPointer>

namespace Grantlee
{

/**
  @brief A smart pointer for handling plugins.

  @author Stephen Kelly <steveire@gmail.com>
*/
template <typename PluginType> class PluginPointer
{
  class _Dummy;

public:
  // This allows returning 0 from a function returning a PluginType*
  PluginPointer(_Dummy * = {}) : m_plugin(0) {}

  PluginPointer(const QString &fileName) : m_object(0), m_plugin(0)
  {
    m_pluginLoader = QSharedPointer<QPluginLoader>(new QPluginLoader(fileName));

    // This causes a load of the plugin, and we never call unload() to
    // unload
    // it. Unloading it would cause the destructors of all types defined in
    // plugins
    // to be unreachable. If a Template object outlives the last engine,
    // that
    // causes segfaults if the plugin has been unloaded.
    m_object = m_pluginLoader->instance();

    m_plugin = qobject_cast<PluginType *>(m_object);
  }

  QString errorString() { return m_pluginLoader->errorString(); }

  QObject *object() { return m_object; }

  PluginType *operator->() { return m_plugin; }

  operator bool() { return m_plugin ? true : false; }

  PluginType *data() const { return m_plugin; }

private:
  QObject *m_object;
  PluginType *m_plugin;
  QSharedPointer<QPluginLoader> m_pluginLoader;
};
}

#endif
