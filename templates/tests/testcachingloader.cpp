/*
  This file is part of the Grantlee template system.

  Copyright (c) 2015 Stephen Kelly <steveire@gmail.com>

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

#include <QtTest/QTest>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "filterexpression.h"
#include "grantlee_paths.h"
#include "coverageobject.h"
#include "cachingloaderdecorator.h"
#include <metaenumvariable_p.h>

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestCachingLoader : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void testRenderAfterError();
};

void TestCachingLoader::testRenderAfterError()
{
  Engine engine;
  engine.setPluginPaths( QStringList() << QStringLiteral( GRANTLEE_PLUGIN_PATH ) );

  QSharedPointer<InMemoryTemplateLoader> loader(new InMemoryTemplateLoader);
  loader->setTemplate(QLatin1String("template1"),
                      QLatin1String("This template has an error {{ va>r }}"));
  loader->setTemplate(QLatin1String("template2"),
                      QLatin1String("Ok"));
  loader->setTemplate(QLatin1String("main"),
                      QLatin1String("{% include template_var %}"));

  QSharedPointer<Grantlee::CachingLoaderDecorator> cache( new Grantlee::CachingLoaderDecorator( loader ) );

  engine.addTemplateLoader( cache );

  Context c;
  Template t;
  t = engine.loadByName(QLatin1String("main"));

  c.insert(QLatin1String("template_var"), QLatin1String("template1"));
  QCOMPARE(t->render(&c), QString());
  QCOMPARE(t->error(), TagSyntaxError);

  c.insert(QLatin1String("template_var"), QLatin1String("template2"));
  QCOMPARE(t->render(&c), QLatin1String("Ok"));
  QCOMPARE(t->error(), NoError);
}

QTEST_MAIN(TestCachingLoader)
#include "testcachingloader.moc"
