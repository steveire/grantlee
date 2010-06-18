
#include <QCoreApplication>
#include <QDebug>

#include <grantlee_core.h>

#include "grantlee_paths.h"

static void renderTemplate( Grantlee::Engine *engine, const QString &templateName ) {

  Grantlee::Template t = engine->loadByName( templateName );

  if ( t->error() )
  {
    qDebug() << t->errorString();
    Q_ASSERT(!"Failed to get template");
  }

  Grantlee::Context c;

  QString result = t->render(&c);

  if ( t->error() )
  {
    qDebug() << t->errorString();
    Q_ASSERT(!"Failed to render template");
  }

  qDebug() << result;
}

int main(int argc, char **argv)
{
  QCoreApplication app(argc, argv);

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

  app.exit(0);
}

