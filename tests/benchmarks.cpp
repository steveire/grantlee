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

#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "lexer_p.h"
#include "template.h"
#include "engine.h"
#include "context.h"
#include "grantlee.h"
#include "filterexpression.h"
#include "parser.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class Benchmarking : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testTokenizing_data() {
    getData();
  }
  void testTokenizing();

  void testParsing_data() {
    getData();
  }
  void testParsing();

  void testRendering_data() {
    getData();
  }
  void testRendering();

  void cleanupTestCase();

private:

  void getData();
  QVariantHash getDictData( int size );

  Engine *m_engine;
  QString m_templateGeneratorString;
  Template *m_templateGenerator;

};

void Benchmarking::initTestCase()
{
  m_engine = Engine::instance();

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginDirs( QStringList() << appDirPath + "/grantlee_defaulttags/"
                           << appDirPath + "/grantlee_defaultfilters/"
                           << appDirPath + "/grantlee_scriptabletags/" );

  m_templateGeneratorString =
    "Lorem {% for i in items %}"
    " Ipsum {% templatetag openblock %} if boo {% templatetag closeblock %} "
    "bar {% templatetag openvariable %} bat|upper {% templatetag closevariable %} baz {{ i }} dolor"
    " {% templatetag openblock %} endif {% templatetag closeblock %} sit."
    "{% endfor %} amet.\n";

  m_templateGenerator = new Template( this );

  m_templateGenerator->setContent( m_templateGeneratorString );

}

void Benchmarking::cleanupTestCase()
{
  delete m_engine;
}

void Benchmarking::testTokenizing()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );

  Lexer l( input );

  QList<Token> tokens;
  QBENCHMARK( tokens = l.tokenize() );
}

void Benchmarking::testParsing()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );

  Lexer l( input );

  QList<Token> tokens;
  tokens = l.tokenize();

  Parser p( tokens, 0 );
  NodeList list;

  QBENCHMARK( list = p.parse( this ) );

}

void Benchmarking::testRendering()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );

  Lexer l( input );

  QList<Token> tokens;
  tokens = l.tokenize();

  Parser p( tokens, 0 );
  NodeList list;

  list = p.parse( this );

  Context context( dict );

  QString output;

  QBENCHMARK( output = list.render( &context ) );

}

void Benchmarking::getData()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );

  Dict dict;

  dict.insert( "boo", "Far" );
  dict.insert( "bat", "Cat" );
  dict.insert( "booList", QVariantList() << "Tom" << "Dick" << "Harry" );

  QString foo;

  // Using Grantlee to create Grantlee templates. How recursive...

  Context c( getDictData( 10 ) );
  QTest::newRow( "growing-10" ) << m_templateGenerator->render( &c ) << dict;

  Context c2( getDictData( 100 ) );
  QTest::newRow( "growing-100" ) << m_templateGenerator->render( &c2 ) << dict;

  Context c3( getDictData( 1000 ) );
  QTest::newRow( "growing-1000" ) << m_templateGenerator->render( &c3 ) << dict;

  Context c4( getDictData( 10000 ) );
  QTest::newRow( "growing-10000" ) << m_templateGenerator->render( &c4 ) << dict;

  Context c5( getDictData( 20000 ) );
  QTest::newRow( "growing-20000" ) << m_templateGenerator->render( &c5 ) << dict;

  dict.clear();
}

QVariantHash Benchmarking::getDictData( int size )
{
  QVariantHash h;
  QVariantList list;

  for ( int i = 0 ; i < size; i++ )
    list << i;
  h.insert( "items", list );

  return h;
}

QTEST_MAIN( Benchmarking )
#include "benchmarktokenizing.moc"

#endif

