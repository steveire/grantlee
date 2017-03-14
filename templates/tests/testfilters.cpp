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

#ifndef FILTERSTEST_H
#define FILTERSTEST_H

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtTest/QTest>

#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "grantlee_paths.h"
#include "template.h"
#include <util.h>

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestFilters : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void testDateBasedFilters_data();
  void testDateBasedFilters() { doTest(); }

  void testStringFilters_data();
  void testStringFilters() { doTest(); }

  void testListFilters_data();
  void testListFilters() { doTest(); }

  void testLogicFilters_data();
  void testLogicFilters() { doTest(); }

  void testMiscFilters_data();
  void testMiscFilters() { doTest(); }

  void testIntegerFilters_data();
  void testIntegerFilters() { doTest(); }

private:
  void doTest();

  QSharedPointer<InMemoryTemplateLoader> loader;
  Engine *m_engine;
};

void TestFilters::initTestCase()
{
  m_engine = new Engine(this);

  loader = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());
  m_engine->addTemplateLoader(loader);

  auto appDirPath
      = QFileInfo(QCoreApplication::applicationDirPath()).absoluteDir().path();
  m_engine->setPluginPaths(
      QStringList() << QStringLiteral(GRANTLEE_PLUGIN_PATH)
                    << appDirPath + QStringLiteral("/tests/") // For testtags.qs
      );
}

void TestFilters::cleanupTestCase() { delete m_engine; }

void TestFilters::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

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
}

void TestFilters::testDateBasedFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  auto now = QDateTime::currentDateTimeUtc();

  dict.insert(QStringLiteral("a"), now.addSecs(-70));

  QTest::newRow("filter-timesince01") << QStringLiteral("{{ a|timesince }}")
                                      << dict << QStringLiteral("1 minute")
                                      << NoError;

  dict.clear();

  dict.insert(QStringLiteral("a"), now.addDays(-1).addSecs(-60));

  QTest::newRow("filter-timesince02") << QStringLiteral("{{ a|timesince }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();

  dict.insert(QStringLiteral("a"),
              now.addSecs(-1 * 60 * 60).addSecs(-1 * 25 * 60).addSecs(-1 * 10));
  QTest::newRow("filter-timesince03")
      << QStringLiteral("{{ a|timesince }}") << dict
      << QStringLiteral("1 hour, 25 minutes") << NoError;

  dict.clear();

  //  Compare to a given parameter

  dict.insert(QStringLiteral("a"), now.addDays(-2));
  dict.insert(QStringLiteral("b"), now.addDays(-1));

  QTest::newRow("filter-timesince04") << QStringLiteral("{{ a|timesince:b }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();

  dict.insert(QStringLiteral("a"), now.addDays(-2).addSecs(-60));
  dict.insert(QStringLiteral("b"), now.addDays(-2));

  QTest::newRow("filter-timesince05") << QStringLiteral("{{ a|timesince:b }}")
                                      << dict << QStringLiteral("1 minute")
                                      << NoError;

  dict.clear();

  //  Check that timezone is respected

  //  {"a":now_tz - timedelta(hours=8), "b":now_tz
  //   QTest::newRow( "filter-timesince06" ) << QString::fromLatin1( "{{
  //   a|timesince:b }}" ) << dict << QString::fromLatin1( "8 hours" ) <<
  //   NoError;

  dict.insert(QStringLiteral("earlier"), now.addDays(-7));
  QTest::newRow("filter-timesince07")
      << QStringLiteral("{{ earlier|timesince }}") << dict
      << QStringLiteral("1 week") << NoError;

  dict.clear();

  dict.insert(QStringLiteral("now"), now);
  dict.insert(QStringLiteral("earlier"), now.addDays(-7));

  QTest::newRow("filter-timesince08")
      << QStringLiteral("{{ earlier|timesince:now }}") << dict
      << QStringLiteral("1 week") << NoError;

  dict.clear();

  dict.insert(QStringLiteral("later"), now.addDays(7));

  QTest::newRow("filter-timesince09") << QStringLiteral("{{ later|timesince }}")
                                      << dict << QStringLiteral("0 minutes")
                                      << NoError;

  dict.clear();

  dict.insert(QStringLiteral("now"), now);
  dict.insert(QStringLiteral("later"), now.addDays(7));

  QTest::newRow("filter-timesince10")
      << QStringLiteral("{{ later|timesince:now }}") << dict
      << QStringLiteral("0 minutes") << NoError;

  //  Ensures that differing timezones are calculated correctly

  //  {"a": now
  //   QTest::newRow( "filter-timesince11" ) << QString::fromLatin1( "{{
  //   a|timesince }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;

  //  {"a": now_tz
  //   QTest::newRow( "filter-timesince12" ) << QString::fromLatin1( "{{
  //   a|timesince }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;

  //  {"a": now_tz_i
  //   QTest::newRow( "filter-timesince13" ) << QString::fromLatin1( "{{
  //   a|timesince }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;

  //  {"a": now_tz, "b": now_tz_i
  //   QTest::newRow( "filter-timesince14" ) << QString::fromLatin1( "{{
  //   a|timesince:b }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;

  //  {"a": now, "b": now_tz_i
  //   QTest::newRow( "filter-timesince15" ) << QString::fromLatin1( "{{
  //   a|timesince:b }}" ) << dict << QString() << NoError;

  //  {"a": now_tz_i, "b": now
  //   QTest::newRow( "filter-timesince16" ) << QString::fromLatin1( "{{
  //   a|timesince:b }}" ) << dict << QString() << NoError;

  dict.clear();

  dict.insert(QStringLiteral("a"), now);
  dict.insert(QStringLiteral("b"), now);

  QTest::newRow("filter-timesince17") << QStringLiteral("{{ a|timesince:b }}")
                                      << dict << QStringLiteral("0 minutes")
                                      << NoError;

  dict.clear();

  dict.insert(QStringLiteral("a"), now);
  dict.insert(QStringLiteral("b"), now.addDays(1));

  QTest::newRow("filter-timesince18") << QStringLiteral("{{ a|timesince:b }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();
  QTest::newRow("filter-timesince19") << QStringLiteral("{{xx|timesince}}") << dict
                                      << QStringLiteral("") << NoError;
  QTest::newRow("filter-timesince20") << QStringLiteral("{{|timesince}}") << dict
                                      << QStringLiteral("") << NoError;

  //  Default compare with datetime.now()

  dict.clear();
  dict.insert(QStringLiteral("a"), now.addSecs(130));

  QTest::newRow("filter-timeuntil01") << QStringLiteral("{{ a|timeuntil }}")
                                      << dict << QStringLiteral("2 minutes")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), now.addDays(1).addSecs(10));

  QTest::newRow("filter-timeuntil02") << QStringLiteral("{{ a|timeuntil }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), now.addSecs(60 * 60 * 8).addSecs(610));

  QTest::newRow("filter-timeuntil03")
      << QStringLiteral("{{ a|timeuntil }}") << dict
      << QStringLiteral("8 hours, 10 minutes") << NoError;

  //  Compare to a given parameter

  dict.clear();
  dict.insert(QStringLiteral("a"), now.addDays(-1));
  dict.insert(QStringLiteral("b"), now.addDays(-2));

  QTest::newRow("filter-timeuntil04") << QStringLiteral("{{ a|timeuntil:b }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), now.addDays(-1));
  dict.insert(QStringLiteral("b"), now.addDays(-1).addSecs(-60));

  QTest::newRow("filter-timeuntil05") << QStringLiteral("{{ a|timeuntil:b }}")
                                      << dict << QStringLiteral("1 minute")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("earlier"), now.addDays(-7));

  QTest::newRow("filter-timeuntil06")
      << QStringLiteral("{{ earlier|timeuntil }}") << dict
      << QStringLiteral("0 minutes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("now"), now);
  dict.insert(QStringLiteral("earlier"), now.addDays(-7));

  QTest::newRow("filter-timeuntil07")
      << QStringLiteral("{{ earlier|timeuntil:now }}") << dict
      << QStringLiteral("0 minutes") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("later"), now.addDays(7).addSecs(5));

  QTest::newRow("filter-timeuntil08") << QStringLiteral("{{ later|timeuntil }}")
                                      << dict << QStringLiteral("1 week")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("now"), now);
  dict.insert(QStringLiteral("later"), now.addDays(7));

  QTest::newRow("filter-timeuntil09")
      << QStringLiteral("{{ later|timeuntil:now }}") << dict
      << QStringLiteral("1 week") << NoError;

  //  Ensures that differing timezones are calculated correctly
  //
  //   //  {"a": now_tz_i
  //   QTest::newRow( "filter-timeuntil10" ) << QString::fromLatin1( "{{
  //   a|timeuntil }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;
  //
  //   //  {"a": now_tz_i, "b": now_tz
  //   QTest::newRow( "filter-timeuntil11" ) << QString::fromLatin1( "{{
  //   a|timeuntil:b }}" ) << dict << QString::fromLatin1( "0 minutes" ) <<
  //   NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), now);
  dict.insert(QStringLiteral("b"), now);
  QTest::newRow("filter-timeuntil12") << QStringLiteral("{{ a|timeuntil:b }}")
                                      << dict << QStringLiteral("0 minutes")
                                      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), now);
  dict.insert(QStringLiteral("b"), now.addDays(-1));

  QTest::newRow("filter-timeuntil13") << QStringLiteral("{{ a|timeuntil:b }}")
                                      << dict << QStringLiteral("1 day")
                                      << NoError;

  dict.clear();
  QTest::newRow("filter-timeuntil14") << QStringLiteral("{{xx|timeuntil}}") << dict
                                      << QStringLiteral("") << NoError;
  QTest::newRow("filter-timeuntil15") << QStringLiteral("{{|timeuntil}}") << dict
                                      << QStringLiteral("") << NoError;

  QDateTime d(QDate(2008, 1, 1));

  dict.clear();
  dict.insert(QStringLiteral("d"), d);

  QTest::newRow("date01") << "{{ d|date:\"MM\" }}" << dict
                          << QStringLiteral("01") << NoError;
  QTest::newRow("date02") << QStringLiteral("{{ d|date }}") << dict
                          << d.toString(QStringLiteral("MMM. d, yyyy"))
                          << NoError;

  dict.clear();
  dict.insert(QStringLiteral("d"), QStringLiteral("fail_string"));
  QTest::newRow("date03") << "{{ d|date:\"MM\" }}" << dict << QString()
                          << NoError;
}

void TestFilters::testStringFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("<a>\'"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("<a>\'"))));

  QTest::newRow("filter-addslash01")
      << QStringLiteral("{% autoescape off %}{{ a|addslashes }} {{ b|addslashes }}{% endautoescape %}")
      << dict << "<a>\\\' <a>\\\'" << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("<a>\'"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("<a>\'"))));

  QTest::newRow("filter-addslash02")
      << QStringLiteral("{{ a|addslashes }} {{ b|addslashes }}") << dict
      << "&lt;a&gt;\\&#39; <a>\\\'" << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("fred>"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("fred&gt;"))));

  QTest::newRow("filter-capfirst01")
      << QStringLiteral("{% autoescape off %}{{ a|capfirst }} {{ b|capfirst }}{% endautoescape %}")
      << dict << QStringLiteral("Fred> Fred&gt;") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("fred>"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("fred&gt;"))));

  QTest::newRow("filter-capfirst02")
      << QStringLiteral("{{ a|capfirst }} {{ b|capfirst }}") << dict
      << QStringLiteral("Fred&gt; Fred&gt;") << NoError;

  //  Note that applying fix_ampsersands in autoescape mode leads to
  //  double escaping.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a&b"))));

  QTest::newRow("filter-fix_ampersands01")
      << QStringLiteral("{% autoescape off %}{{ a|fix_ampersands }} {{ b|fix_ampersands }}{% endautoescape %}")
      << dict << QStringLiteral("a&amp;b a&amp;b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a&b"))));

  QTest::newRow("filter-fix_ampersands02")
      << QStringLiteral("{{ a|fix_ampersands }} {{ b|fix_ampersands }}") << dict
      << QStringLiteral("a&amp;amp;b a&amp;b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("1.42"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("1.42"))));

  QTest::newRow("filter-floatformat01")
      << QStringLiteral("{% autoescape off %}{{ a|floatformat }} {{ b|floatformat }}{% endautoescape %}")
      << dict << QStringLiteral("1.4 1.4") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("1.42"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("1.42"))));

  QTest::newRow("filter-floatformat02")
      << QStringLiteral("{{ a|floatformat }} {{ b|floatformat }}") << dict
      << QStringLiteral("1.4 1.4") << NoError;

  //  The contents of "linenumbers" is escaped according to the current
  //  autoescape setting.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("one\n<two>\nthree"));
  dict.insert(
      QStringLiteral("b"),
      QVariant::fromValue(markSafe(QStringLiteral("one\n&lt;two&gt;\nthree"))));

  QTest::newRow("filter-linenumbers01")
      << QStringLiteral("{{ a|linenumbers }} {{ b|linenumbers }}") << dict
      << "1. one\n2. &lt;two&gt;\n3. three 1. one\n2. &lt;two&gt;\n3. three"
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("one\n<two>\nthree"));
  dict.insert(
      QStringLiteral("b"),
      QVariant::fromValue(markSafe(QStringLiteral("one\n&lt;two&gt;\nthree"))));
  QTest::newRow("filter-linenumbers02")
      << QStringLiteral("{% autoescape off %}{{ a|linenumbers }} {{ b|linenumbers }}{% endautoescape %}")
      << dict << "1. one\n2. <two>\n3. three 1. one\n2. &lt;two&gt;\n3. three"
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("Apple & banana"));
  dict.insert(QStringLiteral("b"), QVariant::fromValue(markSafe(
                                       QStringLiteral("Apple &amp; banana"))));

  QTest::newRow("filter-lower01")
      << QStringLiteral("{% autoescape off %}{{ a|lower }} {{ b|lower }}{% endautoescape %}")
      << dict << QStringLiteral("apple & banana apple &amp; banana") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("Apple & banana"));
  dict.insert(QStringLiteral("b"), QVariant::fromValue(markSafe(
                                       QStringLiteral("Apple &amp; banana"))));

  QTest::newRow("filter-lower02")
      << QStringLiteral("{{ a|lower }} {{ b|lower }}") << dict
      << QStringLiteral("apple &amp; banana apple &amp; banana") << NoError;

  //  The make_list filter can destroy existing escaping, so the results are
  //  escaped.

  dict.clear();
  dict.insert(QStringLiteral("a"), markSafe(QStringLiteral("&")));

  QTest::newRow("filter-make_list01")
      << QStringLiteral(
             "{% autoescape off %}{{ a|make_list }}{% endautoescape %}")
      << dict << "[u\'&\']" << NoError;
  QTest::newRow("filter-make_list02")
      << QStringLiteral("{{ a|make_list }}") << dict
      << QStringLiteral("[u&#39;&amp;&#39;]") << NoError;

  QTest::newRow("filter-make_list03")
      << QStringLiteral("{% autoescape off %}{{ a|make_list|stringformat:\"%1\"|safe }}{% endautoescape %}")
      << dict << QStringLiteral("[u\'&\']") << NoError;
  QTest::newRow("filter-make_list04")
      << QStringLiteral("{{ a|make_list|stringformat:\"%1\"|safe }}") << dict
      << QStringLiteral("[u\'&\']") << NoError;

  //  Running slugify on a pre-escaped string leads to odd behaviour,
  //  but the result is still safe.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a & b"));
  dict.insert(QStringLiteral("b"), markSafe(QStringLiteral("a &amp; b")));

  QTest::newRow("filter-slugify01")
      << QStringLiteral("{% autoescape off %}{{ a|slugify }} {{ b|slugify }}{% endautoescape %}")
      << dict << QStringLiteral("a-b a-amp-b") << NoError;
  QTest::newRow("filter-slugify02")
      << QStringLiteral("{{ a|slugify }} {{ b|slugify }}") << dict
      << QStringLiteral("a-b a-amp-b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("Schöne Grüße"));

  QTest::newRow("filter-slugify03") << QStringLiteral("{{ a|slugify }}") << dict
                                    << QStringLiteral("schone-grue") << NoError;

  dict.clear();
  dict.insert(
      QStringLiteral("a"),
      QStringLiteral("testing\r\njavascript \'string\" <b>escaping</b>"));
  QTest::newRow("escapejs01")
      << QStringLiteral("{{ a|escapejs }}") << dict
      << "testing\\u000D\\u000Ajavascript \\u0027string\\u0022 "
         "\\u003Cb\\u003Eescaping\\u003C/b\\u003E"
      << NoError;
  QTest::newRow("escapejs02")
      << QStringLiteral(
             "{% autoescape off %}{{ a|escapejs }}{% endautoescape %}")
      << dict << "testing\\u000D\\u000Ajavascript \\u0027string\\u0022 "
                 "\\u003Cb\\u003Eescaping\\u003C/b\\u003E"
      << NoError;

  //  Notice that escaping is applied *after* any filters, so the string
  //  formatting here only needs to deal with pre-escaped characters.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a<b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a<b"))));

  QTest::newRow("filter-stringformat01")
      << "{% autoescape off %}.{{ a|stringformat:\"%1\" }}. .{{ "
         "b|stringformat:\"%2\" }}.{% endautoescape %}"
      << dict << QStringLiteral(".a<b. .a<b.") << NoError;
  QTest::newRow("filter-stringformat02")
      << ".{{ a|stringformat:\"%1\" }}. .{{ b|stringformat:\"%2\" }}." << dict
      << QStringLiteral(".a&lt;b. .a<b.") << NoError;
  QTest::newRow("filter-stringformat03")
      << ".{{ a|stringformat:\"foo %1 bar\" }}. .{{ b|stringformat:\"baz %2 "
         "bat\" }}."
      << dict << QStringLiteral(".foo a&lt;b bar. .baz a<b bat.") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("path"), QStringLiteral("www.grantlee.org"));
  QTest::newRow("filter-stringformat04")
      << "{% with path|stringformat:\"<a href=\\\"%1\\\">%1</a>\"|safe as "
         "result %}{{ result }}{% endwith %}"
      << dict << "<a href=\"www.grantlee.org\">www.grantlee.org</a>" << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("JOE\'S CRAB SHACK"));
  QTest::newRow("filter-title01") << "{{ a|title }}" << dict
                                  << QStringLiteral("Joe&#39;s Crab Shack")
                                  << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("555 WEST 53RD STREET"));
  QTest::newRow("filter-title02") << "{{ a|title }}" << dict
                                  << QStringLiteral("555 West 53rd Street")
                                  << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("alpha & bravo"));
  dict.insert(QStringLiteral("b"), QVariant::fromValue(markSafe(
                                       QStringLiteral("alpha &amp; bravo"))));

  QTest::newRow("filter-truncatewords01")
      << "{% autoescape off %}{{ a|truncatewords:\"2\" }} {{ "
         "b|truncatewords:\"2\"}}{% endautoescape %}"
      << dict << QStringLiteral("alpha & ... alpha &amp; ...") << NoError;

  QTest::newRow("filter-truncatewords02")
      << "{{ a|truncatewords:\"2\" }} {{ b|truncatewords:\"2\"}}" << dict
      << QStringLiteral("alpha &amp; ... alpha &amp; ...") << NoError;

  dict.clear();
  QTest::newRow("filter-truncatewords03") << "{{xx|truncatewords:\"0\"}}" << dict
                                          << QString() << NoError;
  QTest::newRow("filter-truncatewords04") << "{{xx|truncatewords:\"-1\"}}" << dict
                                          << QString() << NoError;

  //  The "upper" filter messes up entities (which are case-sensitive),
  //  so it's not safe for non-escaping purposes.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a & b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a &amp; b"))));

  QTest::newRow("filter-upper01")
      << QStringLiteral("{% autoescape off %}{{ a|upper }} {{ b|upper }}{% endautoescape %}")
      << dict << QStringLiteral("A & B A &AMP; B") << NoError;
  QTest::newRow("filter-upper02")
      << QStringLiteral("{{ a|upper }} {{ b|upper }}") << dict
      << QStringLiteral("A &amp; B A &amp;AMP; B") << NoError;

  //   //  {"a": "http://example.com/?x=&y=", "b":
  //   mark_safe("http://example.com?x=&amp;y=")
  //   QTest::newRow( "filter-urlize01") << QString::fromLatin1( "{%
  //   autoescape
  //   off %}{{ a|urlize }} {{ b|urlize }}{% endautoescape %}" ) << dict <<
  //   "<a
  //   href=\"http://example.com/?x=&y=\"
  //   rel=\"nofollow\">http://example.com/?x=&y=</a> <a
  //   href=\"http://example.com?x=&amp;y=\"
  //   rel=\"nofollow\">http://example.com?x=&amp;y=</a>" << NoError;
  //
  //   //  {"a": "http://example.com/?x=&y=", "b":
  //   mark_safe("http://example.com?x=&amp;y=")
  //   QTest::newRow( "filter-urlize02") << QString::fromLatin1( "{{ a|urlize
  //   }}
  //   {{ b|urlize }}" ) << dict << "<a href=\"http://example.com/?x=&amp;y=\"
  //   rel=\"nofollow\">http://example.com/?x=&amp;y=</a> <a
  //   href=\"http://example.com?x=&amp;y=\"
  //   rel=\"nofollow\">http://example.com?x=&amp;y=</a>" << NoError;
  //
  //   //  {"a": mark_safe("a &amp; b")
  //   QTest::newRow( "filter-urlize03") << QString::fromLatin1( "{%
  //   autoescape
  //   off %}{{ a|urlize }}{% endautoescape %}" ) << dict <<
  //   QString::fromLatin1( "a &amp; b" ) << NoError;
  //
  //   //  {"a": mark_safe("a &amp; b")
  //   QTest::newRow( "filter-urlize04") << QString::fromLatin1( "{{ a|urlize
  //   }}" ) << dict << QString::fromLatin1( "a &amp; b" ) << NoError;
  //
  //   //  This will lead to a nonsense result, but at least it won't be
  //
  //   //  exploitable for XSS purposes when auto-escaping is on.
  //
  //   //  {"a": "<script>alert(\"foo\")</script>"
  //   QTest::newRow( "filter-urlize05") << QString::fromLatin1( "{%
  //   autoescape
  //   off %}{{ a|urlize }}{% endautoescape %}" ) << dict <<
  //   "<script>alert(\"foo\")</script>" << NoError;
  //
  //   //  {"a": "<script>alert(\"foo\")</script>"
  //   QTest::newRow( "filter-urlize06") << QString::fromLatin1( "{{ a|urlize
  //   }}" ) << dict << QString::fromLatin1(
  //   "&lt;script&gt;alert(&#39;foo&#39;)&lt;/script&gt;" ) << NoError;
  //
  //   //  mailto: testing for urlize
  //
  //   //  {"a": "Email me at me@example.com"
  //   QTest::newRow( "filter-urlize07") << QString::fromLatin1( "{{ a|urlize
  //   }}" ) << dict << "Email me at <a
  //   href=\"mailto:me@example.com\">me@example.com</a>" << NoError;
  //
  //   //  {"a": "Email me at <me@example.com>"
  //   QTest::newRow( "filter-urlize08") << QString::fromLatin1( "{{ a|urlize
  //   }}" ) << dict << "Email me at &lt;<a
  //   href=\"mailto:me@example.com\">me@example.com</a>&gt;" << NoError;
  //
  //   //  {"a": "\"Unsafe\" http://example.com/x=&y=", "b":
  //   mark_safe("&quot;Safe&quot; http://example.com?x=&amp;y=")
  //   QTest::newRow( "filter-urlizetrunc01") << "{% autoescape off %}{{
  //   a|urlizetrunc:\"8\" }} {{ b|urlizetrunc:\"8\" }}{% endautoescape %}" <<
  //   dict << "\"Unsafe\" <a href=\"http://example.com/x=&y=\"
  //   rel=\"nofollow\">http:...</a> &quot;Safe&quot; <a
  //   href=\"http://example.com?x=&amp;y=\" rel=\"nofollow\">http:...</a>" <<
  //   NoError;
  //
  //   //  {"a": "\"Unsafe\" http://example.com/x=&y=", "b":
  //   mark_safe("&quot;Safe&quot; http://example.com?x=&amp;y=")
  //   QTest::newRow( "filter-urlizetrunc02") << "{{ a|urlizetrunc:\"8\" }} {{
  //   b|urlizetrunc:\"8\" }}" << dict << "&quot;Unsafe&quot; <a
  //   href=\"http://example.com/x=&amp;y=\" rel=\"nofollow\">http:...</a>
  //   &quot;Safe&quot; <a href=\"http://example.com?x=&amp;y=\"
  //   rel=\"nofollow\">http:...</a>" << NoError;

  //   //  Ensure iriencode keeps safe strings:
  //
  //   //  {"url": "?test=1&me=2"
  //   QTest::newRow( "filter-iriencode01") << QString::fromLatin1( "{{
  //   url|iriencode }}" ) << dict << QString::fromLatin1( "?test=1&amp;me=2"
  //   )
  //   << NoError;
  //
  //   //  {"url": "?test=1&me=2"
  //   QTest::newRow( "filter-iriencode02") << QString::fromLatin1( "{%
  //   autoescape off %}{{ url|iriencode }}{% endautoescape %}" ) << dict <<
  //   QString::fromLatin1( "?test=1&me=2" ) << NoError;
  //
  //   //  {"url": mark_safe("?test=1&me=2")
  //   QTest::newRow( "filter-iriencode03") << QString::fromLatin1( "{{
  //   url|iriencode }}" ) << dict << QString::fromLatin1( "?test=1&me=2" ) <<
  //   NoError;
  //
  //   //  {"url": mark_safe("?test=1&me=2")
  //   QTest::newRow( "filter-iriencode04") << QString::fromLatin1( "{%
  //   autoescape off %}{{ url|iriencode }}{% endautoescape %}" ) << dict <<
  //   QString::fromLatin1( "?test=1&me=2" ) << NoError;
  //
  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a & b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a &amp; b"))));

  QTest::newRow("filter-wordcount01")
      << QStringLiteral("{% autoescape off %}{{ a|wordcount }} {{ b|wordcount }}{% endautoescape %}")
      << dict << QStringLiteral("3 3") << NoError;

  QTest::newRow("filter-wordcount02")
      << QStringLiteral("{{ a|wordcount }} {{ b|wordcount }}") << dict
      << QStringLiteral("3 3") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a & b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a & b"))));

  QTest::newRow("filter-wordwrap01")
      << QStringLiteral("{% autoescape off %}{{ a|wordwrap:3 }} {{ b|wordwrap:3 }}{% endautoescape %}")
      << dict << "a &\nb a &\nb" << NoError;

  QTest::newRow("filter-wordwrap02")
      << QStringLiteral("{{ a|wordwrap:3 }} {{ b|wordwrap:3 }}") << dict
      << "a &amp;\nb a &\nb" << NoError;

  dict.clear();
  QTest::newRow("filter-wordwrap03")
      << QStringLiteral("{{xx|wordwrap}}") << dict
      << "" << NoError;

  QTest::newRow("filter-wordwrap04")
      << QStringLiteral("{{|wordwrap}}") << dict
      << "" << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a&b"))));

  QTest::newRow("filter-ljust01")
      << "{% autoescape off %}.{{ a|ljust:\"5\" }}. .{{ b|ljust:\"5\" }}.{% "
         "endautoescape %}"
      << dict << QStringLiteral(".a&b  . .a&b  .") << NoError;

  QTest::newRow("filter-ljust02")
      << ".{{ a|ljust:\"5\" }}. .{{ b|ljust:\"5\" }}." << dict
      << QStringLiteral(".a&amp;b  . .a&b  .") << NoError;

  QTest::newRow("filter-rjust01")
      << "{% autoescape off %}.{{ a|rjust:\"5\" }}. .{{ b|rjust:\"5\" }}.{% "
         "endautoescape %}"
      << dict << QStringLiteral(".  a&b. .  a&b.") << NoError;

  QTest::newRow("filter-rjust02")
      << ".{{ a|rjust:\"5\" }}. .{{ b|rjust:\"5\" }}." << dict
      << QStringLiteral(".  a&amp;b. .  a&b.") << NoError;

  QTest::newRow("filter-center01")
      << "{% autoescape off %}.{{ a|center:\"5\" }}. .{{ b|center:\"5\" "
         "}}.{% "
         "endautoescape %}"
      << dict << QStringLiteral(". a&b . . a&b .") << NoError;

  QTest::newRow("filter-center02")
      << ".{{ a|center:\"5\" }}. .{{ b|center:\"5\" }}." << dict
      << QStringLiteral(". a&amp;b . . a&b .") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("x&y"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("x&amp;y"))));

  QTest::newRow("filter-cut01") << "{% autoescape off %}{{ a|cut:\"x\" }} {{ "
                                   "b|cut:\"x\" }}{% endautoescape %}"
                                << dict << QStringLiteral("&y &amp;y")
                                << NoError;
  QTest::newRow("filter-cut02") << "{{ a|cut:\"x\" }} {{ b|cut:\"x\" }}" << dict
                                << QStringLiteral("&amp;y &amp;y") << NoError;
  QTest::newRow("filter-cut03") << "{% autoescape off %}{{ a|cut:\"&\" }} {{ "
                                   "b|cut:\"&\" }}{% endautoescape %}"
                                << dict << QStringLiteral("xy xamp;y")
                                << NoError;
  QTest::newRow("filter-cut04") << "{{ a|cut:\"&\" }} {{ b|cut:\"&\" }}" << dict
                                << QStringLiteral("xy xamp;y") << NoError;

  //  Passing ";" to cut can break existing HTML entities, so those strings
  //  are auto-escaped.

  QTest::newRow("filter-cut05") << "{% autoescape off %}{{ a|cut:\";\" }} {{ "
                                   "b|cut:\";\" }}{% endautoescape %}"
                                << dict << QStringLiteral("x&y x&ampy")
                                << NoError;
  QTest::newRow("filter-cut06") << "{{ a|cut:\";\" }} {{ b|cut:\";\" }}" << dict
                                << QStringLiteral("x&amp;y x&amp;ampy")
                                << NoError;

  //  The "escape" filter works the same whether autoescape is on or off,
  //  but it has no effect on strings already marked as safe.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("x&y"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("x&y"))));

  QTest::newRow("filter-escape01")
      << QStringLiteral("{{ a|escape }} {{ b|escape }}") << dict
      << QStringLiteral("x&amp;y x&y") << NoError;
  QTest::newRow("filter-escape02")
      << QStringLiteral("{% autoescape off %}{{ a|escape }} {{ b|escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;y x&y") << NoError;

  //  It is only applied once, regardless of the number of times it
  //  appears in a chain.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("x&y"));

  QTest::newRow("filter-escape03")
      << QStringLiteral(
             "{% autoescape off %}{{ a|escape|escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-escape04") << QStringLiteral("{{ a|escape|escape }}")
                                   << dict << QStringLiteral("x&amp;y")
                                   << NoError;

  //  Force_escape is applied immediately. It can be used to provide
  //  double-escaping, for example.

  QTest::newRow("filter-force-escape01")
      << QStringLiteral(
             "{% autoescape off %}{{ a|force_escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-force-escape02")
      << QStringLiteral("{{ a|force_escape }}") << dict
      << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-force-escape03")
      << QStringLiteral("{% autoescape off %}{{ a|force_escape|force_escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;amp;y") << NoError;
  QTest::newRow("filter-force-escape04")
      << QStringLiteral("{{ a|force_escape|force_escape }}") << dict
      << QStringLiteral("x&amp;amp;y") << NoError;

  //  Because the result of force_escape is "safe", an additional
  //  escape filter has no effect.

  QTest::newRow("filter-force-escape05")
      << QStringLiteral("{% autoescape off %}{{ a|force_escape|escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-force-escape06")
      << QStringLiteral("{{ a|force_escape|escape }}") << dict
      << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-force-escape07")
      << QStringLiteral("{% autoescape off %}{{ a|escape|force_escape }}{% endautoescape %}")
      << dict << QStringLiteral("x&amp;y") << NoError;
  QTest::newRow("filter-force-escape08")
      << QStringLiteral("{{ a|escape|force_escape }}") << dict
      << QStringLiteral("x&amp;y") << NoError;

  //  The contents in "linebreaks" and "linebreaksbr" are escaped
  //  according to the current autoescape setting.

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("x&\ny"));
  dict.insert(QStringLiteral("b"), markSafe(QStringLiteral("x&\ny")));

  QTest::newRow("filter-linebreaks01")
      << QStringLiteral("{{ a|linebreaks }} {{ b|linebreaks }}") << dict
      << QStringLiteral("<p>x&amp;<br />y</p> <p>x&<br />y</p>") << NoError;
  QTest::newRow("filter-linebreaks02")
      << QStringLiteral("{% autoescape off %}{{ a|linebreaks }} {{ b|linebreaks }}{% endautoescape %}")
      << dict << QStringLiteral("<p>x&<br />y</p> <p>x&<br />y</p>") << NoError;
  QTest::newRow("filter-linebreaksbr01")
      << QStringLiteral("{{ a|linebreaksbr }} {{ b|linebreaksbr }}") << dict
      << QStringLiteral("x&amp;<br />y x&<br />y") << NoError;
  QTest::newRow("filter-linebreaksbr02")
      << QStringLiteral("{% autoescape off %}{{ a|linebreaksbr }} {{ b|linebreaksbr }}{% endautoescape %}")
      << dict << QStringLiteral("x&<br />y x&<br />y") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("<b>hello</b>"));

  QTest::newRow("filter-safe01")
      << QStringLiteral("{{ a }} -- {{ a|safe }}") << dict
      << QStringLiteral("&lt;b&gt;hello&lt;/b&gt; -- <b>hello</b>") << NoError;
  QTest::newRow("filter-safe02")
      << QStringLiteral(
             "{% autoescape off %}{{ a }} -- {{ a|safe }}{% endautoescape %}")
      << dict << QStringLiteral("<b>hello</b> -- <b>hello</b>") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("&")
                                                  << QStringLiteral("<"));

  QTest::newRow("filter-safeseq01")
      << "{{ a|join:\", \" }} -- {{ a|safeseq|join:\", \" }}" << dict
      << QStringLiteral("&amp;, &lt; -- &, <") << NoError;
  QTest::newRow("filter-safeseq02") << "{% autoescape off %}{{ a|join:\", \" "
                                       "}} -- {{ a|safeseq|join:\", \" "
                                       "}}{% endautoescape %}"
                                    << dict << QStringLiteral("&, < -- &, <")
                                    << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("<a>x</a> <p><b>y</b></p>"));
  dict.insert(QStringLiteral("b"), QVariant::fromValue(markSafe(QStringLiteral(
                                       "<a>x</a> <p><b>y</b></p>"))));

  QTest::newRow("filter-removetags01")
      << "{{ a|removetags:\"a b\" }} {{ b|removetags:\"a b\" }}" << dict
      << QStringLiteral("x &lt;p&gt;y&lt;/p&gt; x <p>y</p>") << NoError;
  QTest::newRow("filter-removetags02")
      << "{% autoescape off %}{{ a|removetags:\"a b\" }} {{ b|removetags:\"a "
         "b\" }}{% endautoescape %}"
      << dict << QStringLiteral("x <p>y</p> x <p>y</p>") << NoError;
  QTest::newRow("filter-striptags01")
      << QStringLiteral("{{ a|striptags }} {{ b|striptags }}") << dict
      << QStringLiteral("x y x y") << NoError;
  QTest::newRow("filter-striptags02")
      << QStringLiteral("{% autoescape off %}{{ a|striptags }} {{ b|striptags }}{% endautoescape %}")
      << dict << QStringLiteral("x y x y") << NoError;
}

void TestFilters::testListFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("a&b")
                                                  << QStringLiteral("x"));
  dict.insert(QStringLiteral("b"),
              QVariantList()
                  << QVariant::fromValue(markSafe(QStringLiteral("a&b")))
                  << QStringLiteral("x"));

  QTest::newRow("filter-first01")
      << QStringLiteral("{{ a|first }} {{ b|first }}") << dict
      << QStringLiteral("a&amp;b a&b") << NoError;
  QTest::newRow("filter-first02")
      << QStringLiteral("{% autoescape off %}{{ a|first }} {{ b|first }}{% endautoescape %}")
      << dict << QStringLiteral("a&b a&b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("x")
                                                  << QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariantList()
                  << QStringLiteral("x")
                  << QVariant::fromValue(markSafe(QStringLiteral("a&b"))));

  QTest::newRow("filter-last01") << QStringLiteral("{{ a|last }} {{ b|last }}")
                                 << dict << QStringLiteral("a&amp;b a&b")
                                 << NoError;
  QTest::newRow("filter-last02")
      << QStringLiteral(
             "{% autoescape off %}{{ a|last }} {{ b|last }}{% endautoescape %}")
      << dict << QStringLiteral("a&b a&b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("a&b")
                                                  << QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariantList()
                  << QVariant::fromValue(markSafe(QStringLiteral("a&b")))
                  << QVariant::fromValue(markSafe(QStringLiteral("a&b"))));
  QTest::newRow("filter-random01")
      << QStringLiteral("{{ a|random }} {{ b|random }}") << dict
      << QStringLiteral("a&amp;b a&b") << NoError;
  QTest::newRow("filter-random02")
      << QStringLiteral("{% autoescape off %}{{ a|random }} {{ b|random }}{% endautoescape %}")
      << dict << QStringLiteral("a&b a&b") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("empty_list"), QVariantList());
  QTest::newRow("filter-random03")
      << QStringLiteral("{{empty_list|random}}")
      << dict << QStringLiteral("") << NoError;
  QTest::newRow("filter-random04")
      << QStringLiteral("{{|random}}")
      << dict << QStringLiteral("") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a&b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a&b"))));

  QTest::newRow("filter-slice01")
      << "{{ a|slice:\"1:3\" }} {{ b|slice:\"1:3\" }}" << dict
      << QStringLiteral("&amp;b &b") << NoError;
  QTest::newRow("filter-slice02")
      << "{% autoescape off %}{{ a|slice:\"1:3\" }} {{ b|slice:\"1:3\" }}{% "
         "endautoescape %}"
      << dict << QStringLiteral("&b &b") << NoError;

  dict.clear();
  QTest::newRow("filter-slice03")
      << "{{xx|slice}}" << dict
      << QStringLiteral("") << NoError;
  QTest::newRow("filter-slice04")
      << "{{|slice}}" << dict
      << QStringLiteral("") << NoError;

  dict.clear();
  QVariantList sublist;
  sublist << QVariant(QStringLiteral("<y"));
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("x>")
                                                  << QVariant(sublist));

  QTest::newRow("filter-unordered_list01")
      << QStringLiteral("{{ a|unordered_list }}") << dict
      << "\t<li>x&gt;\n\t<ul>\n\t\t<li>&lt;y</li>\n\t</ul>\n\t</li>" << NoError;
  QTest::newRow("filter-unordered_list02")
      << QStringLiteral(
             "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}")
      << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>"
      << NoError;

  dict.clear();
  sublist.clear();
  sublist << markSafe(QStringLiteral("<y"));
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("x>")
                                                  << QVariant(sublist));

  QTest::newRow("filter-unordered_list03")
      << QStringLiteral("{{ a|unordered_list }}") << dict
      << "\t<li>x&gt;\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>" << NoError;
  QTest::newRow("filter-unordered_list04")
      << QStringLiteral(
             "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}")
      << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>"
      << NoError;

  dict.clear();
  sublist.clear();
  sublist << QVariant(QStringLiteral("<y"));
  dict.insert(QStringLiteral("a"), QVariantList() << QStringLiteral("x>")
                                                  << QVariant(sublist));

  QTest::newRow("filter-unordered_list05")
      << QStringLiteral(
             "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}")
      << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>"
      << NoError;

  //  length filter.
  dict.clear();
  dict.insert(QStringLiteral("list"), QVariantList() << QStringLiteral("4")
                                                     << QVariant() << true
                                                     << QVariantHash());

  QTest::newRow("length01") << QStringLiteral("{{ list|length }}") << dict
                            << QStringLiteral("4") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("list"), QVariantList());

  QTest::newRow("length02") << QStringLiteral("{{ list|length }}") << dict
                            << QStringLiteral("0") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("string"), QStringLiteral(""));

  QTest::newRow("length03") << QStringLiteral("{{ string|length }}") << dict
                            << QStringLiteral("0") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("string"), QStringLiteral("django"));

  QTest::newRow("length04") << QStringLiteral("{{ string|length }}") << dict
                            << QStringLiteral("6") << NoError;

  //  Invalid uses that should fail silently.

  dict.clear();
  dict.insert(QStringLiteral("int"), 7);

  QTest::newRow("length05") << QStringLiteral("{{ int|length }}") << dict
                            << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("None"), QVariant());

  QTest::newRow("length06") << QStringLiteral("{{ None|length }}") << dict
                            << QString() << NoError;

  //  length_is filter.

  dict.clear();
  dict.insert(QStringLiteral("some_list"), QVariantList() << QStringLiteral("4")
                                                          << QVariant() << true
                                                          << QVariantHash());

  QTest::newRow("length_is01")
      << "{% if some_list|length_is:\"4\" %}Four{% endif %}" << dict
      << QStringLiteral("Four") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("some_list"),
              QVariantList() << QStringLiteral("4") << QVariant() << true
                             << QVariantHash() << 17);

  QTest::newRow("length_is02")
      << "{% if some_list|length_is:\"4\" %}Four{% else %}Not Four{% endif %}"
      << dict << QStringLiteral("Not Four") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("mystring"), QStringLiteral("word"));

  QTest::newRow("length_is03")
      << "{% if mystring|length_is:\"4\" %}Four{% endif %}" << dict
      << QStringLiteral("Four") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("mystring"), QStringLiteral("Python"));

  QTest::newRow("length_is04")
      << "{% if mystring|length_is:\"4\" %}Four{% else %}Not Four{% endif %}"
      << dict << QStringLiteral("Not Four") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("mystring"), QStringLiteral(""));

  QTest::newRow("length_is05")
      << "{% if mystring|length_is:\"4\" %}Four{% else %}Not Four{% endif %}"
      << dict << QStringLiteral("Not Four") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("var"), QStringLiteral("django"));

  QTest::newRow("length_is06")
      << QStringLiteral(
             "{% with var|length as my_length %}{{ my_length }}{% endwith %}")
      << dict << QStringLiteral("6") << NoError;

  //  Boolean return value from length_is should not be coerced to a string

  dict.clear();
  QTest::newRow("length_is07") << "{% if \"X\"|length_is:0 %}Length is 0{% "
                                  "else %}Length not 0{% endif %}"
                               << dict << QStringLiteral("Length not 0")
                               << NoError;
  QTest::newRow("length_is08") << "{% if \"X\"|length_is:1 %}Length is 1{% "
                                  "else %}Length not 1{% endif %}"
                               << dict << QStringLiteral("Length is 1")
                               << NoError;

  //  Invalid uses that should fail silently.

  dict.clear();
  dict.insert(QStringLiteral("var"), QStringLiteral("django"));

  QTest::newRow("length_is09") << "{{ var|length_is:\"fish\" }}" << dict
                               << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("int"), 7);

  QTest::newRow("length_is10") << "{{ int|length_is:\"1\" }}" << dict
                               << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("none"), QVariant());

  QTest::newRow("length_is11") << "{{ none|length_is:\"1\" }}" << dict
                               << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QVariantList()
                                       << QStringLiteral("alpha")
                                       << QStringLiteral("beta & me"));

  QTest::newRow("join01") << "{{ a|join:\", \" }}" << dict
                          << QStringLiteral("alpha, beta &amp; me") << NoError;
  QTest::newRow("join02")
      << "{% autoescape off %}{{ a|join:\", \" }}{% endautoescape %}" << dict
      << QStringLiteral("alpha, beta & me") << NoError;
  QTest::newRow("join03") << "{{ a|join:\" &amp; \" }}" << dict
                          << QStringLiteral("alpha &amp; beta &amp; me")
                          << NoError;
  QTest::newRow("join04")
      << "{% autoescape off %}{{ a|join:\" &amp; \" }}{% endautoescape %}"
      << dict << QStringLiteral("alpha &amp; beta & me") << NoError;

  // Test that joining with unsafe joiners don't result in unsafe strings
  // (#11377)
  dict.insert(QStringLiteral("var"), QStringLiteral(" & "));
  QTest::newRow("join05") << "{{ a|join:var }}" << dict
                          << QStringLiteral("alpha &amp; beta &amp; me")
                          << NoError;
  dict.insert(QStringLiteral("var"), Grantlee::markSafe(QStringLiteral(" & ")));
  QTest::newRow("join06") << "{{ a|join:var }}" << dict
                          << QStringLiteral("alpha & beta &amp; me") << NoError;
  dict.insert(QStringLiteral("a"), QVariantList()
                                       << QStringLiteral("Alpha")
                                       << QStringLiteral("Beta & Me"));
  dict.insert(QStringLiteral("var"), QStringLiteral(" & "));
  QTest::newRow("join07") << "{{ a|join:var|lower }}" << dict
                          << QStringLiteral("alpha &amp; beta &amp; me")
                          << NoError;

  dict.insert(QStringLiteral("a"), QVariantList()
                                       << QStringLiteral("Alpha")
                                       << QStringLiteral("Beta & Me"));
  dict.insert(QStringLiteral("var"), Grantlee::markSafe(QStringLiteral(" & ")));
  QTest::newRow("join08") << "{{ a|join:var|lower }}" << dict
                          << QStringLiteral("alpha & beta &amp; me") << NoError;

  // arguments to filters are intended to be used unescaped.
  //   dict.clear();
  //   dict.insert( QStringLiteral("a"), QVariantList() <<
  //   QString::fromLatin1( "alpha" ) << QString::fromLatin1( "beta & me" ) );
  //   dict.insert( QStringLiteral("var"), QStringLiteral(" & ") );
  //
  //   QTest::newRow( "join05" ) << QString::fromLatin1( "{{ a|join:var }}" )
  //   <<
  //   dict << QString::fromLatin1( "alpha &amp; beta &amp; me" ) << NoError;
  //
  //   dict.clear();
  //   dict.insert( QStringLiteral("a"), QVariantList() <<
  //   QString::fromLatin1( "alpha" ) << QString::fromLatin1( "beta & me" ) );
  //   dict.insert( QStringLiteral("var"), QVariant::fromValue( markSafe(
  //   QString::fromLatin1( " & " ) ) ) );
  //
  //   QTest::newRow( "join06" ) << QString::fromLatin1( "{{ a|join:var }}" )
  //   <<
  //   dict << QString::fromLatin1( "alpha & beta &amp; me" ) << NoError;
  //
  //   dict.clear();
  //   dict.insert( QStringLiteral("a"), QVariantList() <<
  //   QString::fromLatin1( "Alpha" ) << QString::fromLatin1( "Beta & me" ) );
  //   dict.insert( QStringLiteral("var"), QStringLiteral(" & ") );
  //
  //   QTest::newRow( "join07" ) << QString::fromLatin1( "{{ a|join:var|lower
  //   }}" ) << dict << QString::fromLatin1( "alpha &amp; beta &amp; me" ) <<
  //   NoError;
  //
  //   dict.clear();
  //   dict.insert( QStringLiteral("a"), QVariantList() <<
  //   QString::fromLatin1( "Alpha" ) << QString::fromLatin1( "Beta & me" ) );
  //   dict.insert( QStringLiteral("var"), QVariant::fromValue( markSafe(
  //   QString::fromLatin1( " & " ) ) ) );
  //
  //   QTest::newRow( "join08" ) << QString::fromLatin1( "{{ a|join:var|lower
  //   }}" ) << dict << QString::fromLatin1( "alpha & beta &amp; me" ) <<
  //   NoError;

  dict.clear();

  QVariantList mapList;
  const auto cities = QStringList()
                      << QStringLiteral("London") << QStringLiteral("Berlin")
                      << QStringLiteral("Paris") << QStringLiteral("Dublin");
  Q_FOREACH (const QString &city, cities) {
    QVariantHash map;
    map.insert(QStringLiteral("city"), city);
    mapList << map;
  }

  dict.insert(QStringLiteral("mapList"), mapList);

  QTest::newRow("dictsort01")
      << "{% with mapList|dictsort:'city' as result %}{% for item in result "
         "%}{{ item.city }},{% endfor %}{% endwith %}"
      << dict << "Berlin,Dublin,London,Paris," << NoError;

  {
    // Test duplication works
    QVariantHash map;
    map.insert(QStringLiteral("city"), QStringLiteral("Berlin"));
    mapList << map;
  }
  dict.insert(QStringLiteral("mapList"), mapList);

  QTest::newRow("dictsort02")
      << "{% with mapList|dictsort:'city' as result %}{% for item in result "
         "%}{{ item.city }},{% endfor %}{% endwith %}"
      << dict << "Berlin,Berlin,Dublin,London,Paris," << NoError;

  dict.clear();

  QVariantList listList;

  const auto countries
      = QStringList() << QStringLiteral("England") << QStringLiteral("Germany")
                      << QStringLiteral("France") << QStringLiteral("Ireland");

  const auto languages
      = QStringList() << QStringLiteral("English") << QStringLiteral("German")
                      << QStringLiteral("French") << QStringLiteral("Irish");

  for (auto i = 0; i < cities.size(); ++i) {
    listList << QVariant(QVariantList() << cities.at(i) << countries.at(i)
                                        << languages.at(i));
  }

  dict.insert(QStringLiteral("listList"), listList);

  QTest::newRow("dictsort03")
      << "{% with listList|dictsort:'0' as result %}{% for item in result "
         "%}{{ "
         "item.0 }};{{ item.1 }};{{ item.2 }},{% endfor %}{% endwith %}"
      << dict << "Berlin;Germany;German,Dublin;Ireland;Irish,London;England;"
                 "English,Paris;France;French,"
      << NoError;

  QTest::newRow("dictsort04")
      << "{% with listList|dictsort:'1' as result %}{% for item in result "
         "%}{{ "
         "item.0 }};{{ item.1 }};{{ item.2 }},{% endfor %}{% endwith %}"
      << dict << "London;England;English,Paris;France;French,Berlin;Germany;"
                 "German,Dublin;Ireland;Irish,"
      << NoError;
}

void TestFilters::testLogicFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  //  Literal string arguments to the default filter are always treated as
  //  safe strings, regardless of the auto-escaping state.

  //  Note: we have to use {"a": ""} here, otherwise the invalid template
  //  variable string interferes with the test result.

  dict.insert(QStringLiteral("a"), QStringLiteral(""));

  QTest::newRow("filter-default01") << "{{ a|default:\"x<\" }}" << dict
                                    << QStringLiteral("x<") << NoError;
  QTest::newRow("filter-default02")
      << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict
      << QStringLiteral("x<") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"),
              QVariant::fromValue(markSafe(QStringLiteral("x>"))));

  QTest::newRow("filter-default03") << "{{ a|default:\"x<\" }}" << dict
                                    << QStringLiteral("x>") << NoError;
  QTest::newRow("filter-default04")
      << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict
      << QStringLiteral("x>") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QVariant());

  QTest::newRow("filter-default_if_none01") << "{{ a|default:\"x<\" }}" << dict
                                            << QStringLiteral("x<") << NoError;
  QTest::newRow("filter-default_if_none02")
      << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict
      << QStringLiteral("x<") << NoError;
}

void TestFilters::testMiscFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  //
  //   //  {"a": "<1-800-call-me>", "b": mark_safe("<1-800-call-me>")
  //   QTest::newRow( "filter-phone2numeric01") << QString::fromLatin1( "{{
  //   a|phone2numeric }} {{ b|phone2numeric }}" ) << dict <<
  //   QString::fromLatin1( "&lt;1-800-2255-63&gt; <1-800-2255-63>" ) <<
  //   NoError;
  //
  //   //  {"a": "<1-800-call-me>", "b": mark_safe("<1-800-call-me>")
  //   QTest::newRow( "filter-phone2numeric02") << QString::fromLatin1( "{%
  //   autoescape off %}{{ a|phone2numeric }} {{ b|phone2numeric }}{%
  //   endautoescape %}" ) << dict << QString::fromLatin1( "<1-800-2255-63>
  //   <1-800-2255-63>" ) << NoError;
  //
  //  Chaining a bunch of safeness-preserving filters should not alter
  //  the safe status either way.

  dict.insert(QStringLiteral("a"), QStringLiteral("a < b"));
  dict.insert(QStringLiteral("b"),
              QVariant::fromValue(markSafe(QStringLiteral("a < b"))));

  QTest::newRow("chaining01")
      << "{{ a|capfirst|center:\"7\" }}.{{ b|capfirst|center:\"7\" }}" << dict
      << QStringLiteral(" A &lt; b . A < b ") << NoError;
  QTest::newRow("chaining02")
      << "{% autoescape off %}{{ a|capfirst|center:\"7\" }}.{{ "
         "b|capfirst|center:\"7\" }}{% endautoescape %}"
      << dict << QStringLiteral(" A < b . A < b ") << NoError;

  //  Using a filter that forces a string back to unsafe:

  QTest::newRow("chaining03")
      << "{{ a|cut:\"b\"|capfirst }}.{{ b|cut:\"b\"|capfirst }}" << dict
      << QStringLiteral("A &lt; .A < ") << NoError;
  QTest::newRow("chaining04")
      << "{% autoescape off %}{{ a|cut:\"b\"|capfirst }}.{{ "
         "b|cut:\"b\"|capfirst }}{% endautoescape %}"
      << dict << QStringLiteral("A < .A < ") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("a"), QStringLiteral("a < b"));

  //  Using a filter that forces safeness does not lead to double-escaping

  QTest::newRow("chaining05") << QStringLiteral("{{ a|escape|capfirst }}")
                              << dict << QStringLiteral("A &lt; b") << NoError;
  QTest::newRow("chaining06")
      << QStringLiteral(
             "{% autoescape off %}{{ a|escape|capfirst }}{% endautoescape %}")
      << dict << QStringLiteral("A &lt; b") << NoError;

  //  Force to safe, then back (also showing why using force_escape too
  //  early in a chain can lead to unexpected results).

  QTest::newRow("chaining07") << "{{ a|force_escape|cut:\";\" }}" << dict
                              << QStringLiteral("a &amp;lt b") << NoError;
  QTest::newRow("chaining08")
      << "{% autoescape off %}{{ a|force_escape|cut:\";\" }}{% endautoescape "
         "%}"
      << dict << QStringLiteral("a &lt b") << NoError;
  QTest::newRow("chaining09") << "{{ a|cut:\";\"|force_escape }}" << dict
                              << QStringLiteral("a &lt; b") << NoError;
  QTest::newRow("chaining10")
      << "{% autoescape off %}{{ a|cut:\";\"|force_escape }}{% endautoescape "
         "%}"
      << dict << QStringLiteral("a &lt; b") << NoError;
  QTest::newRow("chaining11") << "{{ a|cut:\"b\"|safe }}" << dict
                              << QStringLiteral("a < ") << NoError;
  QTest::newRow("chaining12")
      << "{% autoescape off %}{{ a|cut:\"b\"|safe }}{% endautoescape %}" << dict
      << QStringLiteral("a < ") << NoError;
  QTest::newRow("chaining13") << QStringLiteral("{{ a|safe|force_escape }}")
                              << dict << QStringLiteral("a &lt; b") << NoError;
  QTest::newRow("chaining14")
      << QStringLiteral(
             "{% autoescape off %}{{ a|safe|force_escape }}{% endautoescape %}")
      << dict << QStringLiteral("a &lt; b") << NoError;

  //   //  Filters decorated with stringfilter still respect is_safe.
  //
  //   //  {"unsafe": UnsafeClass()
  //   QTest::newRow( "autoescape-stringfilter01") << QString::fromLatin1( "{{
  //   unsafe|capfirst }}" ) << dict << QString::fromLatin1( "You &amp; me" )
  //   <<
  //   NoError;
  //
  //   //  {"unsafe": UnsafeClass()
  //   QTest::newRow( "autoescape-stringfilter02") << QString::fromLatin1( "{%
  //   autoescape off %}{{ unsafe|capfirst }}{% endautoescape %}" ) << dict <<
  //   QString::fromLatin1( "You & me" ) << NoError;
  //
  //   //  {"safe": SafeClass()
  //   QTest::newRow( "autoescape-stringfilter03") << QString::fromLatin1( "{{
  //   safe|capfirst }}" ) << dict << QString::fromLatin1( "You &gt; me" ) <<
  //   NoError;
  //
  //   //  {"safe": SafeClass()
  //   QTest::newRow( "autoescape-stringfilter04") << QString::fromLatin1( "{%
  //   autoescape off %}{{ safe|capfirst }}{% endautoescape %}" ) << dict <<
  //   QString::fromLatin1( "You &gt; me" ) << NoError;
  //
}

void TestFilters::testIntegerFilters_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert(QStringLiteral("i"), 2000);

  QTest::newRow("add01") << QStringLiteral("{{ i|add:5 }}") << dict
                         << QStringLiteral("2005") << NoError;
  QTest::newRow("add02") << QStringLiteral("{{ i|add:\"napis\" }}") << dict
                         << QStringLiteral("2000") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("i"), QStringLiteral("not_an_int"));

  QTest::newRow("add03") << QStringLiteral("{{ i|add:16 }}") << dict
                         << QStringLiteral("not_an_int") << NoError;
  QTest::newRow("add04") << QStringLiteral("{{ i|add:\"16\" }}") << dict
                         << QStringLiteral("not_an_int16") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("l1"), QVariantList() << 1 << 2);
  dict.insert(QStringLiteral("l2"), QVariantList() << 3 << 4);

  QTest::newRow("add05") << QStringLiteral("{{ l1|add:l2 }}") << dict
                         << QStringLiteral("[1, 2, 3, 4]") << NoError;
  // QTest::newRow( "add06" ) << QString::fromLatin1( "{{ t1|add:t2 }}" ) <<
  // dict << QString::fromLatin1( "2005" ) << NoError;

  // QTest::newRow( "add07" ) << QString::fromLatin1( "{{ d|add:t }}" ) <<
  // dict
  // << QString::fromLatin1( "2005" ) << NoError;

  QTest::newRow("add08") << QStringLiteral("{{ 1|add:2 }}") << dict
                         << QStringLiteral("3") << NoError;

  QTest::newRow("filter-getdigit01") << QStringLiteral("{{ 123|get_digit:1 }}")
                                     << dict << QStringLiteral("3") << NoError;
  QTest::newRow("filter-getdigit02") << QStringLiteral("{{ 123|get_digit:2 }}")
                                     << dict << QStringLiteral("2") << NoError;
  QTest::newRow("filter-getdigit03") << QStringLiteral("{{ 123|get_digit:3 }}")
                                     << dict << QStringLiteral("1") << NoError;
  QTest::newRow("filter-getdigit04") << QStringLiteral("{{ 123|get_digit:4 }}")
                                     << dict << QStringLiteral("123")
                                     << NoError;
  QTest::newRow("filter-getdigit05") << QStringLiteral("{{ 123|get_digit:0 }}")
                                     << dict << QStringLiteral("123")
                                     << NoError;
  QTest::newRow("filter-getdigit06") << QStringLiteral("{{ 123|get_digit:-1 }}")
                                     << dict << QStringLiteral("123")
                                     << NoError;

  dict.clear();
  QTest::newRow("filter-getdigit07") << QStringLiteral("{{xx|get_digit}}")
                                     << dict << QStringLiteral("") << NoError;
  QTest::newRow("filter-getdigit08") << QStringLiteral("{{|get_digit}}")
                                     << dict << QStringLiteral("") << NoError;
  QTest::newRow("filter-getdigit09") << QStringLiteral("{{xx|get_digit:\"\xc2\xb2\"}}")
                                     << dict << QStringLiteral("") << NoError;
}

QTEST_MAIN(TestFilters)
#include "testfilters.moc"

#endif
