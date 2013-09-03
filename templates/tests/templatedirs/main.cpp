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

#include <QCoreApplication>
#include <QDebug>

#include <grantlee_templates.h>

#include "grantlee_paths.h"

static void renderTemplate( Grantlee::Engine *engine, const QString &templateName )
{

  Grantlee::Template t = engine->loadByName( templateName );

  if ( t->error() ) {
    qDebug() << t->errorString();
    Q_ASSERT( !"Failed to get template" );
  }

  Grantlee::Context c;

  QString result = t->render( &c );

  if ( t->error() ) {
    qDebug() << t->errorString();
    Q_ASSERT( !"Failed to render template" );
  }

  qDebug() << result;
}

int main( int argc, char **argv )
{
  QCoreApplication app( argc, argv );

  Grantlee::Engine *engine = new Grantlee::Engine();

  Grantlee::FileSystemTemplateLoader::Ptr loader = Grantlee::FileSystemTemplateLoader::Ptr( new Grantlee::FileSystemTemplateLoader() );
  loader->setTemplateDirs( QStringList()
                           << ( QCoreApplication::applicationDirPath() + "/template_dir_1" )
                           << ( QCoreApplication::applicationDirPath() + "/template_dir_2" )
                         );

  engine->addTemplateLoader( loader );

  engine->setPluginPaths( QStringList() << GRANTLEE_PLUGIN_PATH );
  qDebug() << "Expect: \"Template 1 content\"";
  renderTemplate( engine, "template1.txt" );
  qDebug() << "Expect: \"Template 2 content\"";
  renderTemplate( engine, "template2.txt" );
  qDebug() << "Expect: \"Template 4 content\"";
  renderTemplate( engine, "template3.txt" );
  qDebug() << "Expect: \"Template 5 content\"";
  renderTemplate( engine, "template6.txt" );
  qDebug() << "Expect: \n" "file://" + QCoreApplication::applicationDirPath() + "/template_dir_2/red.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_2/blue.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_1/black.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_1/white.png";
  renderTemplate( engine, "mediatemplate1.txt" );
  qDebug() << "Expect: \n" "file://" + QCoreApplication::applicationDirPath() + "/template_dir_1/black.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_1/white.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_2/red.png\n"
                             "file://" + QCoreApplication::applicationDirPath() + "/template_dir_2/blue.png";
  renderTemplate( engine, "mediatemplate2.txt" );

  app.exit( 0 );
}

