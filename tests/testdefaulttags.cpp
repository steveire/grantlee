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

#ifndef DEFAULTTAGSTEST_H
#define DEFAULTTAGSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "util_p.h"

#include "grantlee.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestDefaultTags : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void cleanupTestCase();

  void testCommentTag_data();
  void testCommentTag() {
    doTest();
  }

  void testFirstOfTag_data();
  void testFirstOfTag() {
    doTest();
  }

  void testIfTag_data();
  void testIfTag() {
    doTest();
  }

  void testForTag_data();
  void testForTag() {
    doTest();
  }

  void testIfEqualTag_data();
  void testIfEqualTag() {
    doTest();
  }

  void testIfNotEqualTag_data();
  void testIfNotEqualTag() {
    doTest();
  }

  void testTemplateTagTag_data();
  void testTemplateTagTag() {
    doTest();
  }

  void testWithTag_data();
  void testWithTag() {
    doTest();
  }

  void testCycleTag_data();
  void testCycleTag() {
    doTest();
  }

  void testWidthRatioTag_data();
  void testWidthRatioTag() {
    doTest();
  }

  void testFilterTag_data();
  void testFilterTag() {
    doTest();
  }

  void testNowTag_data();
  void testNowTag() {
    doTest();
  }

  void testSpacelessTag_data();
  void testSpacelessTag() {
    doTest();
  }

  void testRegroupTag_data();
  void testRegroupTag() {
    doTest();
  }

  void testIfChangedTag_data();
  void testIfChangedTag() {
    doTest();
  }

  void testAutoescapeTag_data();
  void testAutoescapeTag() {
    doTest();
  }

private:

  void doTest();

  Engine *m_engine;

};

void TestDefaultTags::initTestCase()
{
  m_engine = Engine::instance();

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginDirs( QStringList() << appDirPath + "/grantlee_loadertags/"
                           << appDirPath + "/grantlee_defaulttags/"
                           << appDirPath + "/grantlee_defaultfilters/" );
}

void TestDefaultTags::cleanupTestCase()
{
  delete m_engine;
}

void TestDefaultTags::doTest()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );
  QFETCH( QString, output );
  QFETCH( Grantlee::Error, error );

  Template *t = new Template( this );

  t->setContent( input );

  Context context( dict );

  QString result = t->render( &context );

  if ( t->error() != NoError ) {
    QCOMPARE( t->error(), error );
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE( NoError, error );

  QCOMPARE( t->error(), NoError );

  QCOMPARE( result, output );
}

void TestDefaultTags::testCommentTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "comment-tag01" ) << "{% comment %}this is hidden{% endcomment %}hello" << dict << "hello" << NoError;

  QTest::newRow( "comment-tag02" ) << "{% comment %}this is hidden{% endcomment %}hello{% comment %}foo{% endcomment %}" << dict << "hello" << NoError;
  // Comment tag can contain invalid stuff.
  QTest::newRow( "comment-tag03" ) << "foo{% comment %} {% if %} {% endcomment %}" << dict << "foo" << NoError;
  QTest::newRow( "comment-tag04" ) << "foo{% comment %} {% endblock %} {% endcomment %}" << dict << "foo" << NoError;
  QTest::newRow( "comment-tag05" ) << "foo{% comment %} {% somerandomtag %} {% endcomment %}" << dict << "foo" << NoError;
}

void TestDefaultTags::testFirstOfTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;
  dict.insert( "a", 0 );
  dict.insert( "b", 0 );
  dict.insert( "c", 0 );
  QTest::newRow( "firstof01" ) << "{% firstof a b c %}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "a", 1 );
  dict.insert( "b", 0 );
  dict.insert( "c", 0 );
  QTest::newRow( "firstof02" ) << "{% firstof a b c %}" << dict << "1" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  dict.insert( "b", 2 );
  dict.insert( "c", 0 );
  QTest::newRow( "firstof03" ) << "{% firstof a b c %}" << dict << "2" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  dict.insert( "b", 0 );
  dict.insert( "c", 3 );
  QTest::newRow( "firstof04" ) << "{% firstof a b c %}" << dict << "3" << NoError;

  dict.clear();
  dict.insert( "a", 1 );
  dict.insert( "b", 2 );
  dict.insert( "c", 3 );
  QTest::newRow( "firstof05" ) << "{% firstof a b c %}" << dict << "1" << NoError;

  dict.clear();
  dict.insert( "b", 0 );
  dict.insert( "c", 3 );
  QTest::newRow( "firstof06" ) << "{% firstof a b c %}" << dict << "3" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  QTest::newRow( "firstof07" ) << "{% firstof a b \"c\" %}" << dict << "c" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  dict.insert( "b", 0 );
  QTest::newRow( "firstof08" ) << "{% firstof a b \"c and d\" %}" << dict << "c and d" << NoError;
  QTest::newRow( "firstof09" ) << "{% firstof %}" << dict << "a" << TagSyntaxError;

}


void TestDefaultTags::testIfTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( "foo", true );

  QTest::newRow( "if-tag01" ) << "{% if foo %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  QTest::newRow( "if-tag02" ) << "{% if foo %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow( "if-tag03" ) << "{% if foo %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  // AND

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-and01" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-and02" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-and03" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-and04" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  QTest::newRow( "if-tag-and05" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-and06" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  QTest::newRow( "if-tag-and07" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-and08" ) << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  // OR

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-or01" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-or02" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-or03" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-or04" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  QTest::newRow( "if-tag-or05" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-or06" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  QTest::newRow( "if-tag-or07" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-or08" ) << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  // TODO: multiple ORs


  // NOT

  dict.clear();
  dict.insert( "foo", true );
  QTest::newRow( "if-tag-not01" ) << "{% if not foo %}no{% else %}yes{% endif %}" << dict << "yes" << NoError;
  QTest::newRow( "if-tag-not02" ) << "{% if not %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "not", true );
  QTest::newRow( "if-tag-not03" ) << "{% if not %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;
  QTest::newRow( "if-tag-not04" ) << "{% if not not %}no{% else %}yes{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not05" ) << "{% if not not %}no{% else %}yes{% endif %}" << dict << "no" << NoError;
  QTest::newRow( "if-tag-not06" ) << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not07" ) << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not08" ) << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not09" ) << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not10" ) << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not11" ) << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not12" ) << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not13" ) << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not14" ) << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not15" ) << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not16" ) << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not17" ) << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not18" ) << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not19" ) << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not20" ) << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not21" ) << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not22" ) << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not23" ) << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not24" ) << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not25" ) << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not26" ) << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not27" ) << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not28" ) << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not29" ) << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not30" ) << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not31" ) << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not32" ) << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "foo", true );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not33" ) << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", true );
  QTest::newRow( "if-tag-not34" ) << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "foo", false );
  dict.insert( "bar", false );
  QTest::newRow( "if-tag-not35" ) << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  // AND and OR raises a TemplateSyntaxError
  QTest::newRow( "if-tag-error01" ) << "{% if foo or bar and baz %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;

  dict.clear();
  dict.insert( "foo", true );
  QTest::newRow( "if-tag-error02" ) << "{% if foo and %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "if-tag-error03" ) << "{% if foo or %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "if-tag-error04" ) << "{% if not foo and %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "if-tag-error05" ) << "{% if not foo or %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;

}

void TestDefaultTags::testForTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantList list;
  list << 1 << 2 << 3;
  dict.insert( "values", list );
  QTest::newRow( "for-tag01" ) << "{% for val in values %}{{ val }}{% endfor %}" << dict << "123" << NoError;
  QTest::newRow( "for-tag02" ) << "{% for val in values reversed %}{{ val }}{% endfor %}" << dict << "321" << NoError;
  list.clear();
  list << 6 << 6 << 6;
  QTest::newRow( "for-tag-vars01" ) << "{% for val in values %}{{ forloop.counter }}{% endfor %}" << dict << "123" << NoError;
  QTest::newRow( "for-tag-vars02" ) << "{% for val in values %}{{ forloop.counter0 }}{% endfor %}" << dict << "012" << NoError;
  QTest::newRow( "for-tag-vars03" ) << "{% for val in values %}{{ forloop.revcounter }}{% endfor %}" << dict << "321" << NoError;
  QTest::newRow( "for-tag-vars04" ) << "{% for val in values %}{{ forloop.revcounter0 }}{% endfor %}" << dict << "210" << NoError;
  QTest::newRow( "for-tag-vars05" ) << "{% for val in values %}{% if forloop.first %}f{% else %}x{% endif %}{% endfor %}" << dict << "fxx" << NoError;
  QTest::newRow( "for-tag-vars06" ) << "{% for val in values %}{% if forloop.last %}l{% else %}x{% endif %}{% endfor %}" << dict << "xxl" << NoError;

  dict.clear();
  QVariantHash hash;
  hash.insert( "one", 1 );
  hash.insert( "two", 2 );
  dict.insert( "items", hash );
  QTest::newRow( "for-tag-unpack-dict01" ) << "{% for key,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  QTest::newRow( "for-tag-unpack-dict03" ) << "{% for key, value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack-dict04" ) << "{% for key , value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack-dict05" ) << "{% for key ,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack-dict06" ) << "{% for key value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack-dict07" ) << "{% for key,,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack-dict08" ) << "{% for key,value, in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  // Ensure that a single loopvar doesn't truncate the list in val.
  QTest::newRow( "for-tag-unpack-dict09" ) << "{% for val in items %}{{ val.0 }}:{{ val.1 }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << "one" << 1;
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "two" << 2;
  list.append( QVariant( innerList ) );
  dict.insert( "items", list );
  QTest::newRow( "for-tag-unpack01" ) << "{% for key,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  QTest::newRow( "for-tag-unpack03" ) << "{% for key, value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack04" ) << "{% for key , value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack05" ) << "{% for key ,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack06" ) << "{% for key value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack07" ) << "{% for key,,value in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;
  QTest::newRow( "for-tag-unpack08" ) << "{% for key,value, in items %}{{ key }}:{{ value }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  // Ensure that a single loopvar doesn't truncate the list in val.
  QTest::newRow( "for-tag-unpack09" ) << "{% for val in items %}{{ val.0 }}:{{ val.1 }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

// Otherwise, silently truncate if the length of loopvars differs to the length of each set of items.

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << "one" << 1 << "carrot";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "two" << 2 << "orange";
  list.append( QVariant( innerList ) );
  dict.insert( "items", list );

  QTest::newRow( "for-tag-unpack10" ) << "{% for x,y in items %}{{ x }}:{{ y }}/{% endfor %}" << dict << "one:1/two:2/" << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << "one" << 1;
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "two" << 2;
  list.append( QVariant( innerList ) );
  dict.insert( "items", list );

  QTest::newRow( "for-tag-unpack11" ) << "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" << dict << "one:1,/two:2,/" << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << "one" << 1 << "carrot";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "two" << 2;
  list.append( QVariant( innerList ) );
  dict.insert( "items", list );

  QTest::newRow( "for-tag-unpack12" ) << "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" << dict << "one:1,carrot/two:2,/" << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << "one" << 1 << "carrot";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "two" << 2 << "cheese";
  list.append( QVariant( innerList ) );

  dict.insert( "items", list );

  QTest::newRow( "for-tag-unpack13" ) << "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" << dict << "one:1,carrot/two:2,cheese/" << NoError;

// Empty tag:

  dict.clear();
  dict.insert( "values", QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "for-tag-empty01" ) << "{% for val in values %}{{ val }}{% empty %}empty text{% endfor %}" << dict << "123" << NoError;

  dict.clear();
  dict.insert( "values", QVariantList() );
  QTest::newRow( "for-tag-empty02" ) << "{% for val in values %}{{ val }}{% empty %}values array empty{% endfor %}" << dict << "values array empty" << NoError;

  dict.clear();
  QTest::newRow( "for-tag-empty03" ) << "{% for val in values %}{{ val }}{% empty %}values array not found{% endfor %}" << dict << "values array not found" << NoError;

}

void TestDefaultTags::testIfEqualTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( "a", 1 );
  dict.insert( "b", 2 );

  QTest::newRow( "ifequal01" ) << "{% ifequal a b %}yes{% endifequal %}" << dict << "" << NoError;
  QTest::newRow( "ifequal03" ) << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "a", 1 );
  dict.insert( "b", 1 );

  QTest::newRow( "ifequal02" ) << "{% ifequal a b %}yes{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow( "ifequal04" ) << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "test" );

  QTest::newRow( "ifequal05" ) << "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "no" );

  QTest::newRow( "ifequal06" ) << "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "a", "test" );

  QTest::newRow( "ifequal07" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "no" );

  QTest::newRow( "ifequal08" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();

  QTest::newRow( "ifequal09" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  QTest::newRow( "ifequal10" ) << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;


  QTest::newRow( "ifequal-split01" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.insert( "a", "foo" );
  QTest::newRow( "ifequal-split01" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;
  QTest::newRow( "ifequal-split02" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "a", "test man" );
  QTest::newRow( "ifequal-split03" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow( "ifequal-split04" ) << "{% ifequal a 'test man' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "" );
  QTest::newRow( "ifequal-split05" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "a", "i \"love\" you" );
  QTest::newRow( "ifequal-split06" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "i love you" );
  QTest::newRow( "ifequal-split07" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert( "a", "I'm happy" );
  QTest::newRow( "ifequal-split08" ) << "{% ifequal a 'I\\'m happy' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "slash\\man" );
  QTest::newRow( "ifequal-split09" ) << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", "slashman" );
  QTest::newRow( "ifequal-split10" ) << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;
// NUMERIC RESOLUTION

  dict.clear();
  dict.insert( "x", "5" );

  QTest::newRow( "ifequal-numeric01" ) << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "x", 5 );
  QTest::newRow( "ifequal-numeric02" ) << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", 5.2 );
  QTest::newRow( "ifequal-numeric03" ) << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "" << NoError;
  QTest::newRow( "ifequal-numeric04" ) << "{% ifequal x 5.2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", .2 );

  QTest::newRow( "ifequal-numeric05" ) << "{% ifequal x 0.2 %}yes{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow( "ifequal-numeric06" ) << "{% ifequal x .2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", 2 );

  QTest::newRow( "ifequal-numeric07" ) << "{% ifequal x 2. %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "x", 5 );
  QTest::newRow( "ifequal-numeric08" ) << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "x", "5" );
  QTest::newRow( "ifequal-numeric09" ) << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", -5 );
  QTest::newRow( "ifequal-numeric10" ) << "{% ifequal x -5 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", -5.2 );
  QTest::newRow( "ifequal-numeric11" ) << "{% ifequal x -5.2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "x", 5 );
  QTest::newRow( "ifequal-numeric12" ) << "{% ifequal x +5 %}yes{% endifequal %}" << dict << "yes" << NoError;


  // FILTER EXPRESSIONS AS ARGUMENTS

  dict.clear();
  dict.insert( "a", "a" );
  QTest::newRow( "ifequal-filter01" ) << "{% ifequal a|upper \"A\" %}x{% endifequal %}" << dict << "x" << NoError;

  QTest::newRow( "ifequal-filter02" ) << "{% ifequal \"A\" a|upper %}x{% endifequal %}" << dict << "x" << NoError;

  dict.clear();
  dict.insert( "a", "x" );
  dict.insert( "b", "X" );

  QTest::newRow( "ifequal-filter03" ) << "{% ifequal a|upper b|upper %}x{% endifequal %}" << dict << "x" << NoError;

  dict.clear();
  dict.insert( "x", "aaa" );

  QTest::newRow( "ifequal-filter04" ) << "{% ifequal x|slice:\"1\" \"a\" %}x{% endifequal %}" << dict << "x" << NoError;

  dict.clear();
  dict.insert( "x", "aaa" );

  QTest::newRow( "ifequal-filter05" ) << "{% ifequal x|slice:\"1\"|upper \"A\" %}x{% endifequal %}" << dict << "x" << NoError;

}

void TestDefaultTags::testIfNotEqualTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( "a", 1 );
  dict.insert( "b", 2 );

  QTest::newRow( "ifnotequal01" ) << "{% ifnotequal a b %}yes{% endifnotequal %}" << dict << "yes" << NoError;
  QTest::newRow( "ifnotequal03" ) << "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert( "a", 1 );
  dict.insert( "b", 1 );

  QTest::newRow( "ifnotequal02" ) << "{% ifnotequal a b %}yes{% endifnotequal %}" << dict << "" << NoError;
  QTest::newRow( "ifnotequal04" ) << "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" << dict << "no" << NoError;

}

void TestDefaultTags::testTemplateTagTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "templatetag01" ) << "{% templatetag openblock %}" << dict << "{%" << NoError;
  QTest::newRow( "templatetag02" ) << "{% templatetag closeblock %}" << dict << "%}" << NoError;
  QTest::newRow( "templatetag03" ) << "{% templatetag openvariable %}" << dict << "{{" << NoError;
  QTest::newRow( "templatetag04" ) << "{% templatetag closevariable %}" << dict << "}}" << NoError;
  QTest::newRow( "templatetag05" ) << "{% templatetag %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "templatetag06" ) << "{% templatetag foo %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "templatetag07" ) << "{% templatetag openbrace %}" << dict << "{" << NoError;
  QTest::newRow( "templatetag08" ) << "{% templatetag closebrace %}" << dict << "}" << NoError;
  QTest::newRow( "templatetag09" ) << "{% templatetag openbrace %}{% templatetag openbrace %}" << dict << "{{" << NoError;
  QTest::newRow( "templatetag10" ) << "{% templatetag closebrace %}{% templatetag closebrace %}" << dict << "}}" << NoError;
  QTest::newRow( "templatetag11" ) << "{% templatetag opencomment %}" << dict << "{#" << NoError;
  QTest::newRow( "templatetag12" ) << "{% templatetag closecomment %}" << dict << "#}" << NoError;

}

void TestDefaultTags::testWithTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantHash hash;
  hash.insert( "key", 50 );
  dict.insert( "dict", hash );
  QTest::newRow( "with01" ) << "{% with dict.key as key %}{{ key }}{% endwith %}" << dict << "50" << NoError;
  QTest::newRow( "with02" )
  << "{{ key }}{% with dict.key as key %}{{ key }}-{{ dict.key }}-{{ key }}{% endwith %}{{ key }}"
  << dict << "50-50-50" << NoError;
  QTest::newRow( "with03" ) << "{% with dict.key xx key %}{{ key }}{% endwith %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "with04" ) << "{% with dict.key as %}{{ key }}{% endwith %}" << dict << "" << TagSyntaxError;
}

void TestDefaultTags::testCycleTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "cycle01" ) << "{% cycle a %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "cycle02" ) << "{% cycle a,b,c as abc %}{% cycle abc %}" << dict << "ab" << NoError;
  QTest::newRow( "cycle03" ) << "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}" << dict << "abc" << NoError;
  QTest::newRow( "cycle04" ) << "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" << dict << "abca" << NoError;
  QTest::newRow( "cycle05" ) << "{% cycle a %}" << dict << "" << TagSyntaxError;
  // TODO: This is the same as cycle01. Remove.
  QTest::newRow( "cycle06" ) << "{% cycle a %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "cycle07" ) << "{% cycle a,b,c as foo %}{% cycle bar %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "cycle08" ) << "{% cycle a,b,c as foo %}{% cycle foo %}{{ foo }}{{ foo }}{% cycle foo %}{{ foo }}" << dict << "abbbcc" << NoError;

  dict.insert( "test", QVariantList() << 0 << 1 << 2 << 3 << 4 );
  QTest::newRow( "cycle09" ) << "{% for i in test %}{% cycle a,b %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," << NoError;

  dict.clear();
  QTest::newRow( "cycle10" ) << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}" << dict << "ab" << NoError;
  QTest::newRow( "cycle11" ) << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}" << dict << "abc" << NoError;
  QTest::newRow( "cycle12" ) << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" << dict << "abca" << NoError;

  dict.insert( "test", QVariantList() << 0 << 1 << 2 << 3 << 4 );
  QTest::newRow( "cycle13" ) << "{% for i in test %}{% cycle 'a' 'b' %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," << NoError;

  dict.clear();
  dict.insert( "one", "1" );
  dict.insert( "two", "2" );
  QTest::newRow( "cycle14" ) << "{% cycle one two as foo %}{% cycle foo %}" << dict << "12" << NoError;

  dict.clear();
  dict.insert( "test", QVariantList() << 0 << 1 << 2 << 3 << 4 );
  dict.insert( "aye", "a" );
  dict.insert( "bee", "b" );
  QTest::newRow( "cycle15" ) << "{% for i in test %}{% cycle aye bee %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," << NoError;

  dict.clear();
  dict.insert( "one", "A" );
  dict.insert( "two", "2" );
  QTest::newRow( "cycle16" ) << "{% cycle one|lower two as foo %}{% cycle foo %}" << dict << "a2" << NoError;
}

void TestDefaultTags::testWidthRatioTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( "a", 50 );
  dict.insert( "b", 100 );
  QTest::newRow( "widthratio01" ) << "{% widthratio a b 0 %}" << dict << "0" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  dict.insert( "b", 0 );
  QTest::newRow( "widthratio02" ) << "{% widthratio a b 0 %}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "a", 0 );
  dict.insert( "b", 100 );
  QTest::newRow( "widthratio03" ) << "{% widthratio a b 100 %}" << dict << "0" << NoError;

  dict.clear();
  dict.insert( "a", 50 );
  dict.insert( "b", 100 );
  QTest::newRow( "widthratio04" ) << "{% widthratio a b 100 %}" << dict << "50" << NoError;

  dict.clear();
  dict.insert( "a", 100 );
  dict.insert( "b", 100 );
  QTest::newRow( "widthratio05" ) << "{% widthratio a b 100 %}" << dict << "100" << NoError;

  dict.clear();
  dict.insert( "a", 50 );
  dict.insert( "b", 80 );
  QTest::newRow( "widthratio06" ) << "{% widthratio a b 100 %}" << dict << "63" << NoError;

  dict.clear();
  dict.insert( "a", 50 );
  dict.insert( "b", 70 );
  QTest::newRow( "widthratio07" ) << "{% widthratio a b 100 %}" << dict << "71" << NoError;

  dict.clear();
// Raise exception if we don't have 3 args, last one an integer
  QTest::newRow( "widthratio08" ) << "{% widthratio %}" << dict << "" << TagSyntaxError;

  dict.clear();
  QTest::newRow( "widthratio09" ) << "{% widthratio a b %}" << dict << "" << TagSyntaxError;

  dict.clear();
  dict.insert( "a", 50 );
  dict.insert( "b", 100 );
  QTest::newRow( "widthratio10" ) << "{% widthratio a b 100.0 %}" << dict << "50" << NoError;

  dict.clear();
  dict.insert( "a", 50 );
  dict.insert( "b", 100 );
  dict.insert( "c", 100 );
  QTest::newRow( "widthratio11" ) << "{% widthratio a b c %}" << dict << "50" << NoError;

}


void TestDefaultTags::testFilterTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "filter01" ) << "{% filter upper %}{% endfilter %}" << dict << "" << NoError;
  QTest::newRow( "filter02" ) << "{% filter upper %}django{% endfilter %}" << dict << "DJANGO" << NoError;
  QTest::newRow( "filter03" ) << "{% filter upper|lower %}django{% endfilter %}" << dict << "django" << NoError;

  dict.insert( "remove", "spam" );
  QTest::newRow( "filter04" ) << "{% filter cut:remove %}djangospam{% endfilter %}" << dict << "django" << NoError;

}

void TestDefaultTags::testNowTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QDate today = QDateTime::currentDateTime().date();

  QTest::newRow( "now01" ) << "{% now \"d M yyyy\"%}" << dict << ( QString::number( today.day() ) + " " +  QString::number( today.month() ) + " " + QString::number( today.year() ) ) << NoError;


}

void TestDefaultTags::testSpacelessTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "spaceless01" ) << "{% spaceless %} <b>    <i> text </i>    </b> {% endspaceless %}" << dict << "<b><i> text </i></b>" << NoError;
  QTest::newRow( "spaceless02" ) << "{% spaceless %} <b> \n <i> text </i> \n </b> {% endspaceless %}" << dict << "<b><i> text </i></b>" << NoError;
  QTest::newRow( "spaceless03" ) << "{% spaceless %}<b><i>text</i></b>{% endspaceless %}" << dict << "<b><i>text</i></b>" << NoError;

}

void TestDefaultTags::testRegroupTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantList list;
  QVariantHash hash;

  hash.insert( "foo", "c" );
  hash.insert( "bar", 1 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "d" );
  hash.insert( "bar", 1 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "a" );
  hash.insert( "bar", 2 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "b" );
  hash.insert( "bar", 2 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "x" );
  hash.insert( "bar", 3 );
  list.append( hash );

  dict.insert( "data", list );

  QTest::newRow( "regroup01" ) << "{% regroup data by bar as grouped %}"
  "{% for group in grouped %}"
  "{{ group.grouper }}:"
  "{% for item in group.list %}"
  "{{ item.foo }}"
  "{% endfor %},"
  "{% endfor %}" << dict << "1:cd,2:ab,3:x," << NoError;

  dict.clear();
  hash.clear();
  list.clear();

  hash.insert( "foo", "a" );
  hash.insert( "bar", 2 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "b" );
  hash.insert( "bar", 2 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "x" );
  hash.insert( "bar", 3 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "c" );
  hash.insert( "bar", 1 );
  list.append( hash );

  hash.clear();
  hash.insert( "foo", "d" );
  hash.insert( "bar", 1 );
  list.append( hash );


  dict.insert( "data", list );

  // Data is output in the order it is sent in.

  QTest::newRow( "regroup02" ) << "{% regroup data by bar as grouped %}"
  "{% for group in grouped %}"
  "{{ group.grouper }}:"
  "{% for item in group.list %}"
  "{{ item.foo }}"
  "{% endfor %},"
  "{% endfor %}" << dict << "2:ab,3:x,1:cd," << NoError;

}

void TestDefaultTags::testIfChangedTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( "num", QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged01" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" << dict << "123" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 3 );
  QTest::newRow( "ifchanged02" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" << dict << "13" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 1 );
  QTest::newRow( "ifchanged03" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" << dict << "1" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 2 << 3 );
  dict.insert( "numx", QVariantList() << 2 << 2 << 2 );
  QTest::newRow( "ifchanged04" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "122232" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 1 );
  dict.insert( "numx", QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged05" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "1123123123" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 1 );
  dict.insert( "numx", QVariantList() << 2 << 2 << 2 );
  QTest::newRow( "ifchanged06" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "1222" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 1 );
  dict.insert( "numx", QVariantList() << 2 << 2 << 2 );
  dict.insert( "numy", QVariantList() << 3 << 3 << 3 );
  QTest::newRow( "ifchanged07" ) << "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% for y in numy %}{% ifchanged %}{{ y }}{% endifchanged %}{% endfor %}{% endfor %}{% endfor %}" << dict << "1233323332333" << NoError;

  // datalist': [[(1, 'a'), (1, 'a'), (0, 'b'), (1, 'c')], [(0, 'a'), (1, 'c'), (1, 'd'), (1, 'd'), (0, 'e')]]}
  dict.clear();
  QVariantList list;
  QVariantList innerList;
  QVariantList tuple;
  tuple << 1 << "a";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << "a";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 0 << "b";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << "c";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  list.append( QVariant( innerList ) );
  innerList.clear();

  tuple << 0 << "a";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << "c";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << "d";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << "d";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 0 << "e";
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  list.append( QVariant( innerList ) );
  innerList.clear();

  dict.insert( "datalist", list );
  QTest::newRow( "ifchanged08" ) << "{% for data in datalist %}{% for c,d in data %}{% if c %}{% ifchanged %}{{ d }}{% endifchanged %}{% endif %}{% endfor %}{% endfor %}" << dict << "accd" << NoError;

// Test one parameter given to ifchanged.
  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged-param01" ) << "{% for n in num %}{% ifchanged n %}..{% endifchanged %}{{ n }}{% endfor %}" << dict << "..1..2..3" << NoError;

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 2 << 3 );
  dict.insert( "numx", QVariantList() << 5 << 6 << 7 );
  QTest::newRow( "ifchanged-param02" ) << "{% for n in num %}{% for x in numx %}{% ifchanged n %}..{% endifchanged %}{{ x }}{% endfor %}{% endfor %}" << dict << "..567..567..567" << NoError;

// Test multiple parameters to ifchanged.

  dict.clear();
  dict.insert( "num", QVariantList() << 1 << 1 << 2 );
  dict.insert( "numx", QVariantList() << 5 << 6 << 6 );
  QTest::newRow( "ifchanged-param03" ) << "{% for n in num %}{{ n }}{% for x in numx %}{% ifchanged x n %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "156156256" << NoError;

// Test a date+hour like construct, where the hour of the last day
// is the same but the date had changed, so print the hour anyway.

  dict.clear();
  QVariantList days;
  QVariantHash hash;
  hash.insert( "day", 1 );
  hash.insert( "hours", QVariantList() << 1 << 2 << 3 );
  days << hash;
  hash.clear();
  hash.insert( "day", 2 );
  hash.insert( "hours", QVariantList() << 3 );
  days << hash;
  dict.insert( "days", days );
  QTest::newRow( "ifchanged-param04" ) << "{% for d in days %}{% ifchanged %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "112323" << NoError;


// Logically the same as above, just written with explicit
// ifchanged for the day.

  // TODO: fix name conflict upstream
  QTest::newRow( "ifchanged-param05" ) << "{% for d in days %}{% ifchanged d.day %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d.day h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}" << dict << "112323" << NoError;

// Test the else clause of ifchanged.
  dict.clear();
  dict.insert( "ids", QVariantList() << 1 << 1 << 2 << 2 << 2 << 3 );
  QTest::newRow( "ifchanged-else01" ) << "{% for id in ids %}{{ id }}{% ifchanged id %}-first{% else %}-other{% endifchanged %},{% endfor %}" << dict << "1-first,1-other,2-first,2-other,2-other,3-first," << NoError;
  QTest::newRow( "ifchanged-else02" ) << "{% for id in ids %}{{ id }}-{% ifchanged id %}{% cycle red,blue %}{% else %}grey{% endifchanged %},{% endfor %}" << dict << "1-red,1-grey,2-blue,2-grey,2-grey,3-red," << NoError;
  QTest::newRow( "ifchanged-else03" ) << "{% for id in ids %}{{ id }}{% ifchanged id %}-{% cycle red,blue %}{% else %}{% endifchanged %},{% endfor %}" << dict << "1-red,1,2-blue,2,2,3-red," << NoError;

  dict.clear();
  dict.insert( "ids", QVariantList() << 1 << 1 << 2 << 2 << 2 << 3 << 4 );
  QTest::newRow( "ifchanged-else04" ) << "{% for id in ids %}{% ifchanged %}***{{ id }}*{% else %}...{% endifchanged %}{{ forloop.counter }}{% endfor %}" << dict << "***1*1...2***2*3...4...5***3*6***4*7" << NoError;

}


void TestDefaultTags::testAutoescapeTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "autoescape-tag01" ) << "{% autoescape off %}hello{% endautoescape %}" << dict << "hello" << NoError;

  dict.insert( "first", "<b>hello</b>" );
  QTest::newRow( "autoescape-tag02" ) << "{% autoescape off %}{{ first }}{% endautoescape %}" << dict << "<b>hello</b>" << NoError;
  QTest::newRow( "autoescape-tag03" ) << "{% autoescape on %}{{ first }}{% endautoescape %}" << dict << "&lt;b&gt;hello&lt;/b&gt;" << NoError;
  // Autoescape disabling and enabling nest in a predictable way.
  dict.insert( "first", "<a>" );
  QTest::newRow( "autoescape-tag04" ) << "{% autoescape off %}{{ first }} {% autoescape  on%}{{ first }}{% endautoescape %}{% endautoescape %}" << dict << "<a> &lt;a&gt;" << NoError;

  dict.insert( "first", "<b>first</b>" );
  QTest::newRow( "autoescape-tag05" ) << "{% autoescape on %}{{ first }}{% endautoescape %}" << dict << "&lt;b&gt;first&lt;/b&gt;" << NoError;
  // Strings (ASCII or unicode) already marked as "safe" are not
  // auto-escaped
  SafeString safeString( "<b>first</b>" );
  QVariant safeStringVar = QVariant::fromValue<SafeString>( Util::markSafe( safeString ) );
  dict.insert( "first", safeStringVar );

  QTest::newRow( "autoescape-tag06" ) << "{{ first }}" << dict << "<b>first</b>" << NoError;
  QTest::newRow( "autoescape-tag07" ) << "{% autoescape on %}{{ first }}{% endautoescape %}" << dict << "<b>first</b>" << NoError;

  // Literal string arguments to filters, if used in the result, are
  // safe.
  dict.clear();
  dict.insert( "var", QVariant() );
  QTest::newRow( "autoescape-tag08" ) << "{% autoescape on %}{{ var|default_if_none:\"endquote\\\" hah\" }}{% endautoescape %}" << dict << "endquote\" hah" << NoError;
  // Objects which return safe strings as their __unicode__ method
  // won't get double-escaped.
//   'autoescape-tag09': (r'{{ unsafe }}', {'unsafe': filters.UnsafeClass()}, 'you &amp; me'),
//   'autoescape-tag10': (r'{{ safe }}', {'safe': filters.SafeClass()}, 'you &gt; me'),
  // The "safe" and "escape" filters cannot work due to internal
  // implementation details (fortunately, the (no)autoescape block
  // tags can be used in those cases)
  dict.clear();
  dict.insert( "first", "<a>" );
  QTest::newRow( "autoescape-filtertag01" ) << "{{ first }}{% filter safe %}{{ first }} x<y{% endfilter %}" << dict << "" << TagSyntaxError;
}


QTEST_MAIN( TestDefaultTags )
#include "testdefaulttags.moc"

#endif

