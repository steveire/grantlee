/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MUTABLETAGSTEST_H
#define MUTABLETAGSTEST_H

#include <QtTest>
#include <QtCore/QObject>

#include "mutabletemplate.h"
#include "engine.h"
#include "context.h"
#include "grantlee_paths.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestMutableTagsSyntax : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testRawTag0();
  void testRawTag();
  void testRepeaterTag();
  void testMultiRepeater();

  void cleanupTestCase();

private:
  Engine *m_engine;

};

void TestMutableTagsSyntax::initTestCase()
{
  m_engine = new Engine( this );
  m_engine->setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  m_engine->addDefaultLibrary( QLatin1String( "grantlee_mutabletags" ) );
}

void TestMutableTagsSyntax::cleanupTestCase()
{
  delete m_engine;
}

void TestMutableTagsSyntax::testRawTag0()
{

  QVariantHash dict;

  dict.insert( QLatin1String( "var" ), QLatin1String( "Far" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Tom" ) << QLatin1String( "Dick" ) << QLatin1String( "Harry" ) );

  QString content = QLatin1String( "Begin"
                    "{% raw %} Stuff {% endraw %}"
                    "Afters." );
  Template t = m_engine->newMutableTemplate( content, QLatin1String( "t1" ) );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = QLatin1String( "Begin Stuff Afters." );
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "SPAM" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Larry" ) << QLatin1String( "Moe" ) << QLatin1String( "Curly" ) );

  Context c2( dict );
  result = t->render( &c2 );
  QCOMPARE( result, expected );

}

void TestMutableTagsSyntax::testRawTag()
{

  QVariantHash dict;

  dict.insert( QLatin1String( "var" ), QLatin1String( "Far" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Tom" ) << QLatin1String( "Dick" ) << QLatin1String( "Harry" ) );


  QString content = QLatin1String( "Begin {% for name in nameList %}{{ name }},{% endfor %}"
                    "{% raw %}{% for name in nameList %}{{ name }},{% endfor %} var: {{ var }}. {% endraw %}"
                    "Afters." );
  Template t = m_engine->newMutableTemplate( content, QChar::fromLatin1( 't' ) );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = QLatin1String( "Begin Tom,Dick,Harry,Tom,Dick,Harry, var: Far. Afters." );
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "SPAM" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Larry" ) << QLatin1String( "Moe" ) << QLatin1String( "Curly" ) );

  Context c2( dict );
  result = t->render( &c2 );
  expected = QLatin1String( "Begin Tom,Dick,Harry,Larry,Moe,Curly, var: SPAM. Afters." );
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "EGGS" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "John" ) << QLatin1String( "Michael" ) << QLatin1String( "Terry" ) );

  Context c3( dict );
  result = t->render( &c3 );
  expected = QLatin1String( "Begin Tom,Dick,Harry,John,Michael,Terry, var: EGGS. Afters." );
  QCOMPARE( result, expected );

}

void TestMutableTagsSyntax::testRepeaterTag()
{
  QVariantHash dict;

  dict.insert( QLatin1String( "var" ), QLatin1String( "Far" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Tom" ) << QLatin1String( "Dick" ) << QLatin1String( "Harry" ) );

  QString content = QLatin1String( "{% for name in nameList %}{{ name }},{% endfor %}"
                    "{% repeater %}{% for name in nameList %}{{ name }},{% endfor %} var: {{ var }}. {% endrepeater %}"
                    "Afters" );

  Template t = m_engine->newMutableTemplate( content, QChar::fromLatin1( 't' ) );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = QLatin1String( "Tom,Dick,Harry,"
                     "Tom,Dick,Harry, var: Far. "
                     "Afters" );
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "SPAM" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "Larry" ) << QLatin1String( "Moe" ) << QLatin1String( "Curly" ) );

  Context c2( dict );
  result = t->render( &c2 );
  expected = QLatin1String( "Tom,Dick,Harry,"
             "Tom,Dick,Harry, var: Far. "
             "Larry,Moe,Curly, var: SPAM. "
             "Afters" );
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "EGGS" ) );
  dict.insert( QLatin1String( "nameList" ), QVariantList() << QLatin1String( "John" ) << QLatin1String( "Michael" ) << QLatin1String( "Terry" ) );

  Context c3( dict );
  result = t->render( &c3 );
  expected = QLatin1String( "Tom,Dick,Harry,"
             "Tom,Dick,Harry, var: Far. "
             "Larry,Moe,Curly, var: SPAM. "
             "John,Michael,Terry, var: EGGS. "
             "Afters" );
  QCOMPARE( result, expected );
}

void TestMutableTagsSyntax::testMultiRepeater()
{

  QString content = QLatin1String( "Before."
                    "{% repeater %}"
                    "Foo."
                    "{% endrepeater %}"
                    "Middle."
                    "{% repeater %}"
                    "Bar."
                    "{% endrepeater %}"
                    "After." );

  Template t = m_engine->newMutableTemplate( content, QChar::fromLatin1( 't' ) );

  QVariantHash h;
  Context c( h );
  QString result = t->render( &c );
  QString expected = QLatin1String( "Before.Foo.Middle.Bar.After." );
  QCOMPARE( result, expected );

  content = QLatin1String( "Before."
            "{% repeater %}"
            "Foo."
            "{% endrepeater %}"
            "Middle.{{ var }}."
            "{% repeater %}"
            "Bar."
            "{% endrepeater %}"
            "After." );
  Template t2 = m_engine->newMutableTemplate( content, QLatin1String( "t2" ) );

  h.insert( QLatin1String( "var" ), QLatin1String( "String" ) );
  Context c2( h );
  QString result2 = t2->render( &c2 );
  QString expected2 = QLatin1String( "Before.Foo.Middle.String.Bar.After." );
  QCOMPARE( result2, expected2 );
}

QTEST_MAIN( TestMutableTagsSyntax )
#include "testmutabletags.moc"

#endif

