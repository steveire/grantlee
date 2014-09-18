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

#include <QtTest/QTest>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "filterexpression.h"
#include "grantlee_paths.h"
#include "coverageobject.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestScriptableTagsSyntax : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() {
    doTest();
  }

  void testResolve_data();
  void testResolve() {
    doTest();
  }

  void cleanupTestCase();

private:

  void doTest();

  Engine *m_engine;

};

void TestScriptableTagsSyntax::initTestCase()
{
  Q_INIT_RESOURCE(testresource);

  m_engine = new Engine( this );
  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH )
                                          << QString::fromLatin1( ":/plugins/" ) // For scripteddefaults.qs
                         );
  m_engine->addDefaultLibrary( QLatin1String( "grantlee_scriptabletags" ) );
}

void TestScriptableTagsSyntax::cleanupTestCase()
{
  delete m_engine;
}

void TestScriptableTagsSyntax::doTest()
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


  QCOMPARE( t->error(), NoError );

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE( NoError, error );

  QCOMPARE( t->error(), NoError );


  QCOMPARE( result, output );
}

void TestScriptableTagsSyntax::testBasicSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  dict.insert( QLatin1String( "boo" ), QLatin1String( "Far" ) );
  dict.insert( QLatin1String( "booList" ), QVariantList() << QString::fromLatin1( "Tom" ) << QString::fromLatin1( "Dick" ) << QString::fromLatin1( "Harry" ) );

  QTest::newRow( "scriptable-tags01" ) << "{% load scripteddefaults %}{% if2 \"something\\\" stupid\" %}{{ boo }}{% endif2 %}" << dict << QString::fromLatin1( "Far" ) << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow( "scriptable-tags02" ) << "{% load scripteddefaults %}{% if2 \"something\\\" stupid\" %}{% for name in booList %}:{{ name }};{% endfor %}{% endif2 %}" << dict << QString::fromLatin1( ":Tom;:Dick;:Harry;" ) << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow( "scriptable-tags03" ) << QString::fromLatin1( "{% load scripteddefaults %}{% if2 boo %}yes{% else %}no{% endif2 %}" ) << dict << QString::fromLatin1( "yes" ) << NoError;
  QTest::newRow( "scriptable-tags04" ) << QString::fromLatin1( "{% load scripteddefaults %}{% if2 foo %}yes{% else %}no{% endif2 %}" ) << dict << QString::fromLatin1( "no" ) << NoError;

  QTest::newRow( "scriptable-tags05" ) << QString::fromLatin1( "{% load scripteddefaults %}{{ boo|upper }}" ) << dict << QString::fromLatin1( "FAR" ) << NoError;


  dict.insert( QLatin1String( "boo" ), QLatin1String( "Far & away" ) );
  // Variables are escaped ...
  QTest::newRow( "scriptable-tags06" ) << QString::fromLatin1( "{% load scripteddefaults %}{{ boo }}" ) << dict << QString::fromLatin1( "Far &amp; away" ) << NoError;
  // and scripted filters can mark output as 'safe'.
  QTest::newRow( "scriptable-tags07" ) << QString::fromLatin1( "{% load scripteddefaults %}{{ boo|safe2 }}" ) << dict << QString::fromLatin1( "Far & away" ) << NoError;

  // Filters can take arguments
  QTest::newRow( "scriptable-tags08" ) << "{% load scripteddefaults %}{{ booList|join2:\" \" }}" << dict << QString::fromLatin1( "Tom Dick Harry" ) << NoError;

  // Literals in tags are compared un-escaped.
  QTest::newRow( "scriptable-tags09" ) << "{% load scripteddefaults %}{% ifequal2 boo \"Far & away\" %}yes{% endifequal2 %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  // Literals in arguments to filters are not escaped.
  QTest::newRow( "scriptable-tags10" ) << "{% load scripteddefaults %}{{ booList|join2:\" & \" }}" << dict << QString::fromLatin1( "Tom & Dick & Harry" ) << NoError;

  // Nor are variables.
  dict.insert( QLatin1String( "amp" ), QLatin1String( " & " ) );
  QTest::newRow( "scriptable-tags11" ) << QString::fromLatin1( "{% load scripteddefaults %}{{ booList|join2:amp }}" ) << dict << QString::fromLatin1( "Tom & Dick & Harry" ) << NoError;

  QTest::newRow( "scriptable-load-error01" ) << QString::fromLatin1( "{% load %}{{ booList|join2:amp }}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
}

void TestScriptableTagsSyntax::testResolve_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;
  dict.insert( QLatin1String( "boo" ), QLatin1String( "Far" ) );
  dict.insert( QLatin1String( "zing" ), QLatin1String( "Bang" ) );

  QTest::newRow( "resolve-01" ) << "{% load scripteddefaults %}{% resolver boo zing %}" << dict << QString::fromLatin1( "Far - Bang" ) << NoError;
}


QTEST_MAIN( TestScriptableTagsSyntax )
#include "testscriptabletags.moc"

#endif
