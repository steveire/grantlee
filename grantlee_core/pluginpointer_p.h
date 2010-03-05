/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_PLUGINPOINTER_H
#define GRANTLEE_PLUGINPOINTER_H

#include <QtCore/QSharedPointer>
#include <QtCore/QPluginLoader>

namespace Grantlee
{

void _deleter( QPluginLoader *loader )
{
  loader->unload();
  delete loader;
}

template <typename PluginType>
class PluginPointer
{
  class _Dummy;
public:
  PluginPointer()
    : m_plugin( 0 )
  {

  }

  // This allows returning 0 from a function returning a PluginType*
  PluginPointer( _Dummy* )
    : m_plugin( 0 )
  {

  }

  PluginPointer( const QString &fileName )
    : m_plugin( 0 )
  {
    m_pluginLoader = QSharedPointer<QPluginLoader>( new QPluginLoader( fileName ), _deleter );

    QObject *plugin = m_pluginLoader->instance();
    if ( !plugin )
        return;

    m_plugin = qobject_cast<PluginType*>( plugin );
  }

  PluginType* operator->()
  {
    return m_plugin;
  }

  operator bool()
  {
    return m_plugin ? true : false;
  }

  PluginType* data() const
  {
    return m_plugin;
  }

private:
  PluginType *m_plugin;
  QSharedPointer<QPluginLoader> m_pluginLoader;
};

}

#endif
