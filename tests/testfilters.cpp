/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef FILTERSTEST_H
#define FILTERSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "grantlee.h"
#include <util_p.h>


typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestFilters : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void cleanupTestCase();

  void testDateBasedFilters_data();
  void testDateBasedFilters() {
    doTest();
  }

  void testStringFilters_data();
  void testStringFilters() {
    doTest();
  }

  void testListFilters_data();
  void testListFilters() {
    doTest();
  }

  void testLogicFilters_data();
  void testLogicFilters() {
    doTest();
  }

  void testMiscFilters_data();
  void testMiscFilters() {
    doTest();
  }

  void testIntegerFilters_data();
  void testIntegerFilters() {
    doTest();
  }


private:

  void doTest();

  InMemoryTemplateLoader::Ptr loader;
  Engine *m_engine;

};

void TestFilters::initTestCase()
{
  m_engine = Engine::instance();

  loader = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );
  m_engine->addTemplateLoader( loader );

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginDirs( QStringList() << appDirPath + "/grantlee_loadertags/"
                           << appDirPath + "/grantlee_defaulttags/"
                           << appDirPath + "/grantlee_scriptabletags/"
                           << appDirPath + "/grantlee_defaultfilters/"
                           << appDirPath + "/tests/" // For testtags.qs
                         );
}

void TestFilters::cleanupTestCase()
{
  delete m_engine;
}

void TestFilters::doTest()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );
  QFETCH( QString, output );
  QFETCH( Grantlee::Error, errorNumber );

  Template t = Engine::instance()->newTemplate( input, QTest::currentDataTag() );

  Context context( dict );

  QString result = t->render( &context );

  if ( t->error() != NoError ) {
    QCOMPARE( t->error(), errorNumber );
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE( NoError, errorNumber );

  QCOMPARE( t->error(), NoError );

  QCOMPARE( result, output );
}

void TestFilters::testDateBasedFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  dict.insert( "a", QDateTime::currentDateTime().addSecs( -70 ) );

  QTest::newRow( "filter-timesince01" ) << "{{ a|timesince }}" << dict << "1 minute" << NoError;

  dict.clear();

  dict.insert( "a", QDateTime::currentDateTime().addDays( -1 ).addSecs( -60 ) );

  QTest::newRow( "filter-timesince02" ) << "{{ a|timesince }}" << dict << "1 day" << NoError;

  dict.clear();

  dict.insert( "a", QDateTime::currentDateTime().addSecs( -1 * 60 * 60 ).addSecs( -1 * 25 * 60 ).addSecs( -1 * 10 ) );
  QTest::newRow( "filter-timesince03" ) << "{{ a|timesince }}" << dict << "1 hour, 25 minutes" << NoError;

  dict.clear();

  //  Compare to a given parameter

  dict.insert( "a", QDateTime::currentDateTime().addDays( -2 ) );
  dict.insert( "b", QDateTime::currentDateTime().addDays( -1 ) );

  QTest::newRow( "filter-timesince04" ) << "{{ a|timesince:b }}" << dict << "1 day" << NoError;

  dict.clear();

  dict.insert( "a", QDateTime::currentDateTime().addDays( -2 ).addSecs( -60 ) );
  dict.insert( "b", QDateTime::currentDateTime().addDays( -2 ) );

  QTest::newRow( "filter-timesince05" ) << "{{ a|timesince:b }}" << dict << "1 minute" << NoError;

  dict.clear();

  //  Check that timezone is respected

  //  {"a":now_tz - timedelta(hours=8), "b":now_tz
//   QTest::newRow( "filter-timesince06" ) << "{{ a|timesince:b }}" << dict << "8 hours" << NoError;

  dict.insert( "earlier", QDateTime::currentDateTime().addDays( -7 ) );
  QTest::newRow( "filter-timesince07" ) << "{{ earlier|timesince }}" << dict << "1 week" << NoError;

  dict.clear();

  dict.insert( "now", QDateTime::currentDateTime() );
  dict.insert( "earlier", QDateTime::currentDateTime().addDays( -7 ) );

  QTest::newRow( "filter-timesince08" ) << "{{ earlier|timesince:now }}" << dict << "1 week" << NoError;

  dict.clear();

  dict.insert( "later", QDateTime::currentDateTime().addDays( 7 ) );

  QTest::newRow( "filter-timesince09" ) << "{{ later|timesince }}" << dict << "0 minutes" << NoError;

  dict.clear();

  dict.insert( "now", QDateTime::currentDateTime() );
  dict.insert( "later", QDateTime::currentDateTime().addDays( 7 ) );

  QTest::newRow( "filter-timesince10" ) << "{{ later|timesince:now }}" << dict << "0 minutes" << NoError;

  //  Ensures that differing timezones are calculated correctly

  //  {"a": now
//   QTest::newRow( "filter-timesince11" ) << "{{ a|timesince }}" << dict << "0 minutes" << NoError;

  //  {"a": now_tz
//   QTest::newRow( "filter-timesince12" ) << "{{ a|timesince }}" << dict << "0 minutes" << NoError;

  //  {"a": now_tz_i
//   QTest::newRow( "filter-timesince13" ) << "{{ a|timesince }}" << dict << "0 minutes" << NoError;

  //  {"a": now_tz, "b": now_tz_i
//   QTest::newRow( "filter-timesince14" ) << "{{ a|timesince:b }}" << dict << "0 minutes" << NoError;

  //  {"a": now, "b": now_tz_i
//   QTest::newRow( "filter-timesince15" ) << "{{ a|timesince:b }}" << dict << "" << NoError;

  //  {"a": now_tz_i, "b": now
//   QTest::newRow( "filter-timesince16" ) << "{{ a|timesince:b }}" << dict << "" << NoError;

  dict.clear();

  dict.insert( "a", QDateTime::currentDateTime() );
  dict.insert( "b", QDateTime::currentDateTime() );

  QTest::newRow( "filter-timesince17" ) << "{{ a|timesince:b }}" << dict << "0 minutes" << NoError;

  dict.clear();

  dict.insert( "a", QDateTime::currentDateTime() );
  dict.insert( "b", QDateTime::currentDateTime().addDays( 1 ) );

  QTest::newRow( "filter-timesince18" ) << "{{ a|timesince:b }}" << dict << "1 day" << NoError;

  //  Default compare with datetime.now()

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime().addSecs( 130 ) );

  QTest::newRow( "filter-timeuntil01" ) << "{{ a|timeuntil }}" << dict << "2 minutes" << NoError;

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime().addDays( 1 ).addSecs( 10 ) );

  QTest::newRow( "filter-timeuntil02" ) << "{{ a|timeuntil }}" << dict << "1 day" << NoError;

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime().addSecs( 60 * 60 * 8 ).addSecs( 610 ) );

  QTest::newRow( "filter-timeuntil03" ) << "{{ a|timeuntil }}" << dict << "8 hours, 10 minutes" << NoError;

  //  Compare to a given parameter

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime().addDays( -1 ) );
  dict.insert( "b", QDateTime::currentDateTime().addDays( -2 ) );

  QTest::newRow( "filter-timeuntil04" ) << "{{ a|timeuntil:b }}" << dict << "1 day" << NoError;

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime().addDays( -1 ) );
  dict.insert( "b", QDateTime::currentDateTime().addDays( -1 ).addSecs( -60 ) );

  QTest::newRow( "filter-timeuntil05" ) << "{{ a|timeuntil:b }}" << dict << "1 minute" << NoError;

  dict.clear();
  dict.insert( "earlier", QDateTime::currentDateTime().addDays( -7 ) );

  QTest::newRow( "filter-timeuntil06" ) << "{{ earlier|timeuntil }}" << dict << "0 minutes" << NoError;

  dict.clear();
  dict.insert( "now", QDateTime::currentDateTime() );
  dict.insert( "earlier", QDateTime::currentDateTime().addDays( -7 ) );

  QTest::newRow( "filter-timeuntil07" ) << "{{ earlier|timeuntil:now }}" << dict << "0 minutes" << NoError;

  dict.clear();
  dict.insert( "later", QDateTime::currentDateTime().addDays( 7 ) );

  QTest::newRow( "filter-timeuntil08" ) << "{{ later|timeuntil }}" << dict << "1 week" << NoError;

  dict.clear();
  dict.insert( "now", QDateTime::currentDateTime() );
  dict.insert( "later", QDateTime::currentDateTime().addDays( 7 ) );

  QTest::newRow( "filter-timeuntil09" ) << "{{ later|timeuntil:now }}" << dict << "1 week" << NoError;

  //  Ensures that differing timezones are calculated correctly
//
//   //  {"a": now_tz_i
//   QTest::newRow( "filter-timeuntil10" ) << "{{ a|timeuntil }}" << dict << "0 minutes" << NoError;
//
//   //  {"a": now_tz_i, "b": now_tz
//   QTest::newRow( "filter-timeuntil11" ) << "{{ a|timeuntil:b }}" << dict << "0 minutes" << NoError;

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime() );
  dict.insert( "b", QDateTime::currentDateTime() );
  QTest::newRow( "filter-timeuntil12" ) << "{{ a|timeuntil:b }}" << dict << "0 minutes" << NoError;

  dict.clear();
  dict.insert( "a", QDateTime::currentDateTime() );
  dict.insert( "b", QDateTime::currentDateTime().addDays( -1 ) );

  QTest::newRow( "filter-timeuntil13" ) << "{{ a|timeuntil:b }}" << dict << "1 day" << NoError;

  QDateTime d( QDate( 2008, 1, 1 ) );

  dict.clear();
  dict.insert( "d", d );

  QTest::newRow( "date01" ) << "{{ d|date:\"MM\" }}" << dict << "01" << NoError;
  QTest::newRow( "date02" ) << "{{ d|date }}" << dict << "Jan. 1, 2008" << NoError;

  dict.clear();
  dict.insert( "d", "fail_string" );
  QTest::newRow( "date03" ) << "{{ d|date:\"MM\" }}" << dict << "" << NoError;

}

void TestFilters::testStringFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  dict.clear();
  dict.insert( "a", QString( "<a>\'" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "<a>\'" ) ) ) );

  QTest::newRow( "filter-addslash01" ) << "{% autoescape off %}{{ a|addslashes }} {{ b|addslashes }}{% endautoescape %}" << dict << "<a>\\\' <a>\\\'" << NoError;

  dict.clear();
  dict.insert( "a", QString( "<a>\'" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "<a>\'" ) ) ) );

  QTest::newRow( "filter-addslash02" ) << "{{ a|addslashes }} {{ b|addslashes }}" << dict << "&lt;a&gt;\\&#39; <a>\\\'" << NoError;

  dict.clear();
  dict.insert( "a", QString( "fred>" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "fred&gt;" ) ) ) );

  QTest::newRow( "filter-capfirst01" ) << "{% autoescape off %}{{ a|capfirst }} {{ b|capfirst }}{% endautoescape %}" << dict << "Fred> Fred&gt;" << NoError;

  dict.clear();
  dict.insert( "a", QString( "fred>" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "fred&gt;" ) ) ) );

  QTest::newRow( "filter-capfirst02" ) << "{{ a|capfirst }} {{ b|capfirst }}" << dict << "Fred&gt; Fred&gt;" << NoError;

  //  Note that applying fix_ampsersands in autoescape mode leads to
  //  double escaping.

  dict.clear();
  dict.insert( "a", QString( "a&b" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );

  QTest::newRow( "filter-fix_ampersands01" ) << "{% autoescape off %}{{ a|fix_ampersands }} {{ b|fix_ampersands }}{% endautoescape %}" << dict << "a&amp;b a&amp;b" << NoError;

  dict.clear();
  dict.insert( "a", QString( "a&b" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );

  QTest::newRow( "filter-fix_ampersands02" ) << "{{ a|fix_ampersands }} {{ b|fix_ampersands }}" << dict << "a&amp;amp;b a&amp;b" << NoError;

  dict.clear();
  dict.insert( "a", QString( "1.42" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "1.42" ) ) ) );

  QTest::newRow( "filter-floatformat01" ) << "{% autoescape off %}{{ a|floatformat }} {{ b|floatformat }}{% endautoescape %}" << dict << "1.4 1.4" << NoError;

  dict.clear();
  dict.insert( "a", QString( "1.42" ) );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "1.42" ) ) ) );

  QTest::newRow( "filter-floatformat02" ) << "{{ a|floatformat }} {{ b|floatformat }}" << dict << "1.4 1.4" << NoError;

  //  The contents of "linenumbers" is escaped according to the current
  //  autoescape setting.

  dict.clear();
  dict.insert( "a", "one\n<two>\nthree" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "one\n&lt;two&gt;\nthree" ) ) ) );

  QTest::newRow( "filter-linenumbers01" ) << "{{ a|linenumbers }} {{ b|linenumbers }}" << dict << "1. one\n2. &lt;two&gt;\n3. three 1. one\n2. &lt;two&gt;\n3. three" << NoError;

  dict.clear();
  dict.insert( "a", "one\n<two>\nthree" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "one\n&lt;two&gt;\nthree" ) ) ) );
  QTest::newRow( "filter-linenumbers02" ) << "{% autoescape off %}{{ a|linenumbers }} {{ b|linenumbers }}{% endautoescape %}" << dict << "1. one\n2. <two>\n3. three 1. one\n2. &lt;two&gt;\n3. three" << NoError;

  dict.clear();
  dict.insert( "a", "Apple & banana" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "Apple &amp; banana" ) ) ) );

  QTest::newRow( "filter-lower01" ) << "{% autoescape off %}{{ a|lower }} {{ b|lower }}{% endautoescape %}" << dict << "apple & banana apple &amp; banana" << NoError;

  dict.clear();
  dict.insert( "a", "Apple & banana" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "Apple &amp; banana" ) ) ) );

  QTest::newRow( "filter-lower02" ) << "{{ a|lower }} {{ b|lower }}" << dict << "apple &amp; banana apple &amp; banana" << NoError;

  //  The make_list filter can destroy existing escaping, so the results are
  //  escaped.

//
//   //  {"a": mark_safe("&")
//   QTest::newRow( "filter-make_list01") << "{% autoescape off %}{{ a|make_list }}{% endautoescape %}" << dict << "[u\'&\']" << NoError;
//
//   //  {"a": mark_safe("&")
//   QTest::newRow( "filter-make_list02") << "{{ a|make_list }}" << dict << "[u&#39;&amp;&#39;]" << NoError;
//
//   //  {"a": mark_safe("&")
//   QTest::newRow( "filter-make_list03") << "{% autoescape off %}{{ a|make_list|stringformat:"s"|safe }}{% endautoescape %}" << dict << "[u\'&\']" << NoError;
//
//   //  {"a": mark_safe("&")
//   QTest::newRow( "filter-make_list04") << "{{ a|make_list|stringformat:\"s\"|safe }}" << dict << "[u\'&\']" << NoError;
//
//   //  Running slugify on a pre-escaped string leads to odd behaviour,
//
//   //  but the result is still safe.
//
//   //  {"a": "a & b", "b": mark_safe("a &amp; b")
//   QTest::newRow( "filter-slugify01") << "{% autoescape off %}{{ a|slugify }} {{ b|slugify }}{% endautoescape %}" << dict << "a-b a-amp-b" << NoError;
//
//   //  {"a": "a & b", "b": mark_safe("a &amp; b")
//   QTest::newRow( "filter-slugify02") << "{{ a|slugify }} {{ b|slugify }}" << dict << "a-b a-amp-b" << NoError;
//
//   //  {"a": "testing\r\njavascript \'string\" <b>escaping</b>"
//   QTest::newRow( "escapejs01") << "{{ a|escapejs }}" << dict << "testing\\x0D\\x0Ajavascript \\x27string\\x22 \\x3Cb\\x3Eescaping\\x3C/b\\x3E" << NoError;
//
//   //  {"a": "testing\r\njavascript \\\"string\" <b>escaping</b>"
//   QTest::newRow( "escapejs02") << "{% autoescape off %}{{ a|escapejs }}{% endautoescape %}" << dict << "testing\\x0D\\x0Ajavascript \\x27string\\x22 \\x3Cb\\x3Eescaping\\x3C/b\\x3E" << NoError;
//


  //  Notice that escaping is applied *after* any filters, so the string
  //  formatting here only needs to deal with pre-escaped characters.

  dict.clear();
  dict.insert( "a", "a<b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a<b" ) ) ) );

  QTest::newRow( "filter-stringformat01" ) << "{% autoescape off %}.{{ a|stringformat:\"%1\" }}. .{{ b|stringformat:\"%2\" }}.{% endautoescape %}" << dict << ".a<b. .a<b." << NoError;
  QTest::newRow( "filter-stringformat02" ) << ".{{ a|stringformat:\"%1\" }}. .{{ b|stringformat:\"%2\" }}." << dict << ".a&lt;b. .a<b." << NoError;


  //  XXX No test for "title" filter; needs an actual object.


  dict.clear();
  dict.insert( "a", "alpha & bravo" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "alpha &amp; bravo" ) ) ) );

  QTest::newRow( "filter-truncatewords01" ) << "{% autoescape off %}{{ a|truncatewords:\"2\" }} {{ b|truncatewords:\"2\"}}{% endautoescape %}" << dict << "alpha & ... alpha &amp; ..." << NoError;

  QTest::newRow( "filter-truncatewords02" ) << "{{ a|truncatewords:\"2\" }} {{ b|truncatewords:\"2\"}}" << dict << "alpha &amp; ... alpha &amp; ..." << NoError;

  //  The "upper" filter messes up entities (which are case-sensitive),
  //  so it's not safe for non-escaping purposes.

  dict.clear();
  dict.insert( "a", "a & b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a &amp; b" ) ) ) );

  QTest::newRow( "filter-upper01" ) << "{% autoescape off %}{{ a|upper }} {{ b|upper }}{% endautoescape %}" << dict << "A & B A &AMP; B" << NoError;
  QTest::newRow( "filter-upper02" ) << "{{ a|upper }} {{ b|upper }}" << dict << "A &amp; B A &amp;AMP; B" << NoError;

//   //  {"a": "http://example.com/?x=&y=", "b": mark_safe("http://example.com?x=&amp;y=")
//   QTest::newRow( "filter-urlize01") << "{% autoescape off %}{{ a|urlize }} {{ b|urlize }}{% endautoescape %}" << dict << "<a href=\"http://example.com/?x=&y=\" rel=\"nofollow\">http://example.com/?x=&y=</a> <a href=\"http://example.com?x=&amp;y=\" rel=\"nofollow\">http://example.com?x=&amp;y=</a>" << NoError;
//
//   //  {"a": "http://example.com/?x=&y=", "b": mark_safe("http://example.com?x=&amp;y=")
//   QTest::newRow( "filter-urlize02") << "{{ a|urlize }} {{ b|urlize }}" << dict << "<a href=\"http://example.com/?x=&amp;y=\" rel=\"nofollow\">http://example.com/?x=&amp;y=</a> <a href=\"http://example.com?x=&amp;y=\" rel=\"nofollow\">http://example.com?x=&amp;y=</a>" << NoError;
//
//   //  {"a": mark_safe("a &amp; b")
//   QTest::newRow( "filter-urlize03") << "{% autoescape off %}{{ a|urlize }}{% endautoescape %}" << dict << "a &amp; b" << NoError;
//
//   //  {"a": mark_safe("a &amp; b")
//   QTest::newRow( "filter-urlize04") << "{{ a|urlize }}" << dict << "a &amp; b" << NoError;
//
//   //  This will lead to a nonsense result, but at least it won't be
//
//   //  exploitable for XSS purposes when auto-escaping is on.
//
//   //  {"a": "<script>alert(\"foo\")</script>"
//   QTest::newRow( "filter-urlize05") << "{% autoescape off %}{{ a|urlize }}{% endautoescape %}" << dict << "<script>alert(\"foo\")</script>" << NoError;
//
//   //  {"a": "<script>alert(\"foo\")</script>"
//   QTest::newRow( "filter-urlize06") << "{{ a|urlize }}" << dict << "&lt;script&gt;alert(&#39;foo&#39;)&lt;/script&gt;" << NoError;
//
//   //  mailto: testing for urlize
//
//   //  {"a": "Email me at me@example.com"
//   QTest::newRow( "filter-urlize07") << "{{ a|urlize }}" << dict << "Email me at <a href=\"mailto:me@example.com\">me@example.com</a>" << NoError;
//
//   //  {"a": "Email me at <me@example.com>"
//   QTest::newRow( "filter-urlize08") << "{{ a|urlize }}" << dict << "Email me at &lt;<a href=\"mailto:me@example.com\">me@example.com</a>&gt;" << NoError;
//
//   //  {"a": "\"Unsafe\" http://example.com/x=&y=", "b": mark_safe("&quot;Safe&quot; http://example.com?x=&amp;y=")
//   QTest::newRow( "filter-urlizetrunc01") << "{% autoescape off %}{{ a|urlizetrunc:\"8\" }} {{ b|urlizetrunc:\"8\" }}{% endautoescape %}" << dict << "\"Unsafe\" <a href=\"http://example.com/x=&y=\" rel=\"nofollow\">http:...</a> &quot;Safe&quot; <a href=\"http://example.com?x=&amp;y=\" rel=\"nofollow\">http:...</a>" << NoError;
//
//   //  {"a": "\"Unsafe\" http://example.com/x=&y=", "b": mark_safe("&quot;Safe&quot; http://example.com?x=&amp;y=")
//   QTest::newRow( "filter-urlizetrunc02") << "{{ a|urlizetrunc:\"8\" }} {{ b|urlizetrunc:\"8\" }}" << dict << "&quot;Unsafe&quot; <a href=\"http://example.com/x=&amp;y=\" rel=\"nofollow\">http:...</a> &quot;Safe&quot; <a href=\"http://example.com?x=&amp;y=\" rel=\"nofollow\">http:...</a>" << NoError;

//   //  Ensure iriencode keeps safe strings:
//
//   //  {"url": "?test=1&me=2"
//   QTest::newRow( "filter-iriencode01") << "{{ url|iriencode }}" << dict << "?test=1&amp;me=2" << NoError;
//
//   //  {"url": "?test=1&me=2"
//   QTest::newRow( "filter-iriencode02") << "{% autoescape off %}{{ url|iriencode }}{% endautoescape %}" << dict << "?test=1&me=2" << NoError;
//
//   //  {"url": mark_safe("?test=1&me=2")
//   QTest::newRow( "filter-iriencode03") << "{{ url|iriencode }}" << dict << "?test=1&me=2" << NoError;
//
//   //  {"url": mark_safe("?test=1&me=2")
//   QTest::newRow( "filter-iriencode04") << "{% autoescape off %}{{ url|iriencode }}{% endautoescape %}" << dict << "?test=1&me=2" << NoError;
//
  dict.clear();
  dict.insert( "a", "a & b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a &amp; b" ) ) ) );

  QTest::newRow( "filter-wordcount01" ) << "{% autoescape off %}{{ a|wordcount }} {{ b|wordcount }}{% endautoescape %}" << dict << "3 3" << NoError;

  QTest::newRow( "filter-wordcount02" ) << "{{ a|wordcount }} {{ b|wordcount }}" << dict << "3 3" << NoError;

  dict.clear();
  dict.insert( "a", "a & b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a & b" ) ) ) );

  QTest::newRow( "filter-wordwrap01" ) << "{% autoescape off %}{{ a|wordwrap:\"3\" }} {{ b|wordwrap:\"3\" }}{% endautoescape %}" << dict << "a &\nb a &\nb" << NoError;

  QTest::newRow( "filter-wordwrap02" ) << "{{ a|wordwrap:\"3\" }} {{ b|wordwrap:\"3\" }}" << dict << "a &amp;\nb a &\nb" << NoError;

  dict.clear();
  dict.insert( "a", "a&b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );

  QTest::newRow( "filter-ljust01" ) << "{% autoescape off %}.{{ a|ljust:\"5\" }}. .{{ b|ljust:\"5\" }}.{% endautoescape %}" << dict << ".a&b  . .a&b  ." << NoError;

  QTest::newRow( "filter-ljust02" ) << ".{{ a|ljust:\"5\" }}. .{{ b|ljust:\"5\" }}." << dict << ".a&amp;b  . .a&b  ." << NoError;

  QTest::newRow( "filter-rjust01" ) << "{% autoescape off %}.{{ a|rjust:\"5\" }}. .{{ b|rjust:\"5\" }}.{% endautoescape %}" << dict << ".  a&b. .  a&b." << NoError;

  QTest::newRow( "filter-rjust02" ) << ".{{ a|rjust:\"5\" }}. .{{ b|rjust:\"5\" }}." << dict << ".  a&amp;b. .  a&b." << NoError;

  QTest::newRow( "filter-center01" ) << "{% autoescape off %}.{{ a|center:\"5\" }}. .{{ b|center:\"5\" }}.{% endautoescape %}" << dict << ". a&b . . a&b ." << NoError;

  QTest::newRow( "filter-center02" ) << ".{{ a|center:\"5\" }}. .{{ b|center:\"5\" }}." << dict << ". a&amp;b . . a&b ." << NoError;

  dict.clear();
  dict.insert( "a", "x&y" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "x&amp;y" ) ) ) );

  QTest::newRow( "filter-cut01" ) << "{% autoescape off %}{{ a|cut:\"x\" }} {{ b|cut:\"x\" }}{% endautoescape %}" << dict << "&y &amp;y" << NoError;
  QTest::newRow( "filter-cut02" ) << "{{ a|cut:\"x\" }} {{ b|cut:\"x\" }}" << dict << "&amp;y &amp;y" << NoError;
  QTest::newRow( "filter-cut03" ) << "{% autoescape off %}{{ a|cut:\"&\" }} {{ b|cut:\"&\" }}{% endautoescape %}" << dict << "xy xamp;y" << NoError;
  QTest::newRow( "filter-cut04" ) << "{{ a|cut:\"&\" }} {{ b|cut:\"&\" }}" << dict << "xy xamp;y" << NoError;

  //  Passing ";" to cut can break existing HTML entities, so those strings
  //  are auto-escaped.

  QTest::newRow( "filter-cut05" ) << "{% autoescape off %}{{ a|cut:\";\" }} {{ b|cut:\";\" }}{% endautoescape %}" << dict << "x&y x&ampy" << NoError;
  QTest::newRow( "filter-cut06" ) << "{{ a|cut:\";\" }} {{ b|cut:\";\" }}" << dict << "x&amp;y x&amp;ampy" << NoError;

  //  The "escape" filter works the same whether autoescape is on or off,
  //  but it has no effect on strings already marked as safe.

  dict.clear();
  dict.insert( "a", "x&y" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "x&y" ) ) ) );

  QTest::newRow( "filter-escape01" ) << "{{ a|escape }} {{ b|escape }}" << dict << "x&amp;y x&y" << NoError;
  QTest::newRow( "filter-escape02" ) << "{% autoescape off %}{{ a|escape }} {{ b|escape }}{% endautoescape %}" << dict << "x&amp;y x&y" << NoError;

  //  It is only applied once, regardless of the number of times it
  //  appears in a chain.

  dict.clear();
  dict.insert( "a", "x&y" );

  QTest::newRow( "filter-escape03" ) << "{% autoescape off %}{{ a|escape|escape }}{% endautoescape %}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-escape04" ) << "{{ a|escape|escape }}" << dict << "x&amp;y" << NoError;

  //  Force_escape is applied immediately. It can be used to provide
  //  double-escaping, for example.

  QTest::newRow( "filter-force-escape01" ) << "{% autoescape off %}{{ a|force_escape }}{% endautoescape %}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-force-escape02" ) << "{{ a|force_escape }}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-force-escape03" ) << "{% autoescape off %}{{ a|force_escape|force_escape }}{% endautoescape %}" << dict << "x&amp;amp;y" << NoError;
  QTest::newRow( "filter-force-escape04" ) << "{{ a|force_escape|force_escape }}" << dict << "x&amp;amp;y" << NoError;

  //  Because the result of force_escape is "safe", an additional
  //  escape filter has no effect.

  QTest::newRow( "filter-force-escape05" ) << "{% autoescape off %}{{ a|force_escape|escape }}{% endautoescape %}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-force-escape06" ) << "{{ a|force_escape|escape }}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-force-escape07" ) << "{% autoescape off %}{{ a|escape|force_escape }}{% endautoescape %}" << dict << "x&amp;y" << NoError;
  QTest::newRow( "filter-force-escape08" ) << "{{ a|escape|force_escape }}" << dict << "x&amp;y" << NoError;

  //  The contents in "linebreaks" and "linebreaksbr" are escaped
  //  according to the current autoescape setting.
//
//   //  {"a": "x&\ny", "b": mark_safe("x&\ny")
//   QTest::newRow( "filter-linebreaks01") << "{{ a|linebreaks }} {{ b|linebreaks }}" << dict << "<p>x&amp;<br />y</p> <p>x&<br />y</p>" << NoError;
//
//   //  {"a": "x&\ny", "b": mark_safe("x&\ny")
//   QTest::newRow( "filter-linebreaks02") << "{% autoescape off %}{{ a|linebreaks }} {{ b|linebreaks }}{% endautoescape %}" << dict << "<p>x&<br />y</p> <p>x&<br />y</p>" << NoError;
//
//   //  {"a": "x&\ny", "b": mark_safe("x&\ny")
//   QTest::newRow( "filter-linebreaksbr01") << "{{ a|linebreaksbr }} {{ b|linebreaksbr }}" << dict << "x&amp;<br />y x&<br />y" << NoError;
//
//   //  {"a": "x&\ny", "b": mark_safe("x&\ny")
//   QTest::newRow( "filter-linebreaksbr02") << "{% autoescape off %}{{ a|linebreaksbr }} {{ b|linebreaksbr }}{% endautoescape %}" << dict << "x&<br />y x&<br />y" << NoError;

  dict.clear();
  dict.insert( "a", "<b>hello</b>" );

  QTest::newRow( "filter-safe01" ) << "{{ a }} -- {{ a|safe }}" << dict << "&lt;b&gt;hello&lt;/b&gt; -- <b>hello</b>" << NoError;
  QTest::newRow( "filter-safe02" ) << "{% autoescape off %}{{ a }} -- {{ a|safe }}{% endautoescape %}" << dict << "<b>hello</b> -- <b>hello</b>" << NoError;

//   //  , {"a": ["&", "<"]
//   QTest::newRow( "filter-safeseq01") << "{{ a|join:\", \" }} -- {{ a|safeseq|join:\", \" }}" << dict << "&amp;, &lt; -- &, <" << NoError;
//
//   //  , {"a": ["&", "<"]
//   QTest::newRow( "filter-safeseq02") << "{% autoescape off %}{{ a|join:\", \" }} -- {{ a|safeseq|join:\", \" }}{% endautoescape %}" << dict << "&, < -- &, <" << NoError;
//

  dict.clear();
  dict.insert( "a", "<a>x</a> <p><b>y</b></p>" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "<a>x</a> <p><b>y</b></p>" ) ) ) );

  QTest::newRow( "filter-removetags01" ) << "{{ a|removetags:\"a b\" }} {{ b|removetags:\"a b\" }}" << dict << "x &lt;p&gt;y&lt;/p&gt; x <p>y</p>" << NoError;
  QTest::newRow( "filter-removetags02" ) << "{% autoescape off %}{{ a|removetags:\"a b\" }} {{ b|removetags:\"a b\" }}{% endautoescape %}" << dict << "x <p>y</p> x <p>y</p>" << NoError;
  QTest::newRow( "filter-striptags01" ) << "{{ a|striptags }} {{ b|striptags }}" << dict << "x y x y" << NoError;
  QTest::newRow( "filter-striptags02" ) << "{% autoescape off %}{{ a|striptags }} {{ b|striptags }}{% endautoescape %}" << dict << "x y x y" << NoError;

}

void TestFilters::testListFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  dict.insert( "a", QVariantList() << "a&b" << "x" );
  dict.insert( "b", QVariantList() << QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) << "x" );

  QTest::newRow( "filter-first01" ) << "{{ a|first }} {{ b|first }}" << dict << "a&amp;b a&b" << NoError;
  QTest::newRow( "filter-first02" ) << "{% autoescape off %}{{ a|first }} {{ b|first }}{% endautoescape %}" << dict << "a&b a&b" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "x" << "a&b" );
  dict.insert( "b", QVariantList() << "x" << QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );

  QTest::newRow( "filter-last01" ) << "{{ a|last }} {{ b|last }}" << dict << "a&amp;b a&b" << NoError;
  QTest::newRow( "filter-last02" ) << "{% autoescape off %}{{ a|last }} {{ b|last }}{% endautoescape %}" << dict << "a&b a&b" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "a&b" << "a&b" );
  dict.insert( "b", QVariantList() << QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) << QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );
  QTest::newRow( "filter-random01" ) << "{{ a|random }} {{ b|random }}" << dict << "a&amp;b a&b" << NoError;
  QTest::newRow( "filter-random02" ) << "{% autoescape off %}{{ a|random }} {{ b|random }}{% endautoescape %}" << dict << "a&b a&b" << NoError;

  dict.clear();
  dict.insert( "a", "a&b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a&b" ) ) ) );

  QTest::newRow( "filter-slice01" ) << "{{ a|slice:\"1:3\" }} {{ b|slice:\"1:3\" }}" << dict << "&amp;b &b" << NoError;
  QTest::newRow( "filter-slice02" ) << "{% autoescape off %}{{ a|slice:\"1:3\" }} {{ b|slice:\"1:3\" }}{% endautoescape %}" << dict << "&b &b" << NoError;

//   //  {"a": ["x>", [["<y", []]]]
//   QTest::newRow( "filter-unordered_list01") << "{{ a|unordered_list }}" << dict << "\t<li>x&gt;\n\t<ul>\n\t\t<li>&lt;y</li>\n\t</ul>\n\t</li>" << NoError;
//
//   //  {"a": ["x>", [["<y", []]]]
//   QTest::newRow( "filter-unordered_list02") << "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}" << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>" << NoError;
//
//   //  {"a": ["x>", [[mark_safe("<y"), []]]]
//   QTest::newRow( "filter-unordered_list03") << "{{ a|unordered_list }}" << dict << "\t<li>x&gt;\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>" << NoError;
//
//   //  {"a": ["x>", [[mark_safe("<y"), []]]]
//   QTest::newRow( "filter-unordered_list04") << "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}" << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>" << NoError;
//
//   //  {"a": ["x>", [["<y", []]]]
//   QTest::newRow( "filter-unordered_list05") << "{% autoescape off %}{{ a|unordered_list }}{% endautoescape %}" << dict << "\t<li>x>\n\t<ul>\n\t\t<li><y</li>\n\t</ul>\n\t</li>" << NoError;
//

  //  length filter.
  dict.clear();
  dict.insert( "list", QVariantList() << "4" << QVariant() << true << QVariantHash() );

  QTest::newRow( "length01" ) << "{{ list|length }}" << dict << "4" << NoError;

  dict.clear();
  dict.insert( "list", QVariantList() );

  QTest::newRow( "length02" ) << "{{ list|length }}" << dict << "0" << NoError;

  dict.clear();
  dict.insert( "string", "" );

  QTest::newRow( "length03" ) << "{{ string|length }}" << dict << "0" << NoError;

  dict.clear();
  dict.insert( "string", "django" );

  QTest::newRow( "length04" ) << "{{ string|length }}" << dict << "6" << NoError;

  //  Invalid uses that should fail silently.

  dict.clear();
  dict.insert( "int", 7 );

  QTest::newRow( "length05" ) << "{{ int|length }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "None", QVariant() );

  QTest::newRow( "length06" ) << "{{ None|length }}" << dict << "" << NoError;

  //  length_is filter.

  dict.clear();
  dict.insert( "some_list", QVariantList() << "4" << QVariant() << true << QVariantHash() );

  QTest::newRow( "length_is01" ) << "{% if some_list|length_is:\"4\" %}Four{% endif %}" << dict << "Four" << NoError;

  dict.clear();
  dict.insert( "some_list", QVariantList() << "4" << QVariant() << true << QVariantHash() << 17 );

  QTest::newRow( "length_is02" ) << "{% if some_list|length_is:\"4\" %}Four{% else %}Not Four{% endif %}" << dict << "Not Four" << NoError;

  dict.clear();
  dict.insert( "mystring", "word" );

  QTest::newRow( "length_is03" ) << "{% if mystring|length_is:\"4\" %}Four{% endif %}" << dict << "Four" << NoError;

  dict.clear();
  dict.insert( "mystring", "Python" );

  QTest::newRow( "length_is04" ) << "{% if mystring|length_is:\"4\" %}Four{% else %}Not Four{% endif %}" << dict << "Not Four" << NoError;

  dict.clear();
  dict.insert( "mystring", "" );

  QTest::newRow( "length_is05" ) << "{% if mystring|length_is:\"4\" %}Four{% else %}Not Four{% endif %}" << dict << "Not Four" << NoError;

  dict.clear();
  dict.insert( "var", "django" );

  QTest::newRow( "length_is06" ) << "{% with var|length as my_length %}{{ my_length }}{% endwith %}" << dict << "6" << NoError;

  //  Boolean return value from length_is should not be coerced to a string

  dict.clear();
  QTest::newRow( "length_is07" ) << "{% if \"X\"|length_is:0 %}Length is 0{% else %}Length not 0{% endif %}" << dict << "Length not 0" << NoError;
  QTest::newRow( "length_is08" ) << "{% if \"X\"|length_is:1 %}Length is 1{% else %}Length not 1{% endif %}" << dict << "Length is 1" << NoError;

  //  Invalid uses that should fail silently.

  dict.clear();
  dict.insert( "var", "django" );

  QTest::newRow( "length_is09" ) << "{{ var|length_is:\"fish\" }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "int", 7 );

  QTest::newRow( "length_is10" ) << "{{ int|length_is:\"1\" }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "none", QVariant() );

  QTest::newRow( "length_is11" ) << "{{ none|length_is:\"1\" }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "alpha" << "beta & me" );

  QTest::newRow( "join01" ) << "{{ a|join:\", \" }}" << dict << "alpha, beta &amp; me" << NoError;
  QTest::newRow( "join02" ) << "{% autoescape off %}{{ a|join:\", \" }}{% endautoescape %}" << dict << "alpha, beta & me" << NoError;
  QTest::newRow( "join03" ) << "{{ a|join:\" &amp; \" }}" << dict << "alpha &amp; beta &amp; me" << NoError;
  QTest::newRow( "join04" ) << "{% autoescape off %}{{ a|join:\" &amp; \" }}{% endautoescape %}" << dict << "alpha &amp; beta & me" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "alpha" << "beta & me" );
  dict.insert( "var", " & " );

  QTest::newRow( "join05" ) << "{{ a|join:var }}" << dict << "alpha &amp; beta &amp; me" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "alpha" << "beta & me" );
  dict.insert( "var", QVariant::fromValue( Util::markSafe( QString( " & " ) ) ) );

  QTest::newRow( "join06" ) << "{{ a|join:var }}" << dict << "alpha & beta &amp; me" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "Alpha" << "Beta & me" );
  dict.insert( "var", " & " );

  QTest::newRow( "join07" ) << "{{ a|join:var|lower }}" << dict << "alpha &amp; beta &amp; me" << NoError;

  dict.clear();
  dict.insert( "a", QVariantList() << "Alpha" << "Beta & me" );
  dict.insert( "var", QVariant::fromValue( Util::markSafe( QString( " & " ) ) ) );

  QTest::newRow( "join08" ) << "{{ a|join:var|lower }}" << dict << "alpha & beta &amp; me" << NoError;
}

void TestFilters::testLogicFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  //  Literal string arguments to the default filter are always treated as
  //  safe strings, regardless of the auto-escaping state.

  //  Note: we have to use {"a": ""} here, otherwise the invalid template
  //  variable string interferes with the test result.

  dict.insert( "a", "" );

  QTest::newRow( "filter-default01" ) << "{{ a|default:\"x<\" }}" << dict << "x<" << NoError;
  QTest::newRow( "filter-default02" ) << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict << "x<" << NoError;

  dict.clear();
  dict.insert( "a", QVariant::fromValue( Util::markSafe( QString( "x>" ) ) ) );

  QTest::newRow( "filter-default03" ) << "{{ a|default:\"x<\" }}" << dict << "x>" << NoError;
  QTest::newRow( "filter-default04" ) << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict << "x>" << NoError;

  dict.clear();
  dict.insert( "a", QVariant() );

  QTest::newRow( "filter-default_if_none01" ) << "{{ a|default:\"x<\" }}" << dict << "x<" << NoError;
  QTest::newRow( "filter-default_if_none02" ) << "{% autoescape off %}{{ a|default:\"x<\" }}{% endautoescape %}" << dict << "x<" << NoError;

}


void TestFilters::testMiscFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

//
//   //  {"a": "<1-800-call-me>", "b": mark_safe("<1-800-call-me>")
//   QTest::newRow( "filter-phone2numeric01") << "{{ a|phone2numeric }} {{ b|phone2numeric }}" << dict << "&lt;1-800-2255-63&gt; <1-800-2255-63>" << NoError;
//
//   //  {"a": "<1-800-call-me>", "b": mark_safe("<1-800-call-me>")
//   QTest::newRow( "filter-phone2numeric02") << "{% autoescape off %}{{ a|phone2numeric }} {{ b|phone2numeric }}{% endautoescape %}" << dict << "<1-800-2255-63> <1-800-2255-63>" << NoError;
//
  //  Chaining a bunch of safeness-preserving filters should not alter
  //  the safe status either way.

  dict.insert( "a", "a < b" );
  dict.insert( "b", QVariant::fromValue( Util::markSafe( QString( "a < b" ) ) ) );

  QTest::newRow( "chaining01" ) << "{{ a|capfirst|center:\"7\" }}.{{ b|capfirst|center:\"7\" }}" << dict << " A &lt; b . A < b " << NoError;
  QTest::newRow( "chaining02" ) << "{% autoescape off %}{{ a|capfirst|center:\"7\" }}.{{ b|capfirst|center:\"7\" }}{% endautoescape %}" << dict << " A < b . A < b " << NoError;

  //  Using a filter that forces a string back to unsafe:

  QTest::newRow( "chaining03" ) << "{{ a|cut:\"b\"|capfirst }}.{{ b|cut:\"b\"|capfirst }}" << dict << "A &lt; .A < " << NoError;
  QTest::newRow( "chaining04" ) << "{% autoescape off %}{{ a|cut:\"b\"|capfirst }}.{{ b|cut:\"b\"|capfirst }}{% endautoescape %}" << dict << "A < .A < " << NoError;

  dict.clear();
  dict.insert( "a", "a < b" );


  //  Using a filter that forces safeness does not lead to double-escaping

  QTest::newRow( "chaining05" ) << "{{ a|escape|capfirst }}" << dict << "A &lt; b" << NoError;
  QTest::newRow( "chaining06" ) << "{% autoescape off %}{{ a|escape|capfirst }}{% endautoescape %}" << dict << "A &lt; b" << NoError;

  //  Force to safe, then back (also showing why using force_escape too
  //  early in a chain can lead to unexpected results).

  QTest::newRow( "chaining07" ) << "{{ a|force_escape|cut:\";\" }}" << dict << "a &amp;lt b" << NoError;
  QTest::newRow( "chaining08" ) << "{% autoescape off %}{{ a|force_escape|cut:\";\" }}{% endautoescape %}" << dict << "a &lt b" << NoError;
  QTest::newRow( "chaining09" ) << "{{ a|cut:\";\"|force_escape }}" << dict << "a &lt; b" << NoError;
  QTest::newRow( "chaining10" ) << "{% autoescape off %}{{ a|cut:\";\"|force_escape }}{% endautoescape %}" << dict << "a &lt; b" << NoError;
  QTest::newRow( "chaining11" ) << "{{ a|cut:\"b\"|safe }}" << dict << "a < " << NoError;
  QTest::newRow( "chaining12" ) << "{% autoescape off %}{{ a|cut:\"b\"|safe }}{% endautoescape %}" << dict << "a < " << NoError;
  QTest::newRow( "chaining13" ) << "{{ a|safe|force_escape }}" << dict << "a &lt; b" << NoError;
  QTest::newRow( "chaining14" ) << "{% autoescape off %}{{ a|safe|force_escape }}{% endautoescape %}" << dict << "a &lt; b" << NoError;


//   //  Filters decorated with stringfilter still respect is_safe.
//
//   //  {"unsafe": UnsafeClass()
//   QTest::newRow( "autoescape-stringfilter01") << "{{ unsafe|capfirst }}" << dict << "You &amp; me" << NoError;
//
//   //  {"unsafe": UnsafeClass()
//   QTest::newRow( "autoescape-stringfilter02") << "{% autoescape off %}{{ unsafe|capfirst }}{% endautoescape %}" << dict << "You & me" << NoError;
//
//   //  {"safe": SafeClass()
//   QTest::newRow( "autoescape-stringfilter03") << "{{ safe|capfirst }}" << dict << "You &gt; me" << NoError;
//
//   //  {"safe": SafeClass()
//   QTest::newRow( "autoescape-stringfilter04") << "{% autoescape off %}{{ safe|capfirst }}{% endautoescape %}" << dict << "You &gt; me" << NoError;
//
}

void TestFilters::testIntegerFilters_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  QTest::newRow( "filter-add01" ) << "{{ 1|add:2 }}" << dict << "3" << NoError;

  QTest::newRow( "filter-getdigit01" ) << "{{ 123|get_digit:1 }}" << dict << "3" << NoError;
  QTest::newRow( "filter-getdigit02" ) << "{{ 123|get_digit:2 }}" << dict << "2" << NoError;
  QTest::newRow( "filter-getdigit03" ) << "{{ 123|get_digit:3 }}" << dict << "1" << NoError;
  QTest::newRow( "filter-getdigit04" ) << "{{ 123|get_digit:4 }}" << dict << "123" << NoError;

}

QTEST_MAIN( TestFilters )
#include "testfilters.moc"

#endif

