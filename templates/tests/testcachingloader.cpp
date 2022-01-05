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

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtTest/QTest>

#include "cachingloaderdecorator.h"
#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "filterexpression.h"
#include "grantlee_paths.h"
#include "template.h"
#include <metaenumvariable_p.h>

using Dict = QHash<QString, QVariant>;

Q_DECLARE_METATYPE(Grantlee::Error)

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
  engine.setPluginPaths({QStringLiteral(GRANTLEE_PLUGIN_PATH)});

  QSharedPointer<InMemoryTemplateLoader> loader(new InMemoryTemplateLoader);
  loader->setTemplate(QStringLiteral("template1"),
                      QStringLiteral("This template has an error {{ va>r }}"));
  loader->setTemplate(QStringLiteral("template2"), QStringLiteral("Ok"));
  loader->setTemplate(QStringLiteral("main"),
                      QStringLiteral("{% include template_var %}"));

  QSharedPointer<Grantlee::CachingLoaderDecorator> cache(
      new Grantlee::CachingLoaderDecorator(loader));

  engine.addTemplateLoader(cache);

  Context c;
  Template t;
  t = engine.loadByName(QStringLiteral("main"));

  c.insert(QStringLiteral("template_var"), QLatin1String("template1"));
  QCOMPARE(t->render(&c), QString());
  QCOMPARE(t->error(), TagSyntaxError);

  c.insert(QStringLiteral("template_var"), QLatin1String("template2"));
  QCOMPARE(t->render(&c), QLatin1String("Ok"));
  QCOMPARE(t->error(), NoError);
}

QTEST_MAIN(TestCachingLoader)
#include "testcachingloader.moc"
