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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QListWidget>
#include <QWebView>
#include <grantlee/templateloader.h>

namespace Grantlee {
class Engine;
}

class QComboBox;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  MainWindow(const QString &templateDir, QWidget* parent = 0, Qt::WindowFlags f = 0);

protected:
  virtual void initLocalizer();

private Q_SLOTS:
  void render();
  void delayedInit();

protected:
  Grantlee::AbstractLocalizer::Ptr m_localizer;

private:
  QListWidget *m_list;
  QWebView *m_webView;
  QComboBox *m_combo;
  QString m_templateDir;

  Grantlee::Engine *m_engine;
  Grantlee::LocalizedFileSystemTemplateLoader::Ptr  m_templateLoader;
};

template<typename T>
class AppMainWindow : public MainWindow
{
public:
  AppMainWindow(const QString& templateDir, QWidget* parent = 0, Qt::WindowFlags f = 0)
    : MainWindow(templateDir, parent, f)
  {

  }
protected:
  virtual void initLocalizer()
  {
    m_localizer = Grantlee::AbstractLocalizer::Ptr( new T );
  }
};


#endif
