/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef SCRIPTABLETAGSTEST_H
#define SCRIPTABLETAGSTEST_H

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtTest/QTest>

#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "filterexpression.h"
#include "grantlee_paths.h"
#include "template.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestScriptableTagsSyntax : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() { doTest(); }

  void testResolve_data();
  void testResolve() { doTest(); }

  void cleanupTestCase();

private:
  void doTest();

  Engine *m_engine;
};

void TestScriptableTagsSyntax::initTestCase()
{
  Q_INIT_RESOURCE(testresource);

  m_engine = new Engine(this);
  auto appDirPath
      = QFileInfo(QCoreApplication::applicationDirPath()).absoluteDir().path();
  m_engine->setPluginPaths({
      QStringLiteral(GRANTLEE_PLUGIN_PATH),
      QStringLiteral(":/plugins/") // For scripteddefaults.qs
  });
  m_engine->addDefaultLibrary(QStringLiteral("grantlee_scriptabletags"));
}

void TestScriptableTagsSyntax::cleanupTestCase() { delete m_engine; }

void TestScriptableTagsSyntax::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

  if (t->error() != NoError) {
    if (t->error() != error)
      qDebug() << t->errorString();
    QCOMPARE(t->error(), error);
    return;
  }

  Context context(dict);

  auto result = t->render(&context);

  if (t->error() != NoError) {
    if (t->error() != error)
      qDebug() << t->errorString();
    QCOMPARE(t->error(), error);
    return;
  }

  QCOMPARE(t->error(), NoError);

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  QCOMPARE(t->error(), NoError);

  QCOMPARE(result, output);
}

void TestScriptableTagsSyntax::testBasicSyntax_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("boo"), QStringLiteral("Far"));
  dict.insert(QStringLiteral("booList"),
              QVariantList{QStringLiteral("Tom"), QStringLiteral("Dick"),
                           QStringLiteral("Harry")});

  QTest::newRow("scriptable-tags01")
      << "{% load scripteddefaults %}{% if2 "
         "\"something\\\" stupid\" %}{{ boo "
         "}}{% endif2 %}"
      << dict << QStringLiteral("Far") << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow("scriptable-tags02")
      << "{% load scripteddefaults %}{% if2 \"something\\\" stupid\" %}{% "
         "for "
         "name in booList %}:{{ name }};{% endfor %}{% endif2 %}"
      << dict << QStringLiteral(":Tom;:Dick;:Harry;") << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow("scriptable-tags03") << QStringLiteral(
      "{% load scripteddefaults %}{% if2 boo %}yes{% else %}no{% endif2 %}")
                                     << dict << QStringLiteral("yes")
                                     << NoError;
  QTest::newRow("scriptable-tags04") << QStringLiteral(
      "{% load scripteddefaults %}{% if2 foo %}yes{% else %}no{% endif2 %}")
                                     << dict << QStringLiteral("no") << NoError;

  QTest::newRow("scriptable-tags05")
      << QStringLiteral("{% load scripteddefaults %}{{ boo|upper }}") << dict
      << QStringLiteral("FAR") << NoError;

  dict.insert(QStringLiteral("boo"), QStringLiteral("Far & away"));
  // Variables are escaped ...
  QTest::newRow("scriptable-tags06")
      << QStringLiteral("{% load scripteddefaults %}{{ boo }}") << dict
      << QStringLiteral("Far &amp; away") << NoError;
  // and scripted filters can mark output as 'safe'.
  QTest::newRow("scriptable-tags07")
      << QStringLiteral("{% load scripteddefaults %}{{ boo|safe2 }}") << dict
      << QStringLiteral("Far & away") << NoError;

  // Filters can take arguments
  QTest::newRow("scriptable-tags08")
      << "{% load scripteddefaults %}{{ booList|join2:\" \" }}" << dict
      << QStringLiteral("Tom Dick Harry") << NoError;

  // Literals in tags are compared un-escaped.
  QTest::newRow("scriptable-tags09")
      << "{% load scripteddefaults %}{% ifequal2 boo \"Far & away\" %}yes{% "
         "endifequal2 %}"
      << dict << QStringLiteral("yes") << NoError;

  // Literals in arguments to filters are not escaped.
  QTest::newRow("scriptable-tags10")
      << "{% load scripteddefaults %}{{ booList|join2:\" & \" }}" << dict
      << QStringLiteral("Tom & Dick & Harry") << NoError;

  // Nor are variables.
  dict.insert(QStringLiteral("amp"), QStringLiteral(" & "));
  QTest::newRow("scriptable-tags11")
      << QStringLiteral("{% load scripteddefaults %}{{ booList|join2:amp }}")
      << dict << QStringLiteral("Tom & Dick & Harry") << NoError;

  QTest::newRow("scriptable-load-error01")
      << QStringLiteral("{% load %}{{ booList|join2:amp }}") << dict
      << QString() << TagSyntaxError;

  dict.clear();
}

void TestScriptableTagsSyntax::testResolve_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  dict.insert(QStringLiteral("boo"), QStringLiteral("Far"));
  dict.insert(QStringLiteral("zing"), QStringLiteral("Bang"));

  QTest::newRow("resolve-01")
      << "{% load scripteddefaults %}{% resolver boo zing %}" << dict
      << QStringLiteral("Far - Bang") << NoError;
}

QTEST_MAIN(TestScriptableTagsSyntax)
#include "testscriptabletags.moc"

#endif
