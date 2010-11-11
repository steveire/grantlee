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

#include "mainwindow.h"

#include "kdelocalizer.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>

#include "grantlee_paths.h"

#define TEMPLATE_DIR GRANTLEE_TEMPLATE_PATH "/gettext/"

int main( int argc, char **argv )
{
  const QByteArray& ba = QByteArray( "contacts_kde" );
  const KLocalizedString name = ki18n( "Grantlee KDE i18n example" );
  KAboutData aboutData( ba, ba, name, ba, name );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

  AppMainWindow<KDELocalizer> win(TEMPLATE_DIR);
  win.show();

  app.exec();
}

