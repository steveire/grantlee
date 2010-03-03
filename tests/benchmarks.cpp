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

#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "lexer_p.h"
#include "template.h"
#include "engine.h"
#include "context.h"
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
  QString getTemplate( int size );
  Engine *m_engine;
  QString m_templateGeneratorString;
  TemplateImpl *m_templateGenerator;

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

  m_templateGenerator = Engine::instance()->newTemplate( m_templateGeneratorString, this );

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

  QBENCHMARK { p.setTokens( tokens ); list = p.parse(); }

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

  list = p.parse();

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

  // Using Grantlee to create Grantlee templates. How recursive...

//   QList<int> repeatSizes;
//   repeatSizes << 10 << 100 << 200 << 300 << 1000 << 1500 << 2000 << 3000 << 4000;

//   foreach(int size, repeatSizes)
//   {
//     const char * name = QString("growing-%1").arg( size ).toLatin1();
//     QTest::newRow( name ) << getTemplate( size ) << dict;
//   }

  QTest::newRow( "growing-10"   ) << getTemplate( 10   ) << dict;
  QTest::newRow( "growing-20"   ) << getTemplate( 20   ) << dict;
  QTest::newRow( "growing-30"   ) << getTemplate( 30   ) << dict;
  QTest::newRow( "growing-100"  ) << getTemplate( 100  ) << dict;
  QTest::newRow( "growing-300"  ) << getTemplate( 300  ) << dict;
  QTest::newRow( "growing-700"  ) << getTemplate( 700  ) << dict;
  QTest::newRow( "growing-1000" ) << getTemplate( 1000 ) << dict;
  QTest::newRow( "growing-2000" ) << getTemplate( 2000 ) << dict;
  QTest::newRow( "growing-2500" ) << getTemplate( 2500 ) << dict;
  QTest::newRow( "growing-2700" ) << getTemplate( 2700 ) << dict;
  QTest::newRow( "growing-3000" ) << getTemplate( 3000 ) << dict;
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

QString Benchmarking::getTemplate( int size )
{
  Context c( getDictData( size ) );
  return m_templateGenerator->render( &c );
}


QTEST_MAIN( Benchmarking )
#include "benchmarktokenizing.moc"

#endif

