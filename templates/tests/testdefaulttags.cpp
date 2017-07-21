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

#ifndef DEFAULTTAGSTEST_H
#define DEFAULTTAGSTEST_H

#include <QtCore/QDebug>
#include <QtTest/QTest>

#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "grantlee_paths.h"
#include "metatype.h"
#include "template.h"
#include "util.h"

typedef QList<QVariantList> Table;
typedef QHash<QString, QVariant> Dict;
typedef QPair<QString, QString> StringPair;

Q_DECLARE_METATYPE(Grantlee::Error)

class FakeTemplateLoader : public Grantlee::InMemoryTemplateLoader
{
public:
  FakeTemplateLoader() : Grantlee::InMemoryTemplateLoader()
  {
    m_existingMedia << QStringLiteral("existing_image.png")
                    << QStringLiteral("another_existing_image.png")
                    << QStringLiteral("this&that.png");
  }

  /* reimp */ QPair<QString, QString>
  getMediaUri(const QString &fileName) const override
  {
    if (m_existingMedia.contains(fileName))
      return qMakePair(QStringLiteral("/path/to/"), fileName);
    return QPair<QString, QString>();
  }

private:
  QStringList m_existingMedia;
};

class Zoo : public QObject
{
  Q_OBJECT
public:
  Zoo(QObject *parent = 0) : QObject(parent) {}

  enum Animals { Lions, Tigers, Bears };

  Q_ENUMS(Animals)
};

using namespace Grantlee;

class TestDefaultTags : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void testCommentTag_data();
  void testCommentTag() { doTest(); }

  void testFirstOfTag_data();
  void testFirstOfTag() { doTest(); }

  void testIfTag_data();
  void testIfTag() { doTest(); }

  void testForTag_data();
  void testForTag() { doTest(); }

  void testIfEqualTag_data();
  void testIfEqualTag() { doTest(); }

  void testIfNotEqualTag_data();
  void testIfNotEqualTag() { doTest(); }

  void testTemplateTagTag_data();
  void testTemplateTagTag() { doTest(); }

  void testWithTag_data();
  void testWithTag() { doTest(); }

  void testCycleTag_data();
  void testCycleTag() { doTest(); }

  void testWidthRatioTag_data();
  void testWidthRatioTag() { doTest(); }

  void testFilterTag_data();
  void testFilterTag() { doTest(); }

  void testNowTag_data();
  void testNowTag() { doTest(); }

  void testSpacelessTag_data();
  void testSpacelessTag() { doTest(); }

  void testRegroupTag_data();
  void testRegroupTag() { doTest(); }

  void testIfChangedTag_data();
  void testIfChangedTag() { doTest(); }

  void testAutoescapeTag_data();
  void testAutoescapeTag() { doTest(); }

  void testMediaFinderTag_data();
  void testMediaFinderTag() { doTest(); }

  void testRangeTag_data();
  void testRangeTag() { doTest(); }

  void testDebugTag_data();
  void testDebugTag() { doTest(); }

  void testLoadTag_data();
  void testLoadTag() { doTest(); }

  void testUrlTypes_data();
  void testUrlTypes();

  void testRelativePaths_data();
  void testRelativePaths();

private:
  void doTest();

  Engine *m_engine;
};

void TestDefaultTags::initTestCase()
{
  m_engine = new Engine(this);
  m_engine->setPluginPaths(QStringList()
                           << QStringLiteral(GRANTLEE_PLUGIN_PATH));

  auto loader1 = QSharedPointer<FakeTemplateLoader>(new FakeTemplateLoader());

  m_engine->addTemplateLoader(loader1);
}

void TestDefaultTags::cleanupTestCase() { delete m_engine; }

void TestDefaultTags::doTest()
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

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  QCOMPARE(t->error(), NoError);

  QCOMPARE(result, output);

  if (!result.isEmpty()
      && QString::fromLatin1(QTest::currentTestFunction())
             == QStringLiteral("testMediaFinderTag")) {
    QVERIFY(!context.externalMedia().isEmpty());
  }
}

void TestDefaultTags::testCommentTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("comment-tag01")
      << QStringLiteral("{% comment %}this is hidden{% endcomment %}hello")
      << dict << QStringLiteral("hello") << NoError;

  QTest::newRow("comment-tag02")
      << QStringLiteral("{% comment %}this is hidden{% endcomment %}hello{% comment %}foo{% endcomment %}")
      << dict << QStringLiteral("hello") << NoError;
  // Comment tag can contain invalid stuff.
  QTest::newRow("comment-tag03")
      << QStringLiteral("foo{% comment %} {% if %} {% endcomment %}") << dict
      << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-tag04")
      << QStringLiteral("foo{% comment %} {% endblock %} {% endcomment %}")
      << dict << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-tag05")
      << QStringLiteral("foo{% comment %} {% somerandomtag %} {% endcomment %}")
      << dict << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-tag06") << QStringLiteral("{% comment %}yes") << dict
                                 << QString() << UnclosedBlockTagError;
}

void TestDefaultTags::testFirstOfTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 0);
  dict.insert(QStringLiteral("c"), 0);
  QTest::newRow("firstof01") << QStringLiteral("{% firstof a b c %}") << dict
                             << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 0);
  dict.insert(QStringLiteral("c"), 0);
  QTest::newRow("firstof02") << QStringLiteral("{% firstof a b c %}") << dict
                             << QStringLiteral("1") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 2);
  dict.insert(QStringLiteral("c"), 0);
  QTest::newRow("firstof03") << QStringLiteral("{% firstof a b c %}") << dict
                             << QStringLiteral("2") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 0);
  dict.insert(QStringLiteral("c"), 3);
  QTest::newRow("firstof04") << QStringLiteral("{% firstof a b c %}") << dict
                             << QStringLiteral("3") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 2);
  dict.insert(QStringLiteral("c"), 3);
  QTest::newRow("firstof05") << QStringLiteral("{% firstof a b c %}") << dict
                             << QStringLiteral("1") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("b"), 0);
  dict.insert(QStringLiteral("c"), 3);
  QTest::newRow("firstof06") << QStringLiteral("{% firstof a b c %}") << dict
                             << QStringLiteral("3") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  QTest::newRow("firstof07") << "{% firstof a b \"c\" %}" << dict
                             << QStringLiteral("c") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 0);
  QTest::newRow("firstof08") << "{% firstof a b \"c and d\" %}" << dict
                             << QStringLiteral("c and d") << NoError;
  QTest::newRow("firstof09") << QStringLiteral("{% firstof %}") << dict
                             << QStringLiteral("a") << TagSyntaxError;
}

class BadIfObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isTrue READ isTrue)
  Q_PROPERTY(bool isFalse READ isFalse)
  Q_PROPERTY(bool isBad READ isBad)
public:
  BadIfObject(QObject *parent = 0) : QObject(parent), mIsBadCalled(false) {}

  bool isTrue() const { return true; }

  bool isFalse() const { return false; }

  bool isBad() const
  {
    mIsBadCalled = true;
    return true;
  }

  bool isBadCalled() { return mIsBadCalled; }
private:
  mutable bool mIsBadCalled;
};

void TestDefaultTags::testIfTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("foo"), true);

  QTest::newRow("if-tag01")
      << QStringLiteral("{% if foo %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  QTest::newRow("if-tag02")
      << QStringLiteral("{% if foo %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag03")
      << QStringLiteral("{% if foo %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag04")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% endif %}") << dict
      << QStringLiteral("foo") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag05")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% endif %}") << dict
      << QStringLiteral("bar") << NoError;

  dict.clear();
  QTest::newRow("if-tag06")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% endif %}") << dict
      << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag07")
      << QStringLiteral(
             "{% if foo %}foo{% elif bar %}bar{% else %}nothing{% endif %}")
      << dict << QStringLiteral("foo") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag08")
      << QStringLiteral(
             "{% if foo %}foo{% elif bar %}bar{% else %}nothing{% endif %}")
      << dict << QStringLiteral("bar") << NoError;

  dict.clear();
  QTest::newRow("if-tag09")
      << QStringLiteral(
             "{% if foo %}foo{% elif bar %}bar{% else %}nothing{% endif %}")
      << dict << QStringLiteral("nothing") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag10")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% elif baz %}baz{% else %}nothing{% endif %}")
      << dict << QStringLiteral("foo") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag11")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% elif baz %}baz{% else %}nothing{% endif %}")
      << dict << QStringLiteral("bar") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("baz"), true);
  QTest::newRow("if-tag12")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% elif baz %}baz{% else %}nothing{% endif %}")
      << dict << QStringLiteral("baz") << NoError;

  dict.clear();
  QTest::newRow("if-tag13")
      << QStringLiteral("{% if foo %}foo{% elif bar %}bar{% elif baz %}baz{% else %}nothing{% endif %}")
      << dict << QStringLiteral("nothing") << NoError;

  // Filters

  dict.clear();
  dict.insert(QStringLiteral("foo"), QStringLiteral("abcde"));
  QTest::newRow("if-tag-filter01")
      << QStringLiteral("{% if foo|length == 5 %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-filter02")
      << QStringLiteral(
             "{% if foo|upper == \'ABC\' %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  // Equality

  dict.clear();
  QTest::newRow("if-tag-eq01")
      << QStringLiteral("{% if foo == bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), 1);
  QTest::newRow("if-tag-eq02")
      << QStringLiteral("{% if foo == bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), 1);
  dict.insert(QStringLiteral("bar"), 1);
  QTest::newRow("if-tag-eq03")
      << QStringLiteral("{% if foo == bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), 1);
  dict.insert(QStringLiteral("bar"), 2);
  QTest::newRow("if-tag-eq04")
      << QStringLiteral("{% if foo == bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-eq05")
      << QStringLiteral("{% if foo == \'\' %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foostring"), QStringLiteral("foo"));
  QTest::newRow("if-tag-eq06")
      << QStringLiteral(
             "{% if foostring == \'foo\' %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foostring"), QStringLiteral("foo"));
  QTest::newRow("if-tag-eq06")
      << QStringLiteral(
             "{% if foostring == \'foo\' %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("zoo"), QVariant::fromValue(new Zoo(this)));
  dict.insert(QStringLiteral("tigersEnum"),
              QVariant::fromValue<int>(Zoo::Tigers));
  QTest::newRow("if-tag-eq07")
      << QStringLiteral(
             "{% if tigersEnum == zoo.Tigers %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  // Comparison

  dict.clear();
  QTest::newRow("if-tag-gt-01")
      << QStringLiteral("{% if 2 > 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-gt-02")
      << QStringLiteral("{% if 1 > 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-gte-01")
      << QStringLiteral("{% if 1 >= 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-gte-02")
      << QStringLiteral("{% if 1 >= 2 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-lt-01")
      << QStringLiteral("{% if 1 < 2 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-lt-02")
      << QStringLiteral("{% if 1 < 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-lte-01")
      << QStringLiteral("{% if 1 <= 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-lte-02")
      << QStringLiteral("{% if 2 <= 1 %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  // Contains

  dict.clear();
  dict.insert(QStringLiteral("x"), QVariantList() << 1);
  QTest::newRow("if-tag-in-01")
      << QStringLiteral("{% if 1 in x %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QVariantList() << 1);
  QTest::newRow("if-tag-in-02")
      << QStringLiteral("{% if 2 in x %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QVariantList() << 1);
  QTest::newRow("if-tag-not-in-01")
      << QStringLiteral("{% if 1 not in x %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QVariantList() << 1);
  QTest::newRow("if-tag-not-in-02")
      << QStringLiteral("{% if 2 not in x %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  // operator in with string
  dict.clear();
  dict.insert(QStringLiteral("colors"), QStringLiteral("green"));
  QTest::newRow("if-tag-operator-in-string01")
      << QStringLiteral(
             "{% if \"green\" in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("colors"), QStringLiteral("red"));
  QTest::newRow("if-tag-operator-in-string02")
      << QStringLiteral(
             "{% if \"green\" in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("colors"), QStringLiteral("green"));
  QTest::newRow("if-tag-operator-in-string03")
      << QStringLiteral(
             "{% if \"green\" in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("colors"), QStringLiteral("red"));
  QTest::newRow("if-tag-operator-in-string04")
      << QStringLiteral(
             "{% if \"green\" in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("color"), QStringLiteral("green"));
  dict.insert(QStringLiteral("colors"), QStringLiteral("green"));
  QTest::newRow("if-tag-operator-in-string05")
      << QStringLiteral("{% if color in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("color"), QStringLiteral("green"));
  dict.insert(QStringLiteral("colors"), QStringLiteral("red"));
  QTest::newRow("if-tag-operator-in-string06")
      << QStringLiteral("{% if color in colors %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  // AND

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-and01")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-and02")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-and03")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-and04")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  QTest::newRow("if-tag-and05")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-and06")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag-and07")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-and08")
      << QStringLiteral("{% if foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  // OR

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-or01")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-or02")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-or03")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-or04")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  QTest::newRow("if-tag-or05")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-or06")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag-or07")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-or08")
      << QStringLiteral("{% if foo or bar %}yes{% else %}no{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("baz"), true);
  QTest::newRow("if-tag-or09")
      << QStringLiteral("{% if foo or bar or baz %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  // NOT

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag-not01")
      << QStringLiteral("{% if not foo %}no{% else %}yes{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag-not02")
      << QStringLiteral("{% if not not foo %}no{% else %}yes{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not06")
      << QStringLiteral("{% if foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not07")
      << QStringLiteral("{% if foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not08")
      << QStringLiteral("{% if foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not09")
      << QStringLiteral("{% if foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not10")
      << QStringLiteral("{% if foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not11")
      << QStringLiteral("{% if not foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not12")
      << QStringLiteral("{% if not foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not13")
      << QStringLiteral("{% if not foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not14")
      << QStringLiteral("{% if not foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not15")
      << QStringLiteral("{% if not foo and bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not16")
      << QStringLiteral("{% if foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not17")
      << QStringLiteral("{% if foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not18")
      << QStringLiteral("{% if foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not19")
      << QStringLiteral("{% if foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not20")
      << QStringLiteral("{% if foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not21")
      << QStringLiteral("{% if not foo or bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not22")
      << QStringLiteral("{% if not foo or bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not23")
      << QStringLiteral("{% if not foo or bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not24")
      << QStringLiteral("{% if not foo or bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not25")
      << QStringLiteral("{% if not foo or bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not26")
      << QStringLiteral(
             "{% if not foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not27")
      << QStringLiteral(
             "{% if not foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not28")
      << QStringLiteral(
             "{% if not foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not29")
      << QStringLiteral(
             "{% if not foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not30")
      << QStringLiteral(
             "{% if not foo and not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-not31")
      << QStringLiteral("{% if not foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not32")
      << QStringLiteral("{% if not foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not33")
      << QStringLiteral("{% if not foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), true);
  QTest::newRow("if-tag-not34")
      << QStringLiteral("{% if not foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), false);
  dict.insert(QStringLiteral("bar"), false);
  QTest::newRow("if-tag-not35")
      << QStringLiteral("{% if not foo or not bar %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("yes") << NoError;

  QTest::newRow("if-tag-error01") << QStringLiteral("{% if %}yes{% endif %}")
                                  << dict << QString() << TagSyntaxError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), true);
  QTest::newRow("if-tag-error02")
      << QStringLiteral("{% if foo and %}yes{% else %}no{% endif %}") << dict
      << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error03")
      << QStringLiteral("{% if foo or %}yes{% else %}no{% endif %}") << dict
      << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error04")
      << QStringLiteral("{% if not foo and %}yes{% else %}no{% endif %}")
      << dict << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error05")
      << QStringLiteral("{% if not foo or %}yes{% else %}no{% endif %}") << dict
      << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error06")
      << QStringLiteral("{% if abc def %}yes{% endif %}") << dict << QString()
      << TagSyntaxError;
  QTest::newRow("if-tag-error07")
      << QStringLiteral("{% if not %}yes{% endif %}") << dict << QString()
      << TagSyntaxError;
  QTest::newRow("if-tag-error08")
      << QStringLiteral("{% if and %}yes{% endif %}") << dict << QString()
      << TagSyntaxError;
  QTest::newRow("if-tag-error09") << QStringLiteral("{% if or %}yes{% endif %}")
                                  << dict << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error10") << QStringLiteral("{% if == %}yes{% endif %}")
                                  << dict << QString() << TagSyntaxError;
  QTest::newRow("if-tag-error11")
      << QStringLiteral("{% if 1 == %}yes{% endif %}") << dict << QString()
      << TagSyntaxError;
  QTest::newRow("if-tag-error12")
      << QStringLiteral("{% if a not b %}yes{% endif %}") << dict << QString()
      << TagSyntaxError;

  // Short circuit
  dict.clear();
  {
    auto bio = QSharedPointer<BadIfObject>::create();
    dict.insert(QStringLiteral("x"), QVariant::fromValue(bio));
    QTest::newRow("if-tag-shortcircuit01")
        << QStringLiteral(
               "{% if x.isTrue or x.isBad %}yes{% else %}no{% endif %}")
        << dict << QStringLiteral("yes") << NoError;

    QTest::newRow("if-tag-shortcircuit02")
        << QStringLiteral(
               "{% if x.isFalse and x.isBad %}yes{% else %}no{% endif %}")
        << dict << QStringLiteral("no") << NoError;
    dict.clear();
  }

  QTest::newRow("if-tag-badarg01")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("y"), 0);
  QTest::newRow("if-tag-badarg02")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("y"), 1);
  QTest::newRow("if-tag-badarg03")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-badarg04")
      << QStringLiteral(
             "{% if x|default_if_none:y %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("foo"), 1);
  QTest::newRow("if-tag-single-eq")
      << QStringLiteral("{% if foo = bar %}yes{% else %}no{% endif %}") << dict
      << QString() << TagSyntaxError;

  // Truthiness
  dict.clear();
  QVariantHash hash;
  dict.insert(QStringLiteral("var"), hash);
  QTest::newRow("if-truthiness01")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  hash.insert(QStringLiteral("foo"), QStringLiteral("bar"));
  dict.insert(QStringLiteral("var"), hash);
  QTest::newRow("if-truthiness02")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;
  QVariantList list;
  dict.insert(QStringLiteral("var"), list);
  QTest::newRow("if-truthiness03")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  list.append(QStringLiteral("foo"));
  dict.insert(QStringLiteral("var"), list);
  QTest::newRow("if-truthiness04")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;

  QVariant var;
  dict.insert(QStringLiteral("var"), var);
  QTest::newRow("if-truthiness05")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  var = QStringLiteral("foo");
  dict.insert(QStringLiteral("var"), var);
  QTest::newRow("if-truthiness06")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;

  QString str;
  dict.insert(QStringLiteral("var"), str);
  QTest::newRow("if-truthiness07")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  str = QStringLiteral("foo");
  dict.insert(QStringLiteral("var"), str);
  QTest::newRow("if-truthiness08")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;

  auto i = 0;
  dict.insert(QStringLiteral("var"), i);
  QTest::newRow("if-truthiness07")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  i = 7;
  dict.insert(QStringLiteral("var"), i);
  QTest::newRow("if-truthiness08")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;

  auto r = 0.0;
  dict.insert(QStringLiteral("var"), r);
  QTest::newRow("if-truthiness09")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("No") << NoError;
  r = 7.1;
  dict.insert(QStringLiteral("var"), r);
  QTest::newRow("if-truthiness10")
      << QStringLiteral("{% if var %}Yes{% else %}No{% endif %}") << dict
      << QStringLiteral("Yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-badarg01")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QString() << NoError;

  dict.insert(QStringLiteral("y"), 0);

  QTest::newRow("if-tag-badarg02")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("y"), 1);

  QTest::newRow("if-tag-badarg03")
      << QStringLiteral("{% if x|default_if_none:y %}yes{% endif %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  QTest::newRow("if-tag-badarg04")
      << QStringLiteral(
             "{% if x|default_if_none:y %}yes{% else %}no{% endif %}")
      << dict << QStringLiteral("no") << NoError;
}

void TestDefaultTags::testForTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantList list;
  list << 1 << 2 << 3;
  dict.insert(QStringLiteral("values"), list);
  QTest::newRow("for-tag01")
      << QStringLiteral("{% for val in values %}{{ val }}{% endfor %}") << dict
      << QStringLiteral("123") << NoError;
  QTest::newRow("for-tag02")
      << QStringLiteral("{% for val in values reversed %}{{ val }}{% endfor %}")
      << dict << QStringLiteral("321") << NoError;
  list.clear();
  dict.insert(QStringLiteral("values"), list);
  QTest::newRow("for-tag03")
      << QStringLiteral(
             "{% for val in values %}({{ val }} sdfsdf,){% endfor %}")
      << dict << QString() << NoError;
  QStringList emails;
  emails << QStringLiteral("one") << QStringLiteral("two");
  QVariantHash obj;
  obj.insert(QStringLiteral("emails"), emails);
  dict.insert(QStringLiteral("contact"), obj);
  QTest::newRow("for-tag04")
      << QStringLiteral(
             "{% for val in contact.emails %}({{ val }},){% endfor %}")
      << dict << QStringLiteral("(one,)(two,)") << NoError;
  emails.clear();
  obj.insert(QStringLiteral("emails"), emails);
  dict.insert(QStringLiteral("contact"), obj);
  QTest::newRow("for-tag05")
      << QStringLiteral(
             "{% for val in contact.emails %}({{ val }},){% endfor %}")
      << dict << QString() << NoError;
  list.clear();
  dict.clear();
  emails << QStringLiteral("one");
  dict.insert(QStringLiteral("emails"), emails);
  QTest::newRow("for-tag06")
      << QStringLiteral("{% for val in emails %}({{ val }},){% endfor %}")
      << dict << QStringLiteral("(one,)") << NoError;
  list.clear();
  dict.clear();

  QTest::newRow("for-tag07") << QStringLiteral("{% for %}{% endfor %}") << dict
                             << QString() << TagSyntaxError;
  QTest::newRow("for-tag08")
      << QStringLiteral("{% for foo bar bat %}{% endfor %}") << dict
      << QString() << TagSyntaxError;
  QTest::newRow("for-tag09")
      << QStringLiteral("{% for foo bar '' %}{% endfor %}") << dict << QString()
      << TagSyntaxError;

  list << 1 << 2 << 3;
  dict.insert(QStringLiteral("values"), list);
  QTest::newRow("for-tag-vars01")
      << QStringLiteral(
             "{% for val in values %}{{ forloop.counter }}{% endfor %}")
      << dict << QStringLiteral("123") << NoError;
  QTest::newRow("for-tag-vars02")
      << QStringLiteral(
             "{% for val in values %}{{ forloop.counter0 }}{% endfor %}")
      << dict << QStringLiteral("012") << NoError;
  QTest::newRow("for-tag-vars03")
      << QStringLiteral(
             "{% for val in values %}{{ forloop.revcounter }}{% endfor %}")
      << dict << QStringLiteral("321") << NoError;
  QTest::newRow("for-tag-vars04")
      << QStringLiteral(
             "{% for val in values %}{{ forloop.revcounter0 }}{% endfor %}")
      << dict << QStringLiteral("210") << NoError;
  QTest::newRow("for-tag-vars05")
      << QStringLiteral("{% for val in values %}{% if forloop.first %}f{% else %}x{% endif %}{% endfor %}")
      << dict << QStringLiteral("fxx") << NoError;
  QTest::newRow("for-tag-vars06")
      << QStringLiteral("{% for val in values %}{% if forloop.last %}l{% else %}x{% endif %}{% endfor %}")
      << dict << QStringLiteral("xxl") << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << QStringLiteral("one") << 1;
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("two") << 2;
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("items"), list);
  QTest::newRow("for-tag-unpack01")
      << QStringLiteral(
             "{% for key,value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;

  QTest::newRow("for-tag-unpack03")
      << QStringLiteral(
             "{% for key, value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;
  QTest::newRow("for-tag-unpack04")
      << QStringLiteral(
             "{% for key , value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;
  QTest::newRow("for-tag-unpack05")
      << QStringLiteral(
             "{% for key ,value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;
  QTest::newRow("for-tag-unpack06")
      << QStringLiteral(
             "{% for key value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;
  QTest::newRow("for-tag-unpack07")
      << QStringLiteral(
             "{% for key,,value in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;
  QTest::newRow("for-tag-unpack08")
      << QStringLiteral(
             "{% for key,value, in items %}{{ key }}:{{ value }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;

  // Ensure that a single loopvar doesn't truncate the list in val.
  QTest::newRow("for-tag-unpack09")
      << QStringLiteral(
             "{% for val in items %}{{ val.0 }}:{{ val.1 }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;

  // Otherwise, silently truncate if the length of loopvars differs to the
  // length of each set of items.

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QStringLiteral("one") << 1 << QStringLiteral("carrot");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("two") << 2 << QStringLiteral("orange");
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("items"), list);

  QTest::newRow("for-tag-unpack10")
      << QStringLiteral("{% for x,y in items %}{{ x }}:{{ y }}/{% endfor %}")
      << dict << QStringLiteral("one:1/two:2/") << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QStringLiteral("one") << 1;
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("two") << 2;
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("items"), list);

  QTest::newRow("for-tag-unpack11")
      << QStringLiteral(
             "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}")
      << dict << QStringLiteral("one:1,/two:2,/") << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QStringLiteral("one") << 1 << QStringLiteral("carrot");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("two") << 2;
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("items"), list);

  QTest::newRow("for-tag-unpack12")
      << QStringLiteral(
             "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}")
      << dict << QStringLiteral("one:1,carrot/two:2,/") << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QStringLiteral("one") << 1 << QStringLiteral("carrot");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("two") << 2 << QStringLiteral("cheese");
  list.append(QVariant(innerList));

  dict.insert(QStringLiteral("items"), list);

  QTest::newRow("for-tag-unpack13")
      << QStringLiteral(
             "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}")
      << dict << QStringLiteral("one:1,carrot/two:2,cheese/") << NoError;

  // Empty tag:

  dict.clear();
  dict.insert(QStringLiteral("values"), QVariantList() << 1 << 2 << 3);
  QTest::newRow("for-tag-empty01")
      << QStringLiteral("{% for val in values %}{{ val }}{% empty %}empty text{% endfor %}")
      << dict << QStringLiteral("123") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("values"), QVariantList());
  QTest::newRow("for-tag-empty02")
      << QStringLiteral("{% for val in values %}{{ val }}{% empty %}values array empty{% endfor %}")
      << dict << QStringLiteral("values array empty") << NoError;

  dict.clear();
  QTest::newRow("for-tag-empty03")
      << QStringLiteral("{% for val in values %}{{ val }}{% empty %}values array not found{% endfor %}")
      << dict << QStringLiteral("values array not found") << NoError;
}

void TestDefaultTags::testIfEqualTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 2);

  QTest::newRow("ifequal01")
      << QStringLiteral("{% ifequal a b %}yes{% endifequal %}") << dict
      << QString() << NoError;
  QTest::newRow("ifequal03")
      << QStringLiteral("{% ifequal a b %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 1);

  QTest::newRow("ifequal02")
      << QStringLiteral("{% ifequal a b %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;
  QTest::newRow("ifequal04")
      << QStringLiteral("{% ifequal a b %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("test"));

  QTest::newRow("ifequal05")
      << QStringLiteral("{% ifequal a 'test' %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("no"));

  QTest::newRow("ifequal06")
      << QStringLiteral("{% ifequal a 'test' %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("test"));

  QTest::newRow("ifequal07")
      << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("no"));

  QTest::newRow("ifequal08")
      << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("no") << NoError;

  dict.clear();

  QTest::newRow("ifequal09")
      << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("no") << NoError;

  QTest::newRow("ifequal10")
      << QStringLiteral("{% ifequal a b %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("yes") << NoError;

  QTest::newRow("ifequal-split01")
      << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("no") << NoError;

  dict.insert(QStringLiteral("a"), QStringLiteral("foo"));
  QTest::newRow("ifequal-split02")
      << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("test man"));
  QTest::newRow("ifequal-split03")
      << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;
  QTest::newRow("ifequal-split04")
      << QStringLiteral(
             "{% ifequal a 'test man' %}yes{% else %}no{% endifequal %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral(""));
  QTest::newRow("ifequal-split05")
      << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}"
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("i \"love\" you"));
  QTest::newRow("ifequal-split06")
      << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}"
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("i love you"));
  QTest::newRow("ifequal-split07")
      << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}"
      << dict << QStringLiteral("no") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("I'm happy"));
  QTest::newRow("ifequal-split08")
      << "{% ifequal a 'I\\'m happy' %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("slash\\man"));
  QTest::newRow("ifequal-split09")
      << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("slashman"));
  QTest::newRow("ifequal-split10")
      << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict
      << QStringLiteral("no") << NoError;
  // NUMERIC RESOLUTION

  dict.clear();
  dict.insert(QStringLiteral("x"), QStringLiteral("5"));

  QTest::newRow("ifequal-numeric01")
      << QStringLiteral("{% ifequal x 5 %}yes{% endifequal %}") << dict
      << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), 5);
  QTest::newRow("ifequal-numeric02")
      << QStringLiteral("{% ifequal x 5 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), 5.2);
  QTest::newRow("ifequal-numeric03")
      << QStringLiteral("{% ifequal x 5 %}yes{% endifequal %}") << dict
      << QString() << NoError;
  QTest::newRow("ifequal-numeric04")
      << QStringLiteral("{% ifequal x 5.2 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), .2);

  QTest::newRow("ifequal-numeric05")
      << QStringLiteral("{% ifequal x 0.2 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;
  QTest::newRow("ifequal-numeric06")
      << QStringLiteral("{% ifequal x .2 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), 2);

  QTest::newRow("ifequal-numeric07")
      << QStringLiteral("{% ifequal x 2. %}yes{% endifequal %}") << dict
      << QString() << TagSyntaxError;

  dict.clear();
  dict.insert(QStringLiteral("x"), 5);
  QTest::newRow("ifequal-numeric08")
      << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << QString()
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QStringLiteral("5"));
  QTest::newRow("ifequal-numeric09")
      << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), -5);
  QTest::newRow("ifequal-numeric10")
      << QStringLiteral("{% ifequal x -5 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), -5.2);
  QTest::newRow("ifequal-numeric11")
      << QStringLiteral("{% ifequal x -5.2 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), 5);
  QTest::newRow("ifequal-numeric12")
      << QStringLiteral("{% ifequal x +5 %}yes{% endifequal %}") << dict
      << QStringLiteral("yes") << NoError;

  // FILTER EXPRESSIONS AS ARGUMENTS

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a"));
  QTest::newRow("ifequal-filter01")
      << "{% ifequal a|upper \"A\" %}x{% endifequal %}" << dict
      << QStringLiteral("x") << NoError;

  QTest::newRow("ifequal-filter02")
      << "{% ifequal \"A\" a|upper %}x{% endifequal %}" << dict
      << QStringLiteral("x") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("x"));
  dict.insert(QStringLiteral("b"), QStringLiteral("X"));

  QTest::newRow("ifequal-filter03")
      << QStringLiteral("{% ifequal a|upper b|upper %}x{% endifequal %}")
      << dict << QStringLiteral("x") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QStringLiteral("aaa"));

  QTest::newRow("ifequal-filter04")
      << "{% ifequal x|slice:\"1\" \"a\" %}x{% endifequal %}" << dict
      << QStringLiteral("x") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("x"), QStringLiteral("aaa"));

  QTest::newRow("ifequal-filter05")
      << "{% ifequal x|slice:\"1\"|upper \"A\" %}x{% endifequal %}" << dict
      << QStringLiteral("x") << NoError;

  QTest::newRow("ifequal-error01")
      << "{% ifequal x|slice:\"1\"|upper %}x{% endifequal %}" << dict
      << QString() << TagSyntaxError;
}

void TestDefaultTags::testIfNotEqualTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 2);

  QTest::newRow("ifnotequal01")
      << QStringLiteral("{% ifnotequal a b %}yes{% endifnotequal %}") << dict
      << QStringLiteral("yes") << NoError;
  QTest::newRow("ifnotequal03")
      << QStringLiteral(
             "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}")
      << dict << QStringLiteral("yes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 1);
  dict.insert(QStringLiteral("b"), 1);

  QTest::newRow("ifnotequal02")
      << QStringLiteral("{% ifnotequal a b %}yes{% endifnotequal %}") << dict
      << QString() << NoError;
  QTest::newRow("ifnotequal04")
      << QStringLiteral(
             "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}")
      << dict << QStringLiteral("no") << NoError;
}

void TestDefaultTags::testTemplateTagTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("templatetag01")
      << QStringLiteral("{% templatetag openblock %}") << dict
      << QStringLiteral("{%") << NoError;
  QTest::newRow("templatetag02")
      << QStringLiteral("{% templatetag closeblock %}") << dict
      << QStringLiteral("%}") << NoError;
  QTest::newRow("templatetag03")
      << QStringLiteral("{% templatetag openvariable %}") << dict
      << QStringLiteral("{{") << NoError;
  QTest::newRow("templatetag04")
      << QStringLiteral("{% templatetag closevariable %}") << dict
      << QStringLiteral("}}") << NoError;
  QTest::newRow("templatetag05") << QStringLiteral("{% templatetag %}") << dict
                                 << QString() << TagSyntaxError;
  QTest::newRow("templatetag06") << QStringLiteral("{% templatetag foo %}")
                                 << dict << QString() << TagSyntaxError;
  QTest::newRow("templatetag07")
      << QStringLiteral("{% templatetag openbrace %}") << dict
      << QStringLiteral("{") << NoError;
  QTest::newRow("templatetag08")
      << QStringLiteral("{% templatetag closebrace %}") << dict
      << QStringLiteral("}") << NoError;
  QTest::newRow("templatetag09")
      << QStringLiteral(
             "{% templatetag openbrace %}{% templatetag openbrace %}")
      << dict << QStringLiteral("{{") << NoError;
  QTest::newRow("templatetag10")
      << QStringLiteral(
             "{% templatetag closebrace %}{% templatetag closebrace %}")
      << dict << QStringLiteral("}}") << NoError;
  QTest::newRow("templatetag11")
      << QStringLiteral("{% templatetag opencomment %}") << dict
      << QStringLiteral("{#") << NoError;
  QTest::newRow("templatetag12")
      << QStringLiteral("{% templatetag closecomment %}") << dict
      << QStringLiteral("#}") << NoError;
}

void TestDefaultTags::testWithTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantHash hash;
  hash.insert(QStringLiteral("key"), 50);
  dict.insert(QStringLiteral("dict"), hash);
  QTest::newRow("with01")
      << QStringLiteral("{% with dict.key as key %}{{ key }}{% endwith %}")
      << dict << QStringLiteral("50") << NoError;
  QTest::newRow("with02") << QStringLiteral("{{ key }}{% with dict.key as key %}{{ key }}-{{ dict.key }}-{{ key }}{% endwith %}{{ key }}")
                          << dict << QStringLiteral("50-50-50") << NoError;
  QTest::newRow("with-error01")
      << QStringLiteral("{% with dict.key xx key %}{{ key }}{% endwith %}")
      << dict << QString() << TagSyntaxError;
  QTest::newRow("with-error02")
      << QStringLiteral("{% with dict.key as %}{{ key }}{% endwith %}") << dict
      << QString() << TagSyntaxError;
}

void TestDefaultTags::testCycleTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("cycle01") << QStringLiteral("{% cycle a %}") << dict
                           << QString() << TagSyntaxError;
  QTest::newRow("cycle02") << QStringLiteral(
                                  "{% cycle a,b,c as abc %}{% cycle abc %}")
                           << dict << QStringLiteral("ab") << NoError;
  QTest::newRow("cycle03")
      << QStringLiteral(
             "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}")
      << dict << QStringLiteral("abc") << NoError;
  QTest::newRow("cycle04") << QStringLiteral("{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}")
                           << dict << QStringLiteral("abca") << NoError;
  QTest::newRow("cycle05") << QStringLiteral("{% cycle a %}") << dict
                           << QString() << TagSyntaxError;
  // TODO: This is the same as cycle01. Remove.
  QTest::newRow("cycle06") << QStringLiteral("{% cycle a %}") << dict
                           << QString() << TagSyntaxError;
  QTest::newRow("cycle07") << QStringLiteral(
                                  "{% cycle a,b,c as foo %}{% cycle bar %}")
                           << dict << QString() << TagSyntaxError;
  QTest::newRow("cycle08") << QStringLiteral("{% cycle a,b,c as foo %}{% cycle foo %}{{ foo }}{{ foo }}{% cycle foo %}{{ foo }}")
                           << dict << QStringLiteral("abbbcc") << NoError;

  dict.insert(QStringLiteral("test"), QVariantList() << 0 << 1 << 2 << 3 << 4);
  QTest::newRow("cycle09")
      << QStringLiteral(
             "{% for i in test %}{% cycle a,b %}{{ i }},{% endfor %}")
      << dict << QStringLiteral("a0,b1,a2,b3,a4,") << NoError;

  dict.clear();
  QTest::newRow("cycle10")
      << QStringLiteral("{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}") << dict
      << QStringLiteral("ab") << NoError;
  QTest::newRow("cycle11")
      << QStringLiteral(
             "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}")
      << dict << QStringLiteral("abc") << NoError;
  QTest::newRow("cycle12") << QStringLiteral("{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}")
                           << dict << QStringLiteral("abca") << NoError;

  dict.insert(QStringLiteral("test"), QVariantList() << 0 << 1 << 2 << 3 << 4);
  QTest::newRow("cycle13")
      << QStringLiteral(
             "{% for i in test %}{% cycle 'a' 'b' %}{{ i }},{% endfor %}")
      << dict << QStringLiteral("a0,b1,a2,b3,a4,") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("one"), QStringLiteral("1"));
  dict.insert(QStringLiteral("two"), QStringLiteral("2"));
  QTest::newRow("cycle14") << QStringLiteral(
                                  "{% cycle one two as foo %}{% cycle foo %}")
                           << dict << QStringLiteral("12") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("test"), QVariantList() << 0 << 1 << 2 << 3 << 4);
  dict.insert(QStringLiteral("aye"), QStringLiteral("a"));
  dict.insert(QStringLiteral("bee"), QStringLiteral("b"));
  QTest::newRow("cycle15")
      << QStringLiteral(
             "{% for i in test %}{% cycle aye bee %}{{ i }},{% endfor %}")
      << dict << QStringLiteral("a0,b1,a2,b3,a4,") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("one"), QStringLiteral("A"));
  dict.insert(QStringLiteral("two"), QStringLiteral("2"));
  QTest::newRow("cycle16")
      << QStringLiteral("{% cycle one|lower two as foo %}{% cycle foo %}")
      << dict << QStringLiteral("a2") << NoError;

  QTest::newRow("cycle17") << QStringLiteral("{% cycle %}") << dict << QString()
                           << TagSyntaxError;
  QTest::newRow("cycle18") << QStringLiteral("{% cycle var %}") << dict
                           << QString() << TagSyntaxError;

  dict.insert(QStringLiteral("three"), QStringLiteral("B"));
  dict.insert(QStringLiteral("four"), QStringLiteral("4"));

  QTest::newRow("cycle19") << QStringLiteral("{% cycle one two three foo %}")
                           << dict << QStringLiteral("A") << NoError;
  QTest::newRow("cycle20")
      << QStringLiteral(
             "{% cycle one two as foo %}{% cycle three four as bar %}")
      << dict << QStringLiteral("AB") << NoError;
}

void TestDefaultTags::testWidthRatioTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 100);
  QTest::newRow("widthratio01") << QStringLiteral("{% widthratio a b 0 %}")
                                << dict << QStringLiteral("0") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 0);
  QTest::newRow("widthratio02") << QStringLiteral("{% widthratio a b 0 %}")
                                << dict << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 0);
  dict.insert(QStringLiteral("b"), 100);
  QTest::newRow("widthratio03") << QStringLiteral("{% widthratio a b 100 %}")
                                << dict << QStringLiteral("0") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 100);
  QTest::newRow("widthratio04") << QStringLiteral("{% widthratio a b 100 %}")
                                << dict << QStringLiteral("50") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 100);
  dict.insert(QStringLiteral("b"), 100);
  QTest::newRow("widthratio05") << QStringLiteral("{% widthratio a b 100 %}")
                                << dict << QStringLiteral("100") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 80);
  QTest::newRow("widthratio06") << QStringLiteral("{% widthratio a b 100 %}")
                                << dict << QStringLiteral("63") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 70);
  QTest::newRow("widthratio07") << QStringLiteral("{% widthratio a b 100 %}")
                                << dict << QStringLiteral("71") << NoError;

  dict.clear();
  // Raise exception if we don't have 3 args, last one an integer
  QTest::newRow("widthratio08") << QStringLiteral("{% widthratio %}") << dict
                                << QString() << TagSyntaxError;

  dict.clear();
  QTest::newRow("widthratio09") << QStringLiteral("{% widthratio a b %}")
                                << dict << QString() << TagSyntaxError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 100);
  QTest::newRow("widthratio10") << QStringLiteral("{% widthratio a b 100.0 %}")
                                << dict << QStringLiteral("50") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), 50);
  dict.insert(QStringLiteral("b"), 100);
  dict.insert(QStringLiteral("c"), 100);
  QTest::newRow("widthratio11") << QStringLiteral("{% widthratio a b c %}")
                                << dict << QStringLiteral("50") << NoError;
}

void TestDefaultTags::testFilterTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("filter01")
      << QStringLiteral("{% filter upper %}{% endfilter %}") << dict
      << QString() << NoError;
  QTest::newRow("filter02")
      << QStringLiteral("{% filter upper %}django{% endfilter %}") << dict
      << QStringLiteral("DJANGO") << NoError;
  QTest::newRow("filter03")
      << QStringLiteral("{% filter upper|lower %}django{% endfilter %}") << dict
      << QStringLiteral("django") << NoError;

  dict.insert(QStringLiteral("remove"), QStringLiteral("spam"));
  QTest::newRow("filter04")
      << QStringLiteral("{% filter cut:remove %}djangospam{% endfilter %}")
      << dict << QStringLiteral("django") << NoError;
}

void TestDefaultTags::testNowTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  auto today = QDateTime::currentDateTime().date();

  QTest::newRow("now01") << QStringLiteral("{% now \"d M yyyy\"%}") << dict
                         << (QString::number(today.day()) + QLatin1Char(' ')
                             + QString::number(today.month()) + QLatin1Char(' ')
                             + QString::number(today.year()))
                         << NoError;

  QTest::newRow("now02") << QStringLiteral("{% now \"d \"M\" yyyy\"%}") << dict
                         << QString() << TagSyntaxError;
}

void TestDefaultTags::testSpacelessTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("spaceless01")
      << QStringLiteral(
             "{% spaceless %} <b>    <i> text </i>    </b> {% endspaceless %}")
      << dict << QStringLiteral("<b><i> text </i></b>") << NoError;
  QTest::newRow("spaceless02")
      << "{% spaceless %} <b> \n <i> text </i> \n </b> {% endspaceless %}"
      << dict << QStringLiteral("<b><i> text </i></b>") << NoError;
  QTest::newRow("spaceless03")
      << QStringLiteral("{% spaceless %}<b><i>text</i></b>{% endspaceless %}")
      << dict << QStringLiteral("<b><i>text</i></b>") << NoError;

  dict.insert(QStringLiteral("text"), QStringLiteral("This & that"));
  QTest::newRow("spaceless04")
      << QStringLiteral(
             "{% spaceless %}<b>  <i>{{ text }}</i>  </b>{% endspaceless %}")
      << dict << QStringLiteral("<b><i>This &amp; that</i></b>") << NoError;
  QTest::newRow("spaceless05")
      << QStringLiteral("{% autoescape off %}{% spaceless %}<b>  <i>{{ text }}</i>  </b>{% endspaceless %}{% endautoescape %}")
      << dict << QStringLiteral("<b><i>This & that</i></b>") << NoError;
  QTest::newRow("spaceless06")
      << QStringLiteral("{% spaceless %}<b>  <i>{{ text|safe }}</i>  </b>{% endspaceless %}")
      << dict << QStringLiteral("<b><i>This & that</i></b>") << NoError;
}

void TestDefaultTags::testRegroupTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantList list;
  QVariantHash hash;

  hash.insert(QStringLiteral("foo"), QStringLiteral("c"));
  hash.insert(QStringLiteral("bar"), 1);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("d"));
  hash.insert(QStringLiteral("bar"), 1);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("a"));
  hash.insert(QStringLiteral("bar"), 2);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("b"));
  hash.insert(QStringLiteral("bar"), 2);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("x"));
  hash.insert(QStringLiteral("bar"), 3);
  list.append(hash);

  dict.insert(QStringLiteral("data"), list);

  QTest::newRow("regroup01")
      << QString::fromLatin1("{% regroup data by bar as grouped %}"
                             "{% for group in grouped %}"
                             "{{ group.grouper }}:"
                             "{% for item in group.list %}"
                             "{{ item.foo }}"
                             "{% endfor %},"
                             "{% endfor %}")
      << dict << QStringLiteral("1:cd,2:ab,3:x,") << NoError;

  dict.clear();
  hash.clear();
  list.clear();

  hash.insert(QStringLiteral("foo"), QStringLiteral("a"));
  hash.insert(QStringLiteral("bar"), 2);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("b"));
  hash.insert(QStringLiteral("bar"), 2);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("x"));
  hash.insert(QStringLiteral("bar"), 3);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("c"));
  hash.insert(QStringLiteral("bar"), 1);
  list.append(hash);

  hash.clear();
  hash.insert(QStringLiteral("foo"), QStringLiteral("d"));
  hash.insert(QStringLiteral("bar"), 1);
  list.append(hash);

  dict.insert(QStringLiteral("data"), list);

  // Data is output in the order it is sent in.

  QTest::newRow("regroup02")
      << QString::fromLatin1("{% regroup data by bar as grouped %}"
                             "{% for group in grouped %}"
                             "{{ group.grouper }}:"
                             "{% for item in group.list %}"
                             "{{ item.foo }}"
                             "{% endfor %},"
                             "{% endfor %}")
      << dict << QStringLiteral("2:ab,3:x,1:cd,") << NoError;

  dict.clear();
  hash.clear();
  list.clear();

  Table table;

  QVariantList row;
  row.append(1);
  row.append(QStringLiteral("a"));
  table.append(row);

  row.clear();
  row.append(1);
  row.append(QStringLiteral("b"));
  table.append(row);

  row.clear();
  row.append(2);
  row.append(QStringLiteral("a"));
  table.append(row);

  row.clear();
  row.append(3);
  row.append(QStringLiteral("c"));
  table.append(row);

  row.clear();
  row.append(3);
  row.append(QStringLiteral("d"));
  table.append(row);

  dict.insert(QStringLiteral("data"), QVariant::fromValue(table));

  QTest::newRow("regroup03")
      << QString::fromLatin1("{% regroup data by 0 as grouped %}"
                             "{% for group in grouped %}"
                             "{{ group.grouper }}:"
                             "{% for item in group.list %}"
                             "{{ item.1 }}"
                             "{% endfor %},"
                             "{% endfor %}")
      << dict << QStringLiteral("1:ab,2:a,3:cd,") << NoError;
}

void TestDefaultTags::testIfChangedTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 2 << 3);
  QTest::newRow("ifchanged01")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}")
      << dict << QStringLiteral("123") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 3);
  QTest::newRow("ifchanged02")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}")
      << dict << QStringLiteral("13") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 1);
  QTest::newRow("ifchanged03")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}")
      << dict << QStringLiteral("1") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 2 << 3);
  dict.insert(QStringLiteral("numx"), QVariantList() << 2 << 2 << 2);
  QTest::newRow("ifchanged04")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("122232") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 1);
  dict.insert(QStringLiteral("numx"), QVariantList() << 1 << 2 << 3);
  QTest::newRow("ifchanged05")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("1123123123") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 1);
  dict.insert(QStringLiteral("numx"), QVariantList() << 2 << 2 << 2);
  QTest::newRow("ifchanged06")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("1222") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 1);
  dict.insert(QStringLiteral("numx"), QVariantList() << 2 << 2 << 2);
  dict.insert(QStringLiteral("numy"), QVariantList() << 3 << 3 << 3);
  QTest::newRow("ifchanged07")
      << QStringLiteral("{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% for y in numy %}{% ifchanged %}{{ y }}{% endifchanged %}{% endfor %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("1233323332333") << NoError;
  QTest::newRow("ifchanged08")
      << QStringLiteral("{% ifchanged %}{{ num.0 }}{% endifchanged %}") << dict
      << QStringLiteral("1") << NoError;

  // datalist': [[(1, 'a'), (1, 'a'), (0, 'b'), (1, 'c')], [(0, 'a'), (1,
  // 'c'),
  // (1, 'd'), (1, 'd'), (0, 'e')]]}
  dict.clear();
  QVariantList list;
  QVariantList innerList;
  QVariantList tuple;
  tuple << 1 << QStringLiteral("a");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 1 << QStringLiteral("a");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 0 << QStringLiteral("b");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 1 << QStringLiteral("c");
  innerList.append(QVariant(tuple));
  tuple.clear();
  list.append(QVariant(innerList));
  innerList.clear();

  tuple << 0 << QStringLiteral("a");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 1 << QStringLiteral("c");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 1 << QStringLiteral("d");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 1 << QStringLiteral("d");
  innerList.append(QVariant(tuple));
  tuple.clear();
  tuple << 0 << QStringLiteral("e");
  innerList.append(QVariant(tuple));
  tuple.clear();
  list.append(QVariant(innerList));
  innerList.clear();

  dict.insert(QStringLiteral("datalist"), list);
  QTest::newRow("ifchanged08")
      << QStringLiteral("{% for data in datalist %}{% for c,d in data %}{% if c %}{% ifchanged %}{{ d }}{% endifchanged %}{% endif %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("accd") << NoError;

  // Test one parameter given to ifchanged.
  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 2 << 3);
  QTest::newRow("ifchanged-param01")
      << QStringLiteral("{% for n in num %}{% ifchanged n %}..{% endifchanged %}{{ n }}{% endfor %}")
      << dict << QStringLiteral("..1..2..3") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 2 << 3);
  dict.insert(QStringLiteral("numx"), QVariantList() << 5 << 6 << 7);
  QTest::newRow("ifchanged-param02")
      << QStringLiteral("{% for n in num %}{% for x in numx %}{% ifchanged n %}..{% endifchanged %}{{ x }}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("..567..567..567") << NoError;

  // Test multiple parameters to ifchanged.

  dict.clear();
  dict.insert(QStringLiteral("num"), QVariantList() << 1 << 1 << 2);
  dict.insert(QStringLiteral("numx"), QVariantList() << 5 << 6 << 6);
  QTest::newRow("ifchanged-param03")
      << QStringLiteral("{% for n in num %}{{ n }}{% for x in numx %}{% ifchanged x n %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("156156256") << NoError;

  // Test a date+hour like construct, where the hour of the last day
  // is the same but the date had changed, so print the hour anyway.

  dict.clear();
  QVariantList days;
  QVariantHash hash;
  hash.insert(QStringLiteral("day"), 1);
  hash.insert(QStringLiteral("hours"), QVariantList() << 1 << 2 << 3);
  days << hash;
  hash.clear();
  hash.insert(QStringLiteral("day"), 2);
  hash.insert(QStringLiteral("hours"), QVariantList() << 3);
  days << hash;
  dict.insert(QStringLiteral("days"), days);
  QTest::newRow("ifchanged-param04")
      << QStringLiteral("{% for d in days %}{% ifchanged %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("112323") << NoError;

  // Logically the same as above, just written with explicit
  // ifchanged for the day.

  QTest::newRow("ifchanged-param05")
      << QStringLiteral("{% for d in days %}{% ifchanged d.day %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d.day h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}")
      << dict << QStringLiteral("112323") << NoError;

  // Test the else clause of ifchanged.
  dict.clear();
  dict.insert(QStringLiteral("ids"), QVariantList() << 1 << 1 << 2 << 2 << 2
                                                    << 3);
  QTest::newRow("ifchanged-else01")
      << QStringLiteral("{% for id in ids %}{{ id }}{% ifchanged id %}-first{% else %}-other{% endifchanged %},{% endfor %}")
      << dict
      << QStringLiteral("1-first,1-other,2-first,2-other,2-other,3-first,")
      << NoError;
  QTest::newRow("ifchanged-else02")
      << QStringLiteral("{% for id in ids %}{{ id }}-{% ifchanged id %}{% cycle red,blue %}{% else %}grey{% endifchanged %},{% endfor %}")
      << dict << QStringLiteral("1-red,1-grey,2-blue,2-grey,2-grey,3-red,")
      << NoError;
  QTest::newRow("ifchanged-else03")
      << QStringLiteral("{% for id in ids %}{{ id }}{% ifchanged id %}-{% cycle red,blue %}{% else %}{% endifchanged %},{% endfor %}")
      << dict << QStringLiteral("1-red,1,2-blue,2,2,3-red,") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("ids"), QVariantList() << 1 << 1 << 2 << 2 << 2
                                                    << 3 << 4);
  QTest::newRow("ifchanged-else04")
      << QStringLiteral("{% for id in ids %}{% ifchanged %}***{{ id }}*{% else %}...{% endifchanged %}{{ forloop.counter }}{% endfor %}")
      << dict << QStringLiteral("***1*1...2***2*3...4...5***3*6***4*7")
      << NoError;
}

void TestDefaultTags::testAutoescapeTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("autoescape-tag01")
      << QStringLiteral("{% autoescape off %}hello{% endautoescape %}") << dict
      << QStringLiteral("hello") << NoError;

  dict.insert(QStringLiteral("first"), QStringLiteral("<b>hello</b>"));
  QTest::newRow("autoescape-tag02")
      << QStringLiteral("{% autoescape off %}{{ first }}{% endautoescape %}")
      << dict << QStringLiteral("<b>hello</b>") << NoError;
  QTest::newRow("autoescape-tag03")
      << QStringLiteral("{% autoescape on %}{{ first }}{% endautoescape %}")
      << dict << QStringLiteral("&lt;b&gt;hello&lt;/b&gt;") << NoError;
  // Autoescape disabling and enabling nest in a predictable way.
  dict.insert(QStringLiteral("first"), QStringLiteral("<a>"));
  QTest::newRow("autoescape-tag04")
      << QStringLiteral("{% autoescape off %}{{ first }} {% autoescape  on%}{{ first }}{% endautoescape %}{% endautoescape %}")
      << dict << QStringLiteral("<a> &lt;a&gt;") << NoError;

  dict.insert(QStringLiteral("first"), QStringLiteral("<b>first</b>"));
  QTest::newRow("autoescape-tag05")
      << QStringLiteral("{% autoescape on %}{{ first }}{% endautoescape %}")
      << dict << QStringLiteral("&lt;b&gt;first&lt;/b&gt;") << NoError;
  // Strings (ASCII or unicode) already marked as "safe" are not
  // auto-escaped
  SafeString safeString(QStringLiteral("<b>first</b>"));
  auto safeStringVar = QVariant::fromValue<SafeString>(markSafe(safeString));
  dict.insert(QStringLiteral("first"), safeStringVar);

  QTest::newRow("autoescape-tag06") << QStringLiteral("{{ first }}") << dict
                                    << QStringLiteral("<b>first</b>")
                                    << NoError;
  QTest::newRow("autoescape-tag07")
      << QStringLiteral("{% autoescape on %}{{ first }}{% endautoescape %}")
      << dict << QStringLiteral("<b>first</b>") << NoError;

  // Literal string arguments to filters, if used in the result, are
  // safe.
  dict.clear();
  dict.insert(QStringLiteral("var"), QVariant());
  QTest::newRow("autoescape-tag08")
      << "{% autoescape on %}{{ var|default_if_none:\"endquote\\\" hah\" "
         "}}{% "
         "endautoescape %}"
      << dict << "endquote\" hah" << NoError;

  QTest::newRow("autoescape-tag09")
      << "{% autoescape on extra %}{{ var|default_if_none:\"endquote\\\" "
         "hah\" "
         "}}{% endautoescape %}"
      << dict << "" << TagSyntaxError;
  QTest::newRow("autoescape-tag10")
      << "{% autoescape bad %}{{ var|default_if_none:\"endquote\\\" hah\" "
         "}}{% "
         "endautoescape %}"
      << dict << "" << TagSyntaxError;

  // Objects which return safe strings as their __unicode__ method
  // won't get double-escaped.
  //   'autoescape-tag09': (r'{{ unsafe }}', {'unsafe':
  //   filters.UnsafeClass()},
  //   'you &amp; me'),
  //   'autoescape-tag10': (r'{{ safe }}', {'safe': filters.SafeClass()}, 'you
  //   &gt; me'),
  // The "safe" and "escape" filters cannot work due to internal
  // implementation details (fortunately, the (no)autoescape block
  // tags can be used in those cases)
  dict.clear();
  dict.insert(QStringLiteral("first"), QStringLiteral("<a>"));
  QTest::newRow("autoescape-filtertag01")
      << QStringLiteral(
             "{{ first }}{% filter safe %}{{ first }} x<y{% endfilter %}")
      << dict << QString() << TagSyntaxError;
  QTest::newRow("autoescape-filtertag02")
      << QStringLiteral(
             "{{ first }}{% filter escape %}{{ first }} x<y{% endfilter %}")
      << dict << QString() << TagSyntaxError;
}

void TestDefaultTags::testMediaFinderTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  QTest::newRow("media_finder-tag01")
      << "{% media_finder \"existing_image.png\" %}" << dict
      << QStringLiteral("file:///path/to/existing_image.png") << NoError;
  QTest::newRow("media_finder-tag02")
      << "{% media_finder \"does_not_exist.png\" %}" << dict << QString()
      << NoError;
  QTest::newRow("media_finder-tag03")
      << "{% media_finder \"existing_image.png\" \"does_not_exist.png\" %}"
      << dict << QStringLiteral("file:///path/to/existing_image.png")
      << NoError;

  dict.insert(QStringLiteral("existing_img"),
              QStringLiteral("existing_image.png"));
  dict.insert(QStringLiteral("nonexisting_img"),
              QStringLiteral("does_not_exist.png"));

  QTest::newRow("media_finder-tag04") << QStringLiteral("{% media_finder %}")
                                      << dict << QString() << TagSyntaxError;
  QTest::newRow("media_finder-tag05")
      << QStringLiteral("{% media_finder existing_img %}") << dict
      << QStringLiteral("file:///path/to/existing_image.png") << NoError;
  QTest::newRow("media_finder-tag06")
      << QStringLiteral("{% media_finder nonexisting_img %}") << dict
      << QString() << NoError;
  QTest::newRow("media_finder-tag07")
      << "{% media_finder \"does_not_exist.png\" existing_img %}" << dict
      << QStringLiteral("file:///path/to/existing_image.png") << NoError;
  QTest::newRow("media_finder-tag08")
      << QStringLiteral("{% media_finder nonexisting_img existing_img %}")
      << dict << QStringLiteral("file:///path/to/existing_image.png")
      << NoError;
  QTest::newRow("media_finder-tag09")
      << "{% media_finder \"existing_image.png\" "
         "\"another_existing_image.png\" %}"
      << dict << QStringLiteral("file:///path/to/existing_image.png")
      << NoError;
  QTest::newRow("media_finder-tag10")
      << "{% media_finder \"another_existing_image.png\" "
         "\"existing_image.png\" %}"
      << dict << QStringLiteral("file:///path/to/another_existing_image.png")
      << NoError;

  dict.insert(QStringLiteral("this_and_that_img"),
              QStringLiteral("this&that.png"));

  QTest::newRow("media_finder-tag11")
      << "{% media_finder \"this&that.png\" %}" << dict
      << QStringLiteral("file:///path/to/this&amp;that.png") << NoError;
  QTest::newRow("media_finder-tag12")
      << "{% media_finder this_and_that_img %}" << dict
      << QStringLiteral("file:///path/to/this&amp;that.png") << NoError;
  QTest::newRow("media_finder-tag13")
      << "{% autoescape off %}{% media_finder \"this&that.png\" %}{% "
         "endautoescape %}"
      << dict << QStringLiteral("file:///path/to/this&that.png") << NoError;
  QTest::newRow("media_finder-tag14")
      << "{% autoescape off %}{% media_finder this_and_that_img %}{% "
         "endautoescape %}"
      << dict << QStringLiteral("file:///path/to/this&that.png") << NoError;
}

void TestDefaultTags::testRangeTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("range-tag01")
      << QStringLiteral("{% range 5 as i %}{{ i }};{% endrange %}") << dict
      << QStringLiteral("0;1;2;3;4;") << NoError;
  QTest::newRow("range-tag02")
      << QStringLiteral("{% range 1 6 as i %}{{ i }};{% endrange %}") << dict
      << QStringLiteral("1;2;3;4;5;") << NoError;
  QTest::newRow("range-tag03")
      << QStringLiteral("{% range 5 26 5 as i %}{{ i }};{% endrange %}") << dict
      << QStringLiteral("5;10;15;20;25;") << NoError;

  QVariantList list;
  list << 10 << 15 << 2;
  dict.insert(QStringLiteral("values"), list);

  QTest::newRow("range-tag04")
      << QStringLiteral("{% range values.0 values.1 values.2 as i %}{{ i }};{% endrange %}")
      << dict << QStringLiteral("10;12;14;") << NoError;

  QTest::newRow("range-tag05")
      << QStringLiteral("{% range 5 %}Foo;{% endrange %}") << dict
      << QStringLiteral("Foo;Foo;Foo;Foo;Foo;") << NoError;
  QTest::newRow("range-tag06")
      << QStringLiteral("{% range 5 6 %}Foo;{% endrange %}") << dict
      << QString() << TagSyntaxError;
  QTest::newRow("range-tag07")
      << QStringLiteral("{% range 5 6 7 %}Foo;{% endrange %}") << dict
      << QString() << TagSyntaxError;
}

void TestDefaultTags::testDebugTag_data()
{

  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("debug-tag01")
      << QStringLiteral("{% debug %}") << dict
      << QStringLiteral("\n\nContext:\nEnd context:\n\n") << NoError;
  dict.insert(QStringLiteral("answer"), 42);
  QTest::newRow("debug-tag02")
      << QStringLiteral("{% debug %}") << dict
      << QStringLiteral("\n\nContext:\nkey answer, type int\nEnd context:\n\n")
      << NoError;
}

void TestDefaultTags::testLoadTag_data()
{

  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("load-tag01") << QStringLiteral("{% load does_not_exist %}foo")
                              << dict << QString() << TagSyntaxError;
}

void TestDefaultTags::testUrlTypes_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QPair<QString, QString>>("output");

  Dict dict;
  QTest::newRow("url-types01")
      << "{% media_finder \"existing_image.png\" %}" << dict
      << qMakePair(QStringLiteral("file:///path/to/"),
                   QStringLiteral("existing_image.png"));

  QTest::newRow("url-types02") << "{% media_finder \"does_not_exist.png\" %}"
                               << dict << qMakePair(QString(), QString());

  dict.insert(QStringLiteral("existing_img"),
              QStringLiteral("existing_image.png"));
  dict.insert(QStringLiteral("nonexisting_img"),
              QStringLiteral("does_not_exist.png"));

  QTest::newRow("url-types03")
      << QStringLiteral("{% media_finder existing_img %}") << dict
      << qMakePair(QStringLiteral("file:///path/to/"),
                   QStringLiteral("existing_image.png"));

  QTest::newRow("url-types04")
      << QStringLiteral("{% media_finder nonexisting_img %}") << dict
      << qMakePair(QString(), QString());
}

void TestDefaultTags::testUrlTypes()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(StringPair, output);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));
  QVERIFY(t->error() == NoError);
  Context c(dict);
  auto result = t->render(&c);
  QVERIFY(t->error() == NoError);
  QVERIFY(result == output.first + output.second);

  c.setUrlType(Context::RelativeUrls);
  result = t->render(&c);
  QVERIFY(t->error() == NoError);
  QVERIFY(result == output.second);
}

void TestDefaultTags::testRelativePaths_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");

  Dict dict;
  QTest::newRow("relativepaths01")
      << "{% media_finder \"existing_image.png\" %}" << dict
      << QStringLiteral("existing_image.png");

  QTest::newRow("relativepaths02")
      << "{% media_finder \"does_not_exist.png\" %}" << dict << QString();

  dict.insert(QStringLiteral("existing_img"),
              QStringLiteral("existing_image.png"));
  dict.insert(QStringLiteral("nonexisting_img"),
              QStringLiteral("does_not_exist.png"));

  QTest::newRow("relativepaths03")
      << QStringLiteral("{% media_finder existing_img %}") << dict
      << QStringLiteral("existing_image.png");

  QTest::newRow("relativepaths04")
      << QStringLiteral("{% media_finder nonexisting_img %}") << dict
      << QString();
}

void TestDefaultTags::testRelativePaths()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));
  QVERIFY(t->error() == NoError);
  Context c(dict);
  auto result = t->render(&c);
  QVERIFY(t->error() == NoError);
  if (!output.isEmpty())
    QVERIFY(result == QStringLiteral("file:///path/to/") + output);
  else
    QVERIFY(result.isEmpty());

  c.setUrlType(Context::RelativeUrls);
  auto relativePath = QStringLiteral("relative/path");
  c.setRelativeMediaPath(relativePath);
  result = t->render(&c);
  QVERIFY(t->error() == NoError);
  if (!output.isEmpty())
    QVERIFY(result == relativePath + QLatin1Char('/') + output);
  else
    QVERIFY(result.isEmpty());
}

QTEST_MAIN(TestDefaultTags)
#include "testdefaulttags.moc"

#endif
