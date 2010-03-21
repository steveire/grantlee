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

#include <QtCore>
#include <QtCore/QObject>

#include "myinterface.h"
#include "myobject.h"
#include "pluginpointer_p.h"

using Grantlee::PluginPointer;

// #define DONT_LEAK

int main( int argc, char **argv )
{
  QCoreApplication app( argc, argv );

#ifndef DONT_LEAK
  QPluginLoader loader( app.applicationDirPath() + "/myplugin.so" );
  QObject *raw_plugin = loader.instance();
  MyInterface *if1 = qobject_cast<MyInterface*>( raw_plugin );

  qDebug() << if1->double_it( 5 );
#endif

  PluginPointer<MyInterface> p1( app.applicationDirPath() + "/myplugin.so" );
  PluginPointer<MyInterface> p2;

  if ( true ) {
    PluginPointer<MyInterface> p3( app.applicationDirPath() + "/myplugin.so" );
    p1 = p3;
    PluginPointer<MyInterface> p4( p3 );
    PluginPointer<MyInterface> p5 = p3;
  }

  qDebug() << p2->double_it( 15 );

  app.exit();
}

