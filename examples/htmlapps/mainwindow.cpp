/*
  This file is part of the Grantlee template system.

  Copyright (c) 2011 Stephen Kelly <steveire@gmail.com>

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

#include <QHBoxLayout>
#include <QWebView>
#include <QtCore/QDebug>

#include "grantlee_paths.h"
#include <grantlee_templates.h>

#include "localrequestmanager.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);

  QWebView *wv = new QWebView;

  Grantlee::Engine *engine = new Grantlee::Engine(this);
  engine->addDefaultLibrary("customplugin");

  QSharedPointer<Grantlee::FileSystemTemplateLoader> loader(
      new Grantlee::FileSystemTemplateLoader);
  loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH);
  engine->addTemplateLoader(loader);

  wv->page()->setNetworkAccessManager(new LocalRequestManager(engine, this));

  wv->load(QUrl("template:///home/main.html"));

  layout->addWidget(wv);
}
