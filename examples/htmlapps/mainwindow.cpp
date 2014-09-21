
#include "mainwindow.h"

#include <QtCore/QDebug>
#include <QHBoxLayout>
#include <QWebView>

#include <grantlee_templates.h>
#include "grantlee_paths.h"

#include "localrequestmanager.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);

  QWebView *wv = new QWebView;

  Grantlee::Engine *engine = new Grantlee::Engine(this);
  engine->addDefaultLibrary( "customplugin" );

  QSharedPointer<Grantlee::FileSystemTemplateLoader> loader(new Grantlee::FileSystemTemplateLoader);
  loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH);
  engine->addTemplateLoader(loader);

  wv->page()->setNetworkAccessManager(new LocalRequestManager(engine, this));

  wv->load(QUrl("template:///home/main.html"));

  layout->addWidget(wv);
}
