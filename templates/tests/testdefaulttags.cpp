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

#include <QtTest>
#include <QtCore/QObject>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "util.h"
#include "grantlee_paths.h"
#include "metatype.h"
#include "coverageobject.h"

typedef QList<QVariantList> Table;
typedef QHash<QString, QVariant> Dict;
typedef QPair<QString, QString> StringPair;

Q_DECLARE_METATYPE( Table )
Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( StringPair )
Q_DECLARE_METATYPE( Grantlee::Error )

class FakeTemplateLoader : public Grantlee::InMemoryTemplateLoader
{
public:

  typedef QSharedPointer<FakeTemplateLoader> Ptr;

  FakeTemplateLoader()
    : Grantlee::InMemoryTemplateLoader()
  {
    m_existingMedia << QString::fromLatin1( "existing_image.png" )
                    << QString::fromLatin1( "another_existing_image.png" )
                    << QString::fromLatin1( "this&that.png" );
  }

  /* reimp */ QPair<QString, QString> getMediaUri( const QString &fileName ) const {
    if ( m_existingMedia.contains( fileName ) )
      return qMakePair( QString::fromLatin1( "/path/to/" ), fileName );
    return QPair<QString, QString>();
  }

private:
  QStringList m_existingMedia;
};

using namespace Grantlee;

class TestDefaultTags : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
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

  void testMediaFinderTag_data();
  void testMediaFinderTag() {
    doTest();
  }

  void testRangeTag_data();
  void testRangeTag() {
    doTest();
  }

  void testDebugTag_data();
  void testDebugTag() {
    doTest();
  }

  void testLoadTag_data();
  void testLoadTag() {
    doTest();
  }

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
  m_engine = new Engine( this );
  m_engine->setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  FakeTemplateLoader::Ptr loader1 = FakeTemplateLoader::Ptr( new FakeTemplateLoader() );

  m_engine->addTemplateLoader( loader1 );

  Grantlee::registerMetaType<Table>();
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

  Template t = m_engine->newTemplate( input, QLatin1String( QTest::currentDataTag() ) );

  Context context( dict );

  QString result = t->render( &context );

  if ( t->error() != NoError ) {
    if ( t->error() != error )
      qDebug() << t->errorString();
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

  QTest::newRow( "comment-tag01" ) << QString::fromLatin1( "{% comment %}this is hidden{% endcomment %}hello" ) << dict << QString::fromLatin1( "hello" ) << NoError;

  QTest::newRow( "comment-tag02" ) << QString::fromLatin1( "{% comment %}this is hidden{% endcomment %}hello{% comment %}foo{% endcomment %}" ) << dict << QString::fromLatin1( "hello" ) << NoError;
  // Comment tag can contain invalid stuff.
  QTest::newRow( "comment-tag03" ) << QString::fromLatin1( "foo{% comment %} {% if %} {% endcomment %}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-tag04" ) << QString::fromLatin1( "foo{% comment %} {% endblock %} {% endcomment %}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-tag05" ) << QString::fromLatin1( "foo{% comment %} {% somerandomtag %} {% endcomment %}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-tag06" ) << QString::fromLatin1( "{% comment %}yes" ) << dict << QString() << UnclosedBlockTagError;
}

void TestDefaultTags::testFirstOfTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 0 );
  dict.insert( QLatin1String( "c" ), 0 );
  QTest::newRow( "firstof01" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 0 );
  dict.insert( QLatin1String( "c" ), 0 );
  QTest::newRow( "firstof02" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString::fromLatin1( "1" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 2 );
  dict.insert( QLatin1String( "c" ), 0 );
  QTest::newRow( "firstof03" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString::fromLatin1( "2" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 0 );
  dict.insert( QLatin1String( "c" ), 3 );
  QTest::newRow( "firstof04" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString::fromLatin1( "3" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 2 );
  dict.insert( QLatin1String( "c" ), 3 );
  QTest::newRow( "firstof05" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString::fromLatin1( "1" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "b" ), 0 );
  dict.insert( QLatin1String( "c" ), 3 );
  QTest::newRow( "firstof06" ) << QString::fromLatin1( "{% firstof a b c %}" ) << dict << QString::fromLatin1( "3" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  QTest::newRow( "firstof07" ) << "{% firstof a b \"c\" %}" << dict << QString::fromLatin1( "c" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 0 );
  QTest::newRow( "firstof08" ) << "{% firstof a b \"c and d\" %}" << dict << QString::fromLatin1( "c and d" ) << NoError;
  QTest::newRow( "firstof09" ) << QString::fromLatin1( "{% firstof %}" ) << dict << QString::fromLatin1( "a" ) << TagSyntaxError;

}


void TestDefaultTags::testIfTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "foo" ), true );

  QTest::newRow( "if-tag01" ) << QString::fromLatin1( "{% if foo %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  QTest::newRow( "if-tag02" ) << QString::fromLatin1( "{% if foo %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag03" ) << QString::fromLatin1( "{% if foo %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  // AND

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-and01" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-and02" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-and03" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-and04" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  QTest::newRow( "if-tag-and05" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-and06" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  QTest::newRow( "if-tag-and07" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-and08" ) << QString::fromLatin1( "{% if foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  // OR

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-or01" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-or02" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-or03" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-or04" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  QTest::newRow( "if-tag-or05" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-or06" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  QTest::newRow( "if-tag-or07" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-or08" ) << QString::fromLatin1( "{% if foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  // TODO: multiple ORs


  // NOT

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  QTest::newRow( "if-tag-not01" ) << QString::fromLatin1( "{% if not foo %}no{% else %}yes{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "if-tag-not02" ) << QString::fromLatin1( "{% if not %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "not" ), true );
  QTest::newRow( "if-tag-not03" ) << QString::fromLatin1( "{% if not %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "if-tag-not04" ) << QString::fromLatin1( "{% if not not %}no{% else %}yes{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not05" ) << QString::fromLatin1( "{% if not not %}no{% else %}yes{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;
  QTest::newRow( "if-tag-not06" ) << QString::fromLatin1( "{% if foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not07" ) << QString::fromLatin1( "{% if foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not08" ) << QString::fromLatin1( "{% if foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not09" ) << QString::fromLatin1( "{% if foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not10" ) << QString::fromLatin1( "{% if foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not11" ) << QString::fromLatin1( "{% if not foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not12" ) << QString::fromLatin1( "{% if not foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not13" ) << QString::fromLatin1( "{% if not foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not14" ) << QString::fromLatin1( "{% if not foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not15" ) << QString::fromLatin1( "{% if not foo and bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not16" ) << QString::fromLatin1( "{% if foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not17" ) << QString::fromLatin1( "{% if foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not18" ) << QString::fromLatin1( "{% if foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not19" ) << QString::fromLatin1( "{% if foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not20" ) << QString::fromLatin1( "{% if foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not21" ) << QString::fromLatin1( "{% if not foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not22" ) << QString::fromLatin1( "{% if not foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not23" ) << QString::fromLatin1( "{% if not foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not24" ) << QString::fromLatin1( "{% if not foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not25" ) << QString::fromLatin1( "{% if not foo or bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not26" ) << QString::fromLatin1( "{% if not foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not27" ) << QString::fromLatin1( "{% if not foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not28" ) << QString::fromLatin1( "{% if not foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not29" ) << QString::fromLatin1( "{% if not foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not30" ) << QString::fromLatin1( "{% if not foo and not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-not31" ) << QString::fromLatin1( "{% if not foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not32" ) << QString::fromLatin1( "{% if not foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not33" ) << QString::fromLatin1( "{% if not foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), true );
  QTest::newRow( "if-tag-not34" ) << QString::fromLatin1( "{% if not foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), false );
  dict.insert( QLatin1String( "bar" ), false );
  QTest::newRow( "if-tag-not35" ) << QString::fromLatin1( "{% if not foo or not bar %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  // AND and OR raises a TemplateSyntaxError
  QTest::newRow( "if-tag-error01" ) << QString::fromLatin1( "{% if foo or bar and baz %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  dict.insert( QLatin1String( "foo" ), true );
  QTest::newRow( "if-tag-error02" ) << QString::fromLatin1( "{% if foo and %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "if-tag-error03" ) << QString::fromLatin1( "{% if foo or %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "if-tag-error04" ) << QString::fromLatin1( "{% if not foo and %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "if-tag-error05" ) << QString::fromLatin1( "{% if not foo or %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "if-tag-error06" ) << QString::fromLatin1( "{% if %}yes{% else %}no{% endif %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "if-tag-error07" ) << QString::fromLatin1( "{% if foo %}yes" ) << dict << QString() << UnclosedBlockTagError;

  // Truthiness
  dict.clear();
  QVariantHash hash;
  dict.insert( QLatin1String( "var" ), hash );
  QTest::newRow( "if-truthiness01" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  hash.insert( QLatin1String( "foo" ), QLatin1String( "bar" ) );
  dict.insert( QLatin1String( "var" ), hash );
  QTest::newRow( "if-truthiness02" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;
  QVariantList list;
  dict.insert( QLatin1String( "var" ), list );
  QTest::newRow( "if-truthiness03" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  list.append( QLatin1String( "foo" ) );
  dict.insert( QLatin1String( "var" ), list );
  QTest::newRow( "if-truthiness04" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;

  QVariant var;
  dict.insert( QLatin1String( "var" ), var );
  QTest::newRow( "if-truthiness05" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  var = QLatin1String( "foo" );
  dict.insert( QLatin1String( "var" ), var );
  QTest::newRow( "if-truthiness06" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;

  QString str;
  dict.insert( QLatin1String( "var" ), str );
  QTest::newRow( "if-truthiness07" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  str = QLatin1String( "foo" );
  dict.insert( QLatin1String( "var" ), str );
  QTest::newRow( "if-truthiness08" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;

  int i = 0;
  dict.insert( QLatin1String( "var" ), i );
  QTest::newRow( "if-truthiness07" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  i = 7;
  dict.insert( QLatin1String( "var" ), i );
  QTest::newRow( "if-truthiness08" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;

  qreal r = 0.0;
  dict.insert( QLatin1String( "var" ), r );
  QTest::newRow( "if-truthiness09" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "No" ) << NoError;
  r = 7.1;
  dict.insert( QLatin1String( "var" ), r );
  QTest::newRow( "if-truthiness10" ) << QString::fromLatin1( "{% if var %}Yes{% else %}No{% endif %}" ) << dict << QString::fromLatin1( "Yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-badarg01" ) << QString::fromLatin1( "{% if x|default_if_none:y %}yes{% endif %}" ) << dict << QString() << NoError;

  dict.insert( QLatin1String( "y" ), 0 );

  QTest::newRow( "if-tag-badarg02" ) << QString::fromLatin1( "{% if x|default_if_none:y %}yes{% endif %}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "y" ), 1 );

  QTest::newRow( "if-tag-badarg03" ) << QString::fromLatin1( "{% if x|default_if_none:y %}yes{% endif %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  QTest::newRow( "if-tag-badarg04" ) << QString::fromLatin1( "{% if x|default_if_none:y %}yes{% else %}no{% endif %}" ) << dict << QString::fromLatin1( "no" ) << NoError;
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
  dict.insert( QLatin1String( "values" ), list );
  QTest::newRow( "for-tag01" ) << QString::fromLatin1( "{% for val in values %}{{ val }}{% endfor %}" ) << dict << QString::fromLatin1( "123" ) << NoError;
  QTest::newRow( "for-tag02" ) << QString::fromLatin1( "{% for val in values reversed %}{{ val }}{% endfor %}" ) << dict << QString::fromLatin1( "321" ) << NoError;
  list.clear();
  dict.insert( QLatin1String( "values" ), list );
  QTest::newRow( "for-tag03" ) << QString::fromLatin1( "{% for val in values %}({{ val }} sdfsdf,){% endfor %}" ) << dict << QString() << NoError;
  QStringList emails;
  emails << QString::fromLatin1( "one" ) << QString::fromLatin1( "two" );
  QVariantHash obj;
  obj.insert( QLatin1String( "emails" ), emails );
  dict.insert( QLatin1String( "contact" ), obj );
  QTest::newRow( "for-tag04" ) << QString::fromLatin1( "{% for val in contact.emails %}({{ val }},){% endfor %}" ) << dict << QString::fromLatin1( "(one,)(two,)" ) << NoError;
  emails.clear();
  obj.insert( QLatin1String( "emails" ), emails );
  dict.insert( QLatin1String( "contact" ), obj );
  QTest::newRow( "for-tag05" ) << QString::fromLatin1( "{% for val in contact.emails %}({{ val }},){% endfor %}" ) << dict << QString() << NoError;
  list.clear();
  dict.clear();
  emails << QString::fromLatin1( "one" );
  dict.insert( QLatin1String( "emails" ), emails );
  QTest::newRow( "for-tag06" ) << QString::fromLatin1( "{% for val in emails %}({{ val }},){% endfor %}" ) << dict << QString::fromLatin1( "(one,)" ) << NoError;
  list.clear();
  dict.clear();

  QTest::newRow( "for-tag07" ) << QString::fromLatin1( "{% for %}{% endfor %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "for-tag08" ) << QString::fromLatin1( "{% for foo bar bat %}{% endfor %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "for-tag09" ) << QString::fromLatin1( "{% for foo bar '' %}{% endfor %}" ) << dict << QString() << TagSyntaxError;

  list << 1 << 2 << 3;
  dict.insert( QLatin1String( "values" ), list );
  QTest::newRow( "for-tag-vars01" ) << QString::fromLatin1( "{% for val in values %}{{ forloop.counter }}{% endfor %}" ) << dict << QString::fromLatin1( "123" ) << NoError;
  QTest::newRow( "for-tag-vars02" ) << QString::fromLatin1( "{% for val in values %}{{ forloop.counter0 }}{% endfor %}" ) << dict << QString::fromLatin1( "012" ) << NoError;
  QTest::newRow( "for-tag-vars03" ) << QString::fromLatin1( "{% for val in values %}{{ forloop.revcounter }}{% endfor %}" ) << dict << QString::fromLatin1( "321" ) << NoError;
  QTest::newRow( "for-tag-vars04" ) << QString::fromLatin1( "{% for val in values %}{{ forloop.revcounter0 }}{% endfor %}" ) << dict << QString::fromLatin1( "210" ) << NoError;
  QTest::newRow( "for-tag-vars05" ) << QString::fromLatin1( "{% for val in values %}{% if forloop.first %}f{% else %}x{% endif %}{% endfor %}" ) << dict << QString::fromLatin1( "fxx" ) << NoError;
  QTest::newRow( "for-tag-vars06" ) << QString::fromLatin1( "{% for val in values %}{% if forloop.last %}l{% else %}x{% endif %}{% endfor %}" ) << dict << QString::fromLatin1( "xxl" ) << NoError;

  dict.clear();
  QVariantHash hash;
//   hash.insert( QLatin1String( "one" ), 1 );
//   hash.insert( QLatin1String( "two" ), 2 );
//   dict.insert( QLatin1String( "items" ), hash );
//   QTest::newRow( "for-tag-unpack-dict01" ) << QString::fromLatin1( "{% for key,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//
//   QTest::newRow( "for-tag-unpack-dict03" ) << QString::fromLatin1( "{% for key, value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//   QTest::newRow( "for-tag-unpack-dict04" ) << QString::fromLatin1( "{% for key , value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//   QTest::newRow( "for-tag-unpack-dict05" ) << QString::fromLatin1( "{% for key ,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//   QTest::newRow( "for-tag-unpack-dict06" ) << QString::fromLatin1( "{% for key value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//   QTest::newRow( "for-tag-unpack-dict07" ) << QString::fromLatin1( "{% for key,,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//   QTest::newRow( "for-tag-unpack-dict08" ) << QString::fromLatin1( "{% for key,value, in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
//
//   // Ensure that a single loopvar doesn't truncate the list in val.
//   QTest::newRow( "for-tag-unpack-dict09" ) << QString::fromLatin1( "{% for val in items %}{{ val.0 }}:{{ val.1 }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << QString::fromLatin1( "one" ) << 1;
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "two" ) << 2;
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "items" ), list );
  QTest::newRow( "for-tag-unpack01" ) << QString::fromLatin1( "{% for key,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;

  QTest::newRow( "for-tag-unpack03" ) << QString::fromLatin1( "{% for key, value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
  QTest::newRow( "for-tag-unpack04" ) << QString::fromLatin1( "{% for key , value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
  QTest::newRow( "for-tag-unpack05" ) << QString::fromLatin1( "{% for key ,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
  QTest::newRow( "for-tag-unpack06" ) << QString::fromLatin1( "{% for key value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
  QTest::newRow( "for-tag-unpack07" ) << QString::fromLatin1( "{% for key,,value in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;
  QTest::newRow( "for-tag-unpack08" ) << QString::fromLatin1( "{% for key,value, in items %}{{ key }}:{{ value }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;

  // Ensure that a single loopvar doesn't truncate the list in val.
  QTest::newRow( "for-tag-unpack09" ) << QString::fromLatin1( "{% for val in items %}{{ val.0 }}:{{ val.1 }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;

// Otherwise, silently truncate if the length of loopvars differs to the length of each set of items.

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QString::fromLatin1( "one" ) << 1 << QString::fromLatin1( "carrot" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "two" ) << 2 << QString::fromLatin1( "orange" );
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "items" ), list );

  QTest::newRow( "for-tag-unpack10" ) << QString::fromLatin1( "{% for x,y in items %}{{ x }}:{{ y }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1/two:2/" ) << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QString::fromLatin1( "one" ) << 1;
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "two" ) << 2;
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "items" ), list );

  QTest::newRow( "for-tag-unpack11" ) << QString::fromLatin1( "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1,/two:2,/" ) << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QString::fromLatin1( "one" ) << 1 << QString::fromLatin1( "carrot" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "two" ) << 2;
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "items" ), list );

  QTest::newRow( "for-tag-unpack12" ) << QString::fromLatin1( "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1,carrot/two:2,/" ) << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QString::fromLatin1( "one" ) << 1 << QString::fromLatin1( "carrot" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "two" ) << 2 << QString::fromLatin1( "cheese" );
  list.append( QVariant( innerList ) );

  dict.insert( QLatin1String( "items" ), list );

  QTest::newRow( "for-tag-unpack13" ) << QString::fromLatin1( "{% for x,y,z in items %}{{ x }}:{{ y }},{{ z }}/{% endfor %}" ) << dict << QString::fromLatin1( "one:1,carrot/two:2,cheese/" ) << NoError;

// Empty tag:

  dict.clear();
  dict.insert( QLatin1String( "values" ), QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "for-tag-empty01" ) << QString::fromLatin1( "{% for val in values %}{{ val }}{% empty %}empty text{% endfor %}" ) << dict << QString::fromLatin1( "123" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "values" ), QVariantList() );
  QTest::newRow( "for-tag-empty02" ) << QString::fromLatin1( "{% for val in values %}{{ val }}{% empty %}values array empty{% endfor %}" ) << dict << QString::fromLatin1( "values array empty" ) << NoError;

  dict.clear();
  QTest::newRow( "for-tag-empty03" ) << QString::fromLatin1( "{% for val in values %}{{ val }}{% empty %}values array not found{% endfor %}" ) << dict << QString::fromLatin1( "values array not found" ) << NoError;

}

void TestDefaultTags::testIfEqualTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 2 );

  QTest::newRow( "ifequal01" ) << QString::fromLatin1( "{% ifequal a b %}yes{% endifequal %}" ) << dict << QString() << NoError;
  QTest::newRow( "ifequal03" ) << QString::fromLatin1( "{% ifequal a b %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 1 );

  QTest::newRow( "ifequal02" ) << QString::fromLatin1( "{% ifequal a b %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "ifequal04" ) << QString::fromLatin1( "{% ifequal a b %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "test" ) );

  QTest::newRow( "ifequal05" ) << QString::fromLatin1( "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "no" ) );

  QTest::newRow( "ifequal06" ) << QString::fromLatin1( "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "test" ) );

  QTest::newRow( "ifequal07" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "no" ) );

  QTest::newRow( "ifequal08" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();

  QTest::newRow( "ifequal09" ) << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  QTest::newRow( "ifequal10" ) << QString::fromLatin1( "{% ifequal a b %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;


  QTest::newRow( "ifequal-split01" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  dict.insert( QLatin1String( "a" ), QLatin1String( "foo" ) );
  QTest::newRow( "ifequal-split02" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "test man" ) );
  QTest::newRow( "ifequal-split03" ) << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "ifequal-split04" ) << QString::fromLatin1( "{% ifequal a 'test man' %}yes{% else %}no{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "" ) );
  QTest::newRow( "ifequal-split05" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "i \"love\" you" ) );
  QTest::newRow( "ifequal-split06" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "i love you" ) );
  QTest::newRow( "ifequal-split07" ) << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "I'm happy" ) );
  QTest::newRow( "ifequal-split08" ) << "{% ifequal a 'I\\'m happy' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "slash\\man" ) );
  QTest::newRow( "ifequal-split09" ) << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "slashman" ) );
  QTest::newRow( "ifequal-split10" ) << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << QString::fromLatin1( "no" ) << NoError;
// NUMERIC RESOLUTION

  dict.clear();
  dict.insert( QLatin1String( "x" ), QLatin1String( "5" ) );

  QTest::newRow( "ifequal-numeric01" ) << QString::fromLatin1( "{% ifequal x 5 %}yes{% endifequal %}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), 5 );
  QTest::newRow( "ifequal-numeric02" ) << QString::fromLatin1( "{% ifequal x 5 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), 5.2 );
  QTest::newRow( "ifequal-numeric03" ) << QString::fromLatin1( "{% ifequal x 5 %}yes{% endifequal %}" ) << dict << QString() << NoError;
  QTest::newRow( "ifequal-numeric04" ) << QString::fromLatin1( "{% ifequal x 5.2 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), .2 );

  QTest::newRow( "ifequal-numeric05" ) << QString::fromLatin1( "{% ifequal x 0.2 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "ifequal-numeric06" ) << QString::fromLatin1( "{% ifequal x .2 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), 2 );

  QTest::newRow( "ifequal-numeric07" ) << QString::fromLatin1( "{% ifequal x 2. %}yes{% endifequal %}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), 5 );
  QTest::newRow( "ifequal-numeric08" ) << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), QLatin1String( "5" ) );
  QTest::newRow( "ifequal-numeric09" ) << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), -5 );
  QTest::newRow( "ifequal-numeric10" ) << QString::fromLatin1( "{% ifequal x -5 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), -5.2 );
  QTest::newRow( "ifequal-numeric11" ) << QString::fromLatin1( "{% ifequal x -5.2 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), 5 );
  QTest::newRow( "ifequal-numeric12" ) << QString::fromLatin1( "{% ifequal x +5 %}yes{% endifequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;


  // FILTER EXPRESSIONS AS ARGUMENTS

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "a" ) );
  QTest::newRow( "ifequal-filter01" ) << "{% ifequal a|upper \"A\" %}x{% endifequal %}" << dict << QString::fromLatin1( "x" ) << NoError;

  QTest::newRow( "ifequal-filter02" ) << "{% ifequal \"A\" a|upper %}x{% endifequal %}" << dict << QString::fromLatin1( "x" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), QLatin1String( "x" ) );
  dict.insert( QLatin1String( "b" ), QLatin1String( "X" ) );

  QTest::newRow( "ifequal-filter03" ) << QString::fromLatin1( "{% ifequal a|upper b|upper %}x{% endifequal %}" ) << dict << QString::fromLatin1( "x" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), QLatin1String( "aaa" ) );

  QTest::newRow( "ifequal-filter04" ) << "{% ifequal x|slice:\"1\" \"a\" %}x{% endifequal %}" << dict << QString::fromLatin1( "x" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "x" ), QLatin1String( "aaa" ) );

  QTest::newRow( "ifequal-filter05" ) << "{% ifequal x|slice:\"1\"|upper \"A\" %}x{% endifequal %}" << dict << QString::fromLatin1( "x" ) << NoError;

  QTest::newRow( "ifequal-error01" ) << "{% ifequal x|slice:\"1\"|upper %}x{% endifequal %}" << dict << QString() << TagSyntaxError;

}

void TestDefaultTags::testIfNotEqualTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 2 );

  QTest::newRow( "ifnotequal01" ) << QString::fromLatin1( "{% ifnotequal a b %}yes{% endifnotequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "ifnotequal03" ) << QString::fromLatin1( "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 1 );
  dict.insert( QLatin1String( "b" ), 1 );

  QTest::newRow( "ifnotequal02" ) << QString::fromLatin1( "{% ifnotequal a b %}yes{% endifnotequal %}" ) << dict << QString() << NoError;
  QTest::newRow( "ifnotequal04" ) << QString::fromLatin1( "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

}

void TestDefaultTags::testTemplateTagTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "templatetag01" ) << QString::fromLatin1( "{% templatetag openblock %}" ) << dict << QString::fromLatin1( "{%" ) << NoError;
  QTest::newRow( "templatetag02" ) << QString::fromLatin1( "{% templatetag closeblock %}" ) << dict << QString::fromLatin1( "%}" ) << NoError;
  QTest::newRow( "templatetag03" ) << QString::fromLatin1( "{% templatetag openvariable %}" ) << dict << QString::fromLatin1( "{{" ) << NoError;
  QTest::newRow( "templatetag04" ) << QString::fromLatin1( "{% templatetag closevariable %}" ) << dict << QString::fromLatin1( "}}" ) << NoError;
  QTest::newRow( "templatetag05" ) << QString::fromLatin1( "{% templatetag %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "templatetag06" ) << QString::fromLatin1( "{% templatetag foo %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "templatetag07" ) << QString::fromLatin1( "{% templatetag openbrace %}" ) << dict << QString::fromLatin1( "{" ) << NoError;
  QTest::newRow( "templatetag08" ) << QString::fromLatin1( "{% templatetag closebrace %}" ) << dict << QString::fromLatin1( "}" ) << NoError;
  QTest::newRow( "templatetag09" ) << QString::fromLatin1( "{% templatetag openbrace %}{% templatetag openbrace %}" ) << dict << QString::fromLatin1( "{{" ) << NoError;
  QTest::newRow( "templatetag10" ) << QString::fromLatin1( "{% templatetag closebrace %}{% templatetag closebrace %}" ) << dict << QString::fromLatin1( "}}" ) << NoError;
  QTest::newRow( "templatetag11" ) << QString::fromLatin1( "{% templatetag opencomment %}" ) << dict << QString::fromLatin1( "{#" ) << NoError;
  QTest::newRow( "templatetag12" ) << QString::fromLatin1( "{% templatetag closecomment %}" ) << dict << QString::fromLatin1( "#}" ) << NoError;

}

void TestDefaultTags::testWithTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantHash hash;
  hash.insert( QLatin1String( "key" ), 50 );
  dict.insert( QLatin1String( "dict" ), hash );
  QTest::newRow( "with01" ) << QString::fromLatin1( "{% with dict.key as key %}{{ key }}{% endwith %}" ) << dict << QString::fromLatin1( "50" ) << NoError;
  QTest::newRow( "with02" )
  << QString::fromLatin1( "{{ key }}{% with dict.key as key %}{{ key }}-{{ dict.key }}-{{ key }}{% endwith %}{{ key }}" )
  << dict << QString::fromLatin1( "50-50-50" ) << NoError;
  QTest::newRow( "with-error01" ) << QString::fromLatin1( "{% with dict.key xx key %}{{ key }}{% endwith %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "with-error02" ) << QString::fromLatin1( "{% with dict.key as %}{{ key }}{% endwith %}" ) << dict << QString() << TagSyntaxError;
}

void TestDefaultTags::testCycleTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "cycle01" ) << QString::fromLatin1( "{% cycle a %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "cycle02" ) << QString::fromLatin1( "{% cycle a,b,c as abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "ab" ) << NoError;
  QTest::newRow( "cycle03" ) << QString::fromLatin1( "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "abc" ) << NoError;
  QTest::newRow( "cycle04" ) << QString::fromLatin1( "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "abca" ) << NoError;
  QTest::newRow( "cycle05" ) << QString::fromLatin1( "{% cycle a %}" ) << dict << QString() << TagSyntaxError;
  // TODO: This is the same as cycle01. Remove.
  QTest::newRow( "cycle06" ) << QString::fromLatin1( "{% cycle a %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "cycle07" ) << QString::fromLatin1( "{% cycle a,b,c as foo %}{% cycle bar %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "cycle08" ) << QString::fromLatin1( "{% cycle a,b,c as foo %}{% cycle foo %}{{ foo }}{{ foo }}{% cycle foo %}{{ foo }}" ) << dict << QString::fromLatin1( "abbbcc" ) << NoError;

  dict.insert( QLatin1String( "test" ), QVariantList() << 0 << 1 << 2 << 3 << 4 );
  QTest::newRow( "cycle09" ) << QString::fromLatin1( "{% for i in test %}{% cycle a,b %}{{ i }},{% endfor %}" ) << dict << QString::fromLatin1( "a0,b1,a2,b3,a4," ) << NoError;

  dict.clear();
  QTest::newRow( "cycle10" ) << QString::fromLatin1( "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "ab" ) << NoError;
  QTest::newRow( "cycle11" ) << QString::fromLatin1( "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "abc" ) << NoError;
  QTest::newRow( "cycle12" ) << QString::fromLatin1( "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" ) << dict << QString::fromLatin1( "abca" ) << NoError;

  dict.insert( QLatin1String( "test" ), QVariantList() << 0 << 1 << 2 << 3 << 4 );
  QTest::newRow( "cycle13" ) << QString::fromLatin1( "{% for i in test %}{% cycle 'a' 'b' %}{{ i }},{% endfor %}" ) << dict << QString::fromLatin1( "a0,b1,a2,b3,a4," ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "one" ), QLatin1String( "1" ) );
  dict.insert( QLatin1String( "two" ), QLatin1String( "2" ) );
  QTest::newRow( "cycle14" ) << QString::fromLatin1( "{% cycle one two as foo %}{% cycle foo %}" ) << dict << QString::fromLatin1( "12" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "test" ), QVariantList() << 0 << 1 << 2 << 3 << 4 );
  dict.insert( QLatin1String( "aye" ), QLatin1String( "a" ) );
  dict.insert( QLatin1String( "bee" ), QLatin1String( "b" ) );
  QTest::newRow( "cycle15" ) << QString::fromLatin1( "{% for i in test %}{% cycle aye bee %}{{ i }},{% endfor %}" ) << dict << QString::fromLatin1( "a0,b1,a2,b3,a4," ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "one" ), QLatin1String( "A" ) );
  dict.insert( QLatin1String( "two" ), QLatin1String( "2" ) );
  QTest::newRow( "cycle16" ) << QString::fromLatin1( "{% cycle one|lower two as foo %}{% cycle foo %}" ) << dict << QString::fromLatin1( "a2" ) << NoError;

  QTest::newRow( "cycle17" ) << QString::fromLatin1( "{% cycle %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "cycle18" ) << QString::fromLatin1( "{% cycle var %}" ) << dict << QString() << TagSyntaxError;

  dict.insert( QLatin1String( "three" ), QLatin1String( "B" ) );
  dict.insert( QLatin1String( "four" ), QLatin1String( "4" ) );

  QTest::newRow( "cycle19" ) << QString::fromLatin1( "{% cycle one two three foo %}" ) << dict << QString::fromLatin1( "A" ) << NoError;
  QTest::newRow( "cycle20" ) << QString::fromLatin1( "{% cycle one two as foo %}{% cycle three four as bar %}" ) << dict << QString::fromLatin1( "AB" ) << NoError;
}

void TestDefaultTags::testWidthRatioTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 100 );
  QTest::newRow( "widthratio01" ) << QString::fromLatin1( "{% widthratio a b 0 %}" ) << dict << QString::fromLatin1( "0" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 0 );
  QTest::newRow( "widthratio02" ) << QString::fromLatin1( "{% widthratio a b 0 %}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 0 );
  dict.insert( QLatin1String( "b" ), 100 );
  QTest::newRow( "widthratio03" ) << QString::fromLatin1( "{% widthratio a b 100 %}" ) << dict << QString::fromLatin1( "0" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 100 );
  QTest::newRow( "widthratio04" ) << QString::fromLatin1( "{% widthratio a b 100 %}" ) << dict << QString::fromLatin1( "50" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 100 );
  dict.insert( QLatin1String( "b" ), 100 );
  QTest::newRow( "widthratio05" ) << QString::fromLatin1( "{% widthratio a b 100 %}" ) << dict << QString::fromLatin1( "100" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 80 );
  QTest::newRow( "widthratio06" ) << QString::fromLatin1( "{% widthratio a b 100 %}" ) << dict << QString::fromLatin1( "63" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 70 );
  QTest::newRow( "widthratio07" ) << QString::fromLatin1( "{% widthratio a b 100 %}" ) << dict << QString::fromLatin1( "71" ) << NoError;

  dict.clear();
// Raise exception if we don't have 3 args, last one an integer
  QTest::newRow( "widthratio08" ) << QString::fromLatin1( "{% widthratio %}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  QTest::newRow( "widthratio09" ) << QString::fromLatin1( "{% widthratio a b %}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 100 );
  QTest::newRow( "widthratio10" ) << QString::fromLatin1( "{% widthratio a b 100.0 %}" ) << dict << QString::fromLatin1( "50" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "a" ), 50 );
  dict.insert( QLatin1String( "b" ), 100 );
  dict.insert( QLatin1String( "c" ), 100 );
  QTest::newRow( "widthratio11" ) << QString::fromLatin1( "{% widthratio a b c %}" ) << dict << QString::fromLatin1( "50" ) << NoError;

}


void TestDefaultTags::testFilterTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "filter01" ) << QString::fromLatin1( "{% filter upper %}{% endfilter %}" ) << dict << QString() << NoError;
  QTest::newRow( "filter02" ) << QString::fromLatin1( "{% filter upper %}django{% endfilter %}" ) << dict << QString::fromLatin1( "DJANGO" ) << NoError;
  QTest::newRow( "filter03" ) << QString::fromLatin1( "{% filter upper|lower %}django{% endfilter %}" ) << dict << QString::fromLatin1( "django" ) << NoError;

  dict.insert( QLatin1String( "remove" ), QLatin1String( "spam" ) );
  QTest::newRow( "filter04" ) << QString::fromLatin1( "{% filter cut:remove %}djangospam{% endfilter %}" ) << dict << QString::fromLatin1( "django" ) << NoError;

}

void TestDefaultTags::testNowTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QDate today = QDateTime::currentDateTime().date();

  QTest::newRow( "now01" ) << QString::fromLatin1( "{% now \"d M yyyy\"%}" ) << dict << ( QString::number( today.day() ) + QLatin1Char( ' ' ) +  QString::number( today.month() ) + QLatin1Char( ' ' ) + QString::number( today.year() ) ) << NoError;

  QTest::newRow( "now02" ) << QString::fromLatin1( "{% now \"d \"M\" yyyy\"%}" ) << dict << QString() << TagSyntaxError;


}

void TestDefaultTags::testSpacelessTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "spaceless01" ) << QString::fromLatin1( "{% spaceless %} <b>    <i> text </i>    </b> {% endspaceless %}" ) << dict << QString::fromLatin1( "<b><i> text </i></b>" ) << NoError;
  QTest::newRow( "spaceless02" ) << "{% spaceless %} <b> \n <i> text </i> \n </b> {% endspaceless %}" << dict << QString::fromLatin1( "<b><i> text </i></b>" ) << NoError;
  QTest::newRow( "spaceless03" ) << QString::fromLatin1( "{% spaceless %}<b><i>text</i></b>{% endspaceless %}" ) << dict << QString::fromLatin1( "<b><i>text</i></b>" ) << NoError;

  dict.insert(QLatin1String( "text" ), QString::fromLatin1( "This & that" ) );
  QTest::newRow( "spaceless04" ) << QString::fromLatin1( "{% spaceless %}<b>  <i>{{ text }}</i>  </b>{% endspaceless %}" ) << dict << QString::fromLatin1( "<b><i>This &amp; that</i></b>" ) << NoError;
  QTest::newRow( "spaceless05" ) << QString::fromLatin1( "{% autoescape off %}{% spaceless %}<b>  <i>{{ text }}</i>  </b>{% endspaceless %}{% endautoescape %}" ) << dict << QString::fromLatin1( "<b><i>This & that</i></b>" ) << NoError;
  QTest::newRow( "spaceless06" ) << QString::fromLatin1( "{% spaceless %}<b>  <i>{{ text|safe }}</i>  </b>{% endspaceless %}" ) << dict << QString::fromLatin1( "<b><i>This & that</i></b>" ) << NoError;

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

  hash.insert( QLatin1String( "foo" ), QLatin1String( "c" ) );
  hash.insert( QLatin1String( "bar" ), 1 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "d" ) );
  hash.insert( QLatin1String( "bar" ), 1 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "a" ) );
  hash.insert( QLatin1String( "bar" ), 2 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "b" ) );
  hash.insert( QLatin1String( "bar" ), 2 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "x" ) );
  hash.insert( QLatin1String( "bar" ), 3 );
  list.append( hash );

  dict.insert( QLatin1String( "data" ), list );

  QTest::newRow( "regroup01" ) << QString::fromLatin1( "{% regroup data by bar as grouped %}"
      "{% for group in grouped %}"
      "{{ group.grouper }}:"
      "{% for item in group.list %}"
      "{{ item.foo }}"
      "{% endfor %},"
      "{% endfor %}" ) << dict << QString::fromLatin1( "1:cd,2:ab,3:x," ) << NoError;

  dict.clear();
  hash.clear();
  list.clear();

  hash.insert( QLatin1String( "foo" ), QLatin1String( "a" ) );
  hash.insert( QLatin1String( "bar" ), 2 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "b" ) );
  hash.insert( QLatin1String( "bar" ), 2 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "x" ) );
  hash.insert( QLatin1String( "bar" ), 3 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "c" ) );
  hash.insert( QLatin1String( "bar" ), 1 );
  list.append( hash );

  hash.clear();
  hash.insert( QLatin1String( "foo" ), QLatin1String( "d" ) );
  hash.insert( QLatin1String( "bar" ), 1 );
  list.append( hash );


  dict.insert( QLatin1String( "data" ), list );

  // Data is output in the order it is sent in.

  QTest::newRow( "regroup02" ) << QString::fromLatin1( "{% regroup data by bar as grouped %}"
      "{% for group in grouped %}"
      "{{ group.grouper }}:"
      "{% for item in group.list %}"
      "{{ item.foo }}"
      "{% endfor %},"
      "{% endfor %}" ) << dict << QString::fromLatin1( "2:ab,3:x,1:cd," ) << NoError;


  dict.clear();
  hash.clear();
  list.clear();

  Table table;

  QVariantList row;
  row.append( 1 );
  row.append( QLatin1String( "a" ) );
  table.append( row  );

  row.clear();
  row.append( 1 );
  row.append( QLatin1String( "b" ) );
  table.append( row );

  row.clear();
  row.append( 2 );
  row.append( QLatin1String( "a" ) );
  table.append( row );

  row.clear();
  row.append( 3 );
  row.append( QLatin1String( "c" ) );
  table.append( row );

  row.clear();
  row.append( 3 );
  row.append( QLatin1String( "d" ) );
  table.append( row );

  dict.insert( QLatin1String( "data" ), QVariant::fromValue( table ) );

  QTest::newRow( "regroup03" ) << QString::fromLatin1( "{% regroup data by 0 as grouped %}"
      "{% for group in grouped %}"
      "{{ group.grouper }}:"
      "{% for item in group.list %}"
      "{{ item.1 }}"
      "{% endfor %},"
      "{% endfor %}" ) << dict << QString::fromLatin1( "1:ab,2:a,3:cd," ) << NoError;
}

void TestDefaultTags::testIfChangedTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged01" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" ) << dict << QString::fromLatin1( "123" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 3 );
  QTest::newRow( "ifchanged02" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" ) << dict << QString::fromLatin1( "13" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 1 );
  QTest::newRow( "ifchanged03" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% endfor %}" ) << dict << QString::fromLatin1( "1" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 2 << 3 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 2 << 2 << 2 );
  QTest::newRow( "ifchanged04" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "122232" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 1 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged05" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "1123123123" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 1 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 2 << 2 << 2 );
  QTest::newRow( "ifchanged06" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "1222" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 1 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 2 << 2 << 2 );
  dict.insert( QLatin1String( "numy" ), QVariantList() << 3 << 3 << 3 );
  QTest::newRow( "ifchanged07" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged %}{{ n }}{% endifchanged %}{% for x in numx %}{% ifchanged %}{{ x }}{% endifchanged %}{% for y in numy %}{% ifchanged %}{{ y }}{% endifchanged %}{% endfor %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "1233323332333" ) << NoError;
  QTest::newRow( "ifchanged08" ) << QString::fromLatin1( "{% ifchanged %}{{ num.0 }}{% endifchanged %}" ) << dict << QString::fromLatin1( "1" ) << NoError;

  // datalist': [[(1, 'a'), (1, 'a'), (0, 'b'), (1, 'c')], [(0, 'a'), (1, 'c'), (1, 'd'), (1, 'd'), (0, 'e')]]}
  dict.clear();
  QVariantList list;
  QVariantList innerList;
  QVariantList tuple;
  tuple << 1 << QString::fromLatin1( "a" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << QString::fromLatin1( "a" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 0 << QString::fromLatin1( "b" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << QString::fromLatin1( "c" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  list.append( QVariant( innerList ) );
  innerList.clear();

  tuple << 0 << QString::fromLatin1( "a" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << QString::fromLatin1( "c" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << QString::fromLatin1( "d" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 1 << QString::fromLatin1( "d" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  tuple << 0 << QString::fromLatin1( "e" );
  innerList.append( QVariant( tuple ) );
  tuple.clear();
  list.append( QVariant( innerList ) );
  innerList.clear();

  dict.insert( QLatin1String( "datalist" ), list );
  QTest::newRow( "ifchanged08" ) << QString::fromLatin1( "{% for data in datalist %}{% for c,d in data %}{% if c %}{% ifchanged %}{{ d }}{% endifchanged %}{% endif %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "accd" ) << NoError;

// Test one parameter given to ifchanged.
  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 2 << 3 );
  QTest::newRow( "ifchanged-param01" ) << QString::fromLatin1( "{% for n in num %}{% ifchanged n %}..{% endifchanged %}{{ n }}{% endfor %}" ) << dict << QString::fromLatin1( "..1..2..3" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 2 << 3 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 5 << 6 << 7 );
  QTest::newRow( "ifchanged-param02" ) << QString::fromLatin1( "{% for n in num %}{% for x in numx %}{% ifchanged n %}..{% endifchanged %}{{ x }}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "..567..567..567" ) << NoError;

// Test multiple parameters to ifchanged.

  dict.clear();
  dict.insert( QLatin1String( "num" ), QVariantList() << 1 << 1 << 2 );
  dict.insert( QLatin1String( "numx" ), QVariantList() << 5 << 6 << 6 );
  QTest::newRow( "ifchanged-param03" ) << QString::fromLatin1( "{% for n in num %}{{ n }}{% for x in numx %}{% ifchanged x n %}{{ x }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "156156256" ) << NoError;

// Test a date+hour like construct, where the hour of the last day
// is the same but the date had changed, so print the hour anyway.

  dict.clear();
  QVariantList days;
  QVariantHash hash;
  hash.insert( QLatin1String( "day" ), 1 );
  hash.insert( QLatin1String( "hours" ), QVariantList() << 1 << 2 << 3 );
  days << hash;
  hash.clear();
  hash.insert( QLatin1String( "day" ), 2 );
  hash.insert( QLatin1String( "hours" ), QVariantList() << 3 );
  days << hash;
  dict.insert( QLatin1String( "days" ), days );
  QTest::newRow( "ifchanged-param04" ) << QString::fromLatin1( "{% for d in days %}{% ifchanged %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "112323" ) << NoError;


// Logically the same as above, just written with explicit
// ifchanged for the day.

  QTest::newRow( "ifchanged-param05" ) << QString::fromLatin1( "{% for d in days %}{% ifchanged d.day %}{{ d.day }}{% endifchanged %}{% for h in d.hours %}{% ifchanged d.day h %}{{ h }}{% endifchanged %}{% endfor %}{% endfor %}" ) << dict << QString::fromLatin1( "112323" ) << NoError;

// Test the else clause of ifchanged.
  dict.clear();
  dict.insert( QLatin1String( "ids" ), QVariantList() << 1 << 1 << 2 << 2 << 2 << 3 );
  QTest::newRow( "ifchanged-else01" ) << QString::fromLatin1( "{% for id in ids %}{{ id }}{% ifchanged id %}-first{% else %}-other{% endifchanged %},{% endfor %}" ) << dict << QString::fromLatin1( "1-first,1-other,2-first,2-other,2-other,3-first," ) << NoError;
  QTest::newRow( "ifchanged-else02" ) << QString::fromLatin1( "{% for id in ids %}{{ id }}-{% ifchanged id %}{% cycle red,blue %}{% else %}grey{% endifchanged %},{% endfor %}" ) << dict << QString::fromLatin1( "1-red,1-grey,2-blue,2-grey,2-grey,3-red," ) << NoError;
  QTest::newRow( "ifchanged-else03" ) << QString::fromLatin1( "{% for id in ids %}{{ id }}{% ifchanged id %}-{% cycle red,blue %}{% else %}{% endifchanged %},{% endfor %}" ) << dict << QString::fromLatin1( "1-red,1,2-blue,2,2,3-red," ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "ids" ), QVariantList() << 1 << 1 << 2 << 2 << 2 << 3 << 4 );
  QTest::newRow( "ifchanged-else04" ) << QString::fromLatin1( "{% for id in ids %}{% ifchanged %}***{{ id }}*{% else %}...{% endifchanged %}{{ forloop.counter }}{% endfor %}" ) << dict << QString::fromLatin1( "***1*1...2***2*3...4...5***3*6***4*7" ) << NoError;

}


void TestDefaultTags::testAutoescapeTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "autoescape-tag01" ) << QString::fromLatin1( "{% autoescape off %}hello{% endautoescape %}" ) << dict << QString::fromLatin1( "hello" ) << NoError;

  dict.insert( QLatin1String( "first" ), QLatin1String( "<b>hello</b>" ) );
  QTest::newRow( "autoescape-tag02" ) << QString::fromLatin1( "{% autoescape off %}{{ first }}{% endautoescape %}" ) << dict << QString::fromLatin1( "<b>hello</b>" ) << NoError;
  QTest::newRow( "autoescape-tag03" ) << QString::fromLatin1( "{% autoescape on %}{{ first }}{% endautoescape %}" ) << dict << QString::fromLatin1( "&lt;b&gt;hello&lt;/b&gt;" ) << NoError;
  // Autoescape disabling and enabling nest in a predictable way.
  dict.insert( QLatin1String( "first" ), QLatin1String( "<a>" ) );
  QTest::newRow( "autoescape-tag04" ) << QString::fromLatin1( "{% autoescape off %}{{ first }} {% autoescape  on%}{{ first }}{% endautoescape %}{% endautoescape %}" ) << dict << QString::fromLatin1( "<a> &lt;a&gt;" ) << NoError;

  dict.insert( QLatin1String( "first" ), QLatin1String( "<b>first</b>" ) );
  QTest::newRow( "autoescape-tag05" ) << QString::fromLatin1( "{% autoescape on %}{{ first }}{% endautoescape %}" ) << dict << QString::fromLatin1( "&lt;b&gt;first&lt;/b&gt;" ) << NoError;
  // Strings (ASCII or unicode) already marked as "safe" are not
  // auto-escaped
  SafeString safeString( QLatin1String( "<b>first</b>" ) );
  QVariant safeStringVar = QVariant::fromValue<SafeString>( markSafe( safeString ) );
  dict.insert( QLatin1String( "first" ), safeStringVar );

  QTest::newRow( "autoescape-tag06" ) << QString::fromLatin1( "{{ first }}" ) << dict << QString::fromLatin1( "<b>first</b>" ) << NoError;
  QTest::newRow( "autoescape-tag07" ) << QString::fromLatin1( "{% autoescape on %}{{ first }}{% endautoescape %}" ) << dict << QString::fromLatin1( "<b>first</b>" ) << NoError;

  // Literal string arguments to filters, if used in the result, are
  // safe.
  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariant() );
  QTest::newRow( "autoescape-tag08" ) << "{% autoescape on %}{{ var|default_if_none:\"endquote\\\" hah\" }}{% endautoescape %}" << dict << "endquote\" hah" << NoError;

  QTest::newRow( "autoescape-tag09" ) << "{% autoescape on extra %}{{ var|default_if_none:\"endquote\\\" hah\" }}{% endautoescape %}" << dict << "" << TagSyntaxError;
  QTest::newRow( "autoescape-tag10" ) << "{% autoescape bad %}{{ var|default_if_none:\"endquote\\\" hah\" }}{% endautoescape %}" << dict << "" << TagSyntaxError;

  // Objects which return safe strings as their __unicode__ method
  // won't get double-escaped.
//   'autoescape-tag09': (r'{{ unsafe }}', {'unsafe': filters.UnsafeClass()}, 'you &amp; me'),
//   'autoescape-tag10': (r'{{ safe }}', {'safe': filters.SafeClass()}, 'you &gt; me'),
  // The "safe" and "escape" filters cannot work due to internal
  // implementation details (fortunately, the (no)autoescape block
  // tags can be used in those cases)
  dict.clear();
  dict.insert( QLatin1String( "first" ), QLatin1String( "<a>" ) );
  QTest::newRow( "autoescape-filtertag01" ) << QString::fromLatin1( "{{ first }}{% filter safe %}{{ first }} x<y{% endfilter %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "autoescape-filtertag02" ) << QString::fromLatin1( "{{ first }}{% filter escape %}{{ first }} x<y{% endfilter %}" ) << dict << QString() << TagSyntaxError;
}


void TestDefaultTags::testMediaFinderTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;
  QTest::newRow( "media_finder-tag01" ) << "{% media_finder \"existing_image.png\" %}" << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;
  QTest::newRow( "media_finder-tag02" ) << "{% media_finder \"does_not_exist.png\" %}" << dict << QString() << NoError;
  QTest::newRow( "media_finder-tag03" ) << "{% media_finder \"existing_image.png\" \"does_not_exist.png\" %}" << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;

  dict.insert( QLatin1String( "existing_img" ), QLatin1String( "existing_image.png" ) );
  dict.insert( QLatin1String( "nonexisting_img" ), QLatin1String( "does_not_exist.png" ) );

  QTest::newRow( "media_finder-tag04" ) << QString::fromLatin1( "{% media_finder %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "media_finder-tag05" ) << QString::fromLatin1( "{% media_finder existing_img %}" ) << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;
  QTest::newRow( "media_finder-tag06" ) << QString::fromLatin1( "{% media_finder nonexisting_img %}" ) << dict << QString() << NoError;
  QTest::newRow( "media_finder-tag07" ) << "{% media_finder \"does_not_exist.png\" existing_img %}" << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;
  QTest::newRow( "media_finder-tag08" ) << QString::fromLatin1( "{% media_finder nonexisting_img existing_img %}" ) << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;
  QTest::newRow( "media_finder-tag09" ) << "{% media_finder \"existing_image.png\" \"another_existing_image.png\" %}" << dict << QString::fromLatin1( "file:///path/to/existing_image.png" ) << NoError;
  QTest::newRow( "media_finder-tag10" ) << "{% media_finder \"another_existing_image.png\" \"existing_image.png\" %}" << dict << QString::fromLatin1( "file:///path/to/another_existing_image.png" ) << NoError;

  dict.insert( QLatin1String( "this_and_that_img" ), QLatin1String( "this&that.png" ) );

  QTest::newRow( "media_finder-tag11" ) << "{% media_finder \"this&that.png\" %}" << dict << QString::fromLatin1( "file:///path/to/this&amp;that.png" ) << NoError;
  QTest::newRow( "media_finder-tag12" ) << "{% media_finder this_and_that_img %}" << dict << QString::fromLatin1( "file:///path/to/this&amp;that.png" ) << NoError;
  QTest::newRow( "media_finder-tag13" ) << "{% autoescape off %}{% media_finder \"this&that.png\" %}{% endautoescape %}" << dict << QString::fromLatin1( "file:///path/to/this&that.png" ) << NoError;
  QTest::newRow( "media_finder-tag14" ) << "{% autoescape off %}{% media_finder this_and_that_img %}{% endautoescape %}" << dict << QString::fromLatin1( "file:///path/to/this&that.png" ) << NoError;


}

void TestDefaultTags::testRangeTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "range-tag01" ) << QString::fromLatin1( "{% range 5 as i %}{{ i }};{% endrange %}" ) << dict << QString::fromLatin1( "0;1;2;3;4;" ) << NoError;
  QTest::newRow( "range-tag02" ) << QString::fromLatin1( "{% range 1 6 as i %}{{ i }};{% endrange %}" ) << dict << QString::fromLatin1( "1;2;3;4;5;" ) << NoError;
  QTest::newRow( "range-tag03" ) << QString::fromLatin1( "{% range 5 26 5 as i %}{{ i }};{% endrange %}" ) << dict << QString::fromLatin1( "5;10;15;20;25;" ) << NoError;

  QVariantList list;
  list << 10 << 15 << 2;
  dict.insert( QLatin1String( "values" ), list );

  QTest::newRow( "range-tag04" ) << QString::fromLatin1( "{% range values.0 values.1 values.2 as i %}{{ i }};{% endrange %}" ) << dict << QString::fromLatin1( "10;12;14;" ) << NoError;

  QTest::newRow( "range-tag05" ) << QString::fromLatin1( "{% range 5 %}Foo;{% endrange %}" ) << dict << QString::fromLatin1( "Foo;Foo;Foo;Foo;Foo;" ) << NoError;
  QTest::newRow( "range-tag06" ) << QString::fromLatin1( "{% range 5 6 %}Foo;{% endrange %}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "range-tag07" ) << QString::fromLatin1( "{% range 5 6 7 %}Foo;{% endrange %}" ) << dict << QString() << TagSyntaxError;
}

void TestDefaultTags::testDebugTag_data()
{

  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "debug-tag01" ) << QString::fromLatin1( "{% debug %}" ) << dict << QString::fromLatin1( "\n\nContext:\nEnd context:\n\n" ) << NoError;
  dict.insert( QLatin1String( "answer" ), 42 );
  QTest::newRow( "debug-tag02" ) << QString::fromLatin1( "{% debug %}" ) << dict << QString::fromLatin1( "\n\nContext:\nkey answer, type int\nEnd context:\n\n" ) << NoError;
}

void TestDefaultTags::testLoadTag_data()
{

  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "load-tag01" ) << QString::fromLatin1( "{% load does_not_exist %}foo" ) << dict << QString() << TagSyntaxError;
}

void TestDefaultTags::testUrlTypes_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QPair<QString, QString> >( "output" );

  Dict dict;
  QTest::newRow( "url-types01" ) << "{% media_finder \"existing_image.png\" %}" << dict
                                 << qMakePair( QString::fromLatin1( "file:///path/to/" ), QString::fromLatin1( "existing_image.png" ) );

  QTest::newRow( "url-types02" ) << "{% media_finder \"does_not_exist.png\" %}" << dict
                                 << qMakePair( QString(), QString() );

  dict.insert( QLatin1String( "existing_img" ), QLatin1String( "existing_image.png" ) );
  dict.insert( QLatin1String( "nonexisting_img" ), QLatin1String( "does_not_exist.png" ) );

  QTest::newRow( "url-types03" ) << QString::fromLatin1( "{% media_finder existing_img %}" ) << dict
                                 << qMakePair( QString::fromLatin1( "file:///path/to/" ), QString::fromLatin1( "existing_image.png" ) );


  QTest::newRow( "url-types04" ) << QString::fromLatin1( "{% media_finder nonexisting_img %}" ) << dict
                                 << qMakePair( QString(), QString() );
}

void TestDefaultTags::testUrlTypes()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );
  QFETCH( StringPair, output );

  Template t = m_engine->newTemplate( input, QLatin1String( QTest::currentDataTag() ) );
  QVERIFY( t->error() == NoError );
  Context c( dict );
  QString result = t->render( &c );
  QVERIFY( t->error() == NoError );
  QVERIFY( result == output.first + output.second );

  c.setUrlType( Context::RelativeUrls );
  result = t->render( &c );
  QVERIFY( t->error() == NoError );
  QVERIFY( result == output.second );
}

void TestDefaultTags::testRelativePaths_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );

  Dict dict;
  QTest::newRow( "relativepaths01" ) << "{% media_finder \"existing_image.png\" %}" << dict << QString::fromLatin1( "existing_image.png" );

  QTest::newRow( "relativepaths02" ) << "{% media_finder \"does_not_exist.png\" %}" << dict << QString();

  dict.insert( QLatin1String( "existing_img" ), QLatin1String( "existing_image.png" ) );
  dict.insert( QLatin1String( "nonexisting_img" ), QLatin1String( "does_not_exist.png" ) );

  QTest::newRow( "relativepaths03" ) << QString::fromLatin1( "{% media_finder existing_img %}" ) << dict << QString::fromLatin1( "existing_image.png" );


  QTest::newRow( "relativepaths04" ) << QString::fromLatin1( "{% media_finder nonexisting_img %}" ) << dict << QString();
}

void TestDefaultTags::testRelativePaths()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );
  QFETCH( QString, output );

  Template t = m_engine->newTemplate( input, QLatin1String( QTest::currentDataTag() ) );
  QVERIFY( t->error() == NoError );
  Context c( dict );
  QString result = t->render( &c );
  QVERIFY( t->error() == NoError );
  if ( !output.isEmpty() )
    QVERIFY( result == QLatin1String( "file:///path/to/" ) + output );
  else
    QVERIFY( result.isEmpty() );

  c.setUrlType( Context::RelativeUrls );
  QString relativePath = QLatin1String( "relative/path" );
  c.setRelativeMediaPath( relativePath );
  result = t->render( &c );
  QVERIFY( t->error() == NoError );
  if ( !output.isEmpty() )
    QVERIFY( result == relativePath + QLatin1Char( '/' ) + output );
  else
    QVERIFY( result.isEmpty() );
}



QTEST_MAIN( TestDefaultTags )
#include "testdefaulttags.moc"

#endif

