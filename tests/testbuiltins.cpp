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

#ifndef BUILTINSTEST_H
#define BUILTINSTEST_H

#include <QtTest>
#include <QtCore/QObject>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "filterexpression.h"
#include "grantlee_paths.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

/**
  For use with tests.
*/
class OtherClass : public QObject
{
  Q_OBJECT
  Q_PROPERTY( QString method READ method )
  Q_PROPERTY( Animals animals READ animals )
public:
  OtherClass( QObject *parent = 0 )
      : QObject( parent ) { }

  enum Animals
  {
    Lions,
    Tigers,
    Bears
  };

  Q_ENUMS(Animals)
  Animals animals() const { return Tigers; }

  QString method() const {
    return QLatin1String( "OtherClass::method" );
  }

};

/**
  For use with tests.
*/
class SomeClass : public QObject
{
  Q_OBJECT
  Q_PROPERTY( QString method READ method )
  Q_PROPERTY( QVariant otherClass READ otherClass )

public:
  enum FirstEnum
  {
    Employee,
    Employer,
    Manager
  };

  enum SecondEnum
  {
    Voter = 2,
    Consumer = 4,
    Citizen = 8
  };

  Q_ENUMS(FirstEnum SecondEnum)

  SomeClass( QObject *parent = 0 )
      : QObject( parent ), m_other( new OtherClass( this ) ) { }

  QString method() const {
    return QLatin1String( "SomeClass::method" );
  }

  QVariant otherClass() const {
    return QVariant::fromValue( m_other );
  }

  QString nonAccessibleMethod( const QString &str ) {
    return str;
  }

private:
  QObject *m_other;

};

class NoEscapeOutputStream : public OutputStream
{
public:
  NoEscapeOutputStream()
    : OutputStream()
  {

  }

  NoEscapeOutputStream(QTextStream* stream)
    : OutputStream( stream )
  {

  }

  virtual QSharedPointer< OutputStream > clone() const {
    return QSharedPointer<OutputStream>( new NoEscapeOutputStream );
  }

  virtual QString escape( const QString& input ) const {
    return input;
  }
};

class JSOutputStream : public OutputStream
{
public:
  JSOutputStream()
    : OutputStream()
  {

  }

  JSOutputStream(QTextStream* stream)
    : OutputStream( stream )
  {

  }

  virtual QSharedPointer< OutputStream > clone() const {
    return QSharedPointer<OutputStream>( new JSOutputStream );
  }

  virtual QString escape( const QString& input ) const {
    QList<QPair<QString, QString> > jsEscapes;
    jsEscapes << QPair<QString, QString>( QChar::fromLatin1( '\\' ), QLatin1String( "\\u005C" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '\'' ),  QLatin1String( "\\u0027" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '\"' ),  QLatin1String( "\\u0022" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '>' ), QLatin1String( "\\u003E" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '<' ), QLatin1String( "\\u003C" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '&' ), QLatin1String( "\\u0026" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '=' ), QLatin1String( "\\u003D" ) )
              << QPair<QString, QString>( QChar::fromLatin1( '-' ), QLatin1String( "\\u002D" ) )
              << QPair<QString, QString>( QChar::fromLatin1( ';' ), QLatin1String( "\\u003B" ) )
              << QPair<QString, QString>( QChar( 0x2028 ), QLatin1String( "\\u2028" ) )
              << QPair<QString, QString>( QChar( 0x2029 ), QLatin1String( "\\u2029" ) );

    for( int i = 0; i < 32; ++i )
    {
      jsEscapes << QPair<QString, QString>( QChar( i ), QLatin1String( "\\u00" ) + QString::fromLatin1( "%1" ).arg( i, 2, 16, QLatin1Char( '0' ) ).toUpper() );
    }

    QListIterator<QPair<QString, QString> > it( jsEscapes );
    QString retString = input;
    while ( it.hasNext() ) {
      QPair<QString, QString> escape = it.next();
      retString = retString.replace( escape.first, escape.second );
    }
    return retString;


  }
};

class TestBuiltinSyntax : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() {
    doTest();
  }

  void testEnums_data();
  void testEnums() {
    doTest();
  }

  void testListIndex_data();
  void testListIndex() {
    doTest();
  }

  void testFilterSyntax_data();
  void testFilterSyntax() {
    doTest();
  }

  void testCommentSyntax_data();
  void testCommentSyntax() {
    doTest();
  }

  void testMultiline_data();
  void testMultiline() {
    doTest();
  }

  void testEscaping_data();
  void testEscaping() {
    doTest();
  }

  void testTypeAccessors_data();
  void testTypeAccessors() {
    doTest();
  }

  void testMultipleStates();
  void testAlternativeEscaping();

  void testTemplatePathSafety_data();
  void testTemplatePathSafety();

  void testMediaPathSafety_data();
  void testMediaPathSafety();

  void cleanupTestCase();

private:
  Engine *m_engine;

  InMemoryTemplateLoader::Ptr m_loader;

  Engine* getEngine();

  void doTest();

};

void TestBuiltinSyntax::initTestCase()
{
  m_engine = getEngine();

  m_loader = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );
  m_engine->addTemplateLoader( m_loader );
}

Engine* TestBuiltinSyntax::getEngine()
{
  Engine *engine = new Engine( this );
  engine->setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );
  return engine;
}

void TestBuiltinSyntax::cleanupTestCase()
{
  delete m_engine;
}

void TestBuiltinSyntax::doTest()
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

  QCOMPARE( result, output );
}

void TestBuiltinSyntax::testBasicSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  // Plain text should go through the template parser untouched
  QTest::newRow( "basic-syntax01" ) << QString::fromLatin1( "something cool" ) << dict << QString::fromLatin1( "something cool" ) << NoError;

  // Variables should be replaced with their value in the current
  // context
  dict.insert( QLatin1String( "headline" ), QLatin1String( "Success" ) );
  QTest::newRow( "basic-syntax02" ) << QString::fromLatin1( "{{ headline }}" ) << dict << QString::fromLatin1( "Success" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "first" ), 1 );
  dict.insert( QLatin1String( "second" ), 2 );

  // More than one replacement variable is allowed in a template
  QTest::newRow( "basic-syntax03" ) << QString::fromLatin1( "{{ first }} --- {{ second }}" ) << dict << QString::fromLatin1( "1 --- 2" ) << NoError;

  dict.clear();
  // Fail silently when a variable is not found in the current context
  QTest::newRow( "basic-syntax04" ) << QString::fromLatin1( "as{{ missing }}df" ) << dict << QString::fromLatin1( "asdf" ) << NoError;

  // A variable may not contain more than one word
  QTest::newRow( "basic-syntax06" ) << QString::fromLatin1( "{{ multi word variable }}" ) << dict << QString() << TagSyntaxError;
  // Raise TemplateSyntaxError for empty variable tags
  QTest::newRow( "basic-syntax07" ) << QString::fromLatin1( "{{ }}" ) << dict << QString() << EmptyVariableError;
  QTest::newRow( "basic-syntax08" ) << QString::fromLatin1( "{{        }}" ) << dict << QString() << EmptyVariableError;


  // Attribute syntax allows a template to call an object's attribute

  QObject *someClass = new SomeClass( this );
  dict.insert( QLatin1String( "var" ), QVariant::fromValue( someClass ) );

  QTest::newRow( "basic-syntax09" ) << QString::fromLatin1( "{{ var.method }}" ) << dict << QString::fromLatin1( "SomeClass::method" ) << NoError;

  // Multiple levels of attribute access are allowed
  QTest::newRow( "basic-syntax10" ) << QString::fromLatin1( "{{ var.otherClass.method }}" ) << dict << QString::fromLatin1( "OtherClass::method" ) << NoError;

  // Fail silently when a variable's attribute isn't found
  QTest::newRow( "basic-syntax11" ) << QString::fromLatin1( "{{ var.blech }}" ) << dict << QString() << NoError;

  // TODO: Needed?
  // Raise TemplateSyntaxError when trying to access a variable beginning with an underscore
  // #C# {"var": SomeClass()}
  dict.clear();
  QVariantHash hash;
  hash.insert( QLatin1String( "__dict__" ), QLatin1String( "foo" ) );
  dict.insert( QLatin1String( "var" ), hash );
  QTest::newRow( "basic-syntax12" ) << QString::fromLatin1( "{{ var.__dict__ }}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  // Raise TemplateSyntaxError when trying to access a variable containing an illegal character
  QTest::newRow( "basic-syntax13" ) << QString::fromLatin1( "{{ va>r }}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "basic-syntax14" ) << QString::fromLatin1( "{{ (var.r) }}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "basic-syntax15" ) << QString::fromLatin1( "{{ sp%am }}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "basic-syntax16" ) << QString::fromLatin1( "{{ eggs! }}" ) << dict << QString() << TagSyntaxError;
  QTest::newRow( "basic-syntax17" ) << QString::fromLatin1( "{{ moo? }}" ) << dict << QString() << TagSyntaxError;

  // Attribute syntax allows a template to call a dictionary key's value

  hash.clear();
  hash.insert( QLatin1String( "bar" ), QLatin1String( "baz" ) );
  dict.insert( QLatin1String( "foo" ), hash );
  QTest::newRow( "basic-syntax18" ) << QString::fromLatin1( "{{ foo.bar }}" ) << dict << QString::fromLatin1( "baz" ) << NoError;

  // Fail silently when a variable's dictionary key isn't found
  QTest::newRow( "basic-syntax19" ) << QString::fromLatin1( "{{ foo.spam }}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariant::fromValue( someClass ) );
  // Fail silently when attempting to access an unavailable method
  QTest::newRow( "basic-syntax20" ) << QString::fromLatin1( "{{ var.nonAccessibleMethod }}" ) << dict << QString() << NoError;

  dict.clear();
  // Don't get confused when parsing something that is almost, but not
  // quite, a template tag.
  QTest::newRow( "basic-syntax21" ) << QString::fromLatin1( "a {{ moo %} b" ) << dict << QString::fromLatin1( "a {{ moo %} b" ) << NoError;
  QTest::newRow( "basic-syntax22" ) << QString::fromLatin1( "{{ moo #}" ) << dict << QString::fromLatin1( "{{ moo #}" ) << NoError;

  dict.insert( QLatin1String( "cow" ), QLatin1String( "cow" ) );
  // Will try to treat "moo #} {{ cow" as the variable. Not ideal, but
  // costly to work around, so this triggers an error.
  QTest::newRow( "basic-syntax23" ) << QString::fromLatin1( "{{ moo #} {{ cow }}" ) << dict << QString() << TagSyntaxError;

  dict.clear();
  // Embedded newlines make it not-a-tag.
  QTest::newRow( "basic-syntax24" ) << "{{ moo\n }}" << dict << "{{ moo\n }}" << NoError;
  // Literal strings are permitted inside variables, mostly for i18n
  // purposes.
  QTest::newRow( "basic-syntax25" ) << "{{ \"fred\" }}" << dict << QString::fromLatin1( "fred" ) << NoError;
  QTest::newRow( "basic-syntax26" ) << "{{ \"\\\"fred\\\"\" }}" << dict << "\"fred\"" << NoError;
  QTest::newRow( "basic-syntax27" ) << "{{ _(\"\\\"fred\\\"\") }}" << dict << "\"fred\"" << NoError;


  dict.clear();
  hash.clear();
  QVariantHash innerHash;
  innerHash.insert( QLatin1String( "3" ), QLatin1String( "d" ) );
  hash.insert( QLatin1String( "2" ), innerHash );
  dict.insert( QLatin1String( "1" ), hash );

  QTest::newRow( "basic-syntax28" ) << QString::fromLatin1( "{{ 1.2.3 }}" ) << dict << QString::fromLatin1( "d" ) << NoError;

  dict.clear();
  hash.clear();
  QVariantList list;
  list << QString::fromLatin1( "a" ) << QString::fromLatin1( "b" ) << QString::fromLatin1( "c" ) << QString::fromLatin1( "d" );
  hash.insert( QLatin1String( "2" ), list );
  dict.insert( QLatin1String( "1" ), hash );
  QTest::newRow( "basic-syntax29" ) << QString::fromLatin1( "{{ 1.2.3 }}" ) << dict << QString::fromLatin1( "d" ) << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "a" ) << QString::fromLatin1( "b" ) << QString::fromLatin1( "c" ) << QString::fromLatin1( "d" );
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "1" ), list );

  QTest::newRow( "basic-syntax30" ) << QString::fromLatin1( "{{ 1.2.3 }}" ) << dict << QString::fromLatin1( "d" ) << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" ) << QString::fromLatin1( "x" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" ) << QString::fromLatin1( "y" );
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << QString::fromLatin1( "a" ) << QString::fromLatin1( "b" ) << QString::fromLatin1( "c" ) << QString::fromLatin1( "d" );
  list.append( QVariant( innerList ) );
  dict.insert( QLatin1String( "1" ), list );

  QTest::newRow( "basic-syntax31" ) << QString::fromLatin1( "{{ 1.2.3 }}" ) << dict << QString::fromLatin1( "d" ) << NoError;

  dict.clear();
  list.clear();
  hash.clear();
  hash.insert( QLatin1String( "x" ), QLatin1String( "x" ) );
  list.append( hash );
  hash.clear();
  hash.insert( QLatin1String( "y" ), QLatin1String( "y" ) );
  list.append( hash );
  hash.clear();
  hash.insert( QLatin1String( "3" ), QLatin1String( "d" ) );
  list.append( hash );

  dict.insert( QLatin1String( "1" ), list );

  QTest::newRow( "basic-syntax32" ) << QString::fromLatin1( "{{ 1.2.3 }}" ) << dict << QString::fromLatin1( "d" ) << NoError;

  dict.clear();

  dict.insert( QLatin1String( "1" ), QLatin1String( "abc" ) );
  QTest::newRow( "basic-syntax33" ) << QString::fromLatin1( "{{ 1 }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "basic-syntax34" ) << QString::fromLatin1( "{{ 1.2 }}" ) << dict << QString::fromLatin1( "1.2" ) << NoError;

  dict.clear();

  dict.insert( QLatin1String( "abc" ), QLatin1String( "def" ) );

  QTest::newRow( "basic-syntax35" ) << QString::fromLatin1( "{{ abc._something }} {{ abc._something|upper }}" ) << dict << QString() << TagSyntaxError;
}

void TestBuiltinSyntax::testEnums_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QObject *otherClass = new OtherClass( this );
  dict.insert( QLatin1String( "var" ), QVariant::fromValue( otherClass ) );

  QTest::newRow( "class-enums01" ) << QString::fromLatin1( "{{ var.Lions }}" ) << dict << QString::fromLatin1( "0" ) << NoError;
  QTest::newRow( "class-enums02" ) << QString::fromLatin1( "{{ var.Tigers }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "class-enums03" ) << QString::fromLatin1( "{{ var.Bears }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "class-enums04" ) << QString::fromLatin1( "{{ var.Hamsters }}" ) << dict << QString() << NoError;
  QTest::newRow( "class-enums05" ) << QString::fromLatin1( "{{ var.Tigers.name }}" ) << dict << QString::fromLatin1( "Animals" ) << NoError;
  QTest::newRow( "class-enums06" ) << QString::fromLatin1( "{{ var.Tigers.scope }}" ) << dict << QString::fromLatin1( "OtherClass" ) << NoError;
  QTest::newRow( "class-enums07" ) << QString::fromLatin1( "{{ var.Tigers.value }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "class-enums08" ) << QString::fromLatin1( "{{ var.Tigers.key }}" ) << dict << QString::fromLatin1( "Tigers" ) << NoError;
  QTest::newRow( "class-enums09" ) << QString::fromLatin1( "{{ var.animals }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "class-enums10" ) << QString::fromLatin1( "{{ var.animals.name }}" ) << dict << QString::fromLatin1( "Animals" ) << NoError;
  QTest::newRow( "class-enums11" ) << QString::fromLatin1( "{{ var.animals.scope }}" ) << dict << QString::fromLatin1( "OtherClass" ) << NoError;
  QTest::newRow( "class-enums12" ) << QString::fromLatin1( "{{ var.animals.value }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "class-enums13" ) << QString::fromLatin1( "{{ var.animals.key }}" ) << dict << QString::fromLatin1( "Tigers" ) << NoError;
  QTest::newRow( "class-enums14" ) << QString::fromLatin1( "{{ var.Animals.0 }}" ) << dict << QString::fromLatin1( "0" ) << NoError;
  QTest::newRow( "class-enums15" ) << QString::fromLatin1( "{{ var.Animals.2 }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "class-enums16" ) << QString::fromLatin1( "{{ var.Animals.3 }}" ) << dict << QString() << NoError;
  QTest::newRow( "class-enums17" ) << QString::fromLatin1( "{{ var.Animals.0.name }}" ) << dict << QString::fromLatin1( "Animals" ) << NoError;
  QTest::newRow( "class-enums18" ) << QString::fromLatin1( "{{ var.Animals.0.scope }}" ) << dict << QString::fromLatin1( "OtherClass" ) << NoError;
  QTest::newRow( "class-enums19" ) << QString::fromLatin1( "{{ var.Animals.0.value }}" ) << dict << QString::fromLatin1( "0" ) << NoError;
  QTest::newRow( "class-enums20" ) << QString::fromLatin1( "{{ var.Animals.0.key }}" ) << dict << QString::fromLatin1( "Lions" ) << NoError;
  QTest::newRow( "class-enums21" ) << QString::fromLatin1( "{{ var.Animals.2.key }}" ) << dict << QString::fromLatin1( "Bears" ) << NoError;
  QTest::newRow( "class-enums22" ) << QString::fromLatin1( "{{ var.Tigers.samba }}" ) << dict << QString() << NoError;
  QTest::newRow( "class-enums23" )
      << QString::fromLatin1( "{% with var.animals as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}" ) << dict
      << QString::fromLatin1( "Tigers,1,OtherClass" ) << NoError;
  QTest::newRow( "class-enums24" )
      << QString::fromLatin1( "{% with var.Animals.2 as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}" ) << dict
      << QString::fromLatin1( "Bears,2,OtherClass" ) << NoError;
  QTest::newRow( "class-enums25" )
      << QString::fromLatin1( "{% with var.Bears as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}" ) << dict
      << QString::fromLatin1( "Bears,2,OtherClass" ) << NoError;
  QTest::newRow( "class-enums26" )
      << QString::fromLatin1( "{% with var.Animals as result %}{{ result.0.key }},{{ result.1.key }},{{ result.2.key }}{% endwith %}" ) << dict
      << QString::fromLatin1( "Lions,Tigers,Bears" ) << NoError;

  dict.clear();

  QObject *someClass = new SomeClass( this );
  dict.insert( QLatin1String( "var" ), QVariant::fromValue( someClass ) );

  QTest::newRow( "class-enums27" ) << QString::fromLatin1( "{{ var.Employee }}" ) << dict << QString::fromLatin1( "0" ) << NoError;
  QTest::newRow( "class-enums28" ) << QString::fromLatin1( "{{ var.Employer }}" ) << dict << QString::fromLatin1( "1" ) << NoError;
  QTest::newRow( "class-enums29" ) << QString::fromLatin1( "{{ var.Manager }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "class-enums30" ) << QString::fromLatin1( "{{ var.Voter }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "class-enums31" ) << QString::fromLatin1( "{{ var.Consumer }}" ) << dict << QString::fromLatin1( "4" ) << NoError;
  QTest::newRow( "class-enums32" ) << QString::fromLatin1( "{{ var.Citizen }}" ) << dict << QString::fromLatin1( "8" ) << NoError;
  QTest::newRow( "class-enums33" ) << QString::fromLatin1( "{{ var.FirstEnum }}" ) << dict << QString() << NoError;
  QTest::newRow( "class-enums34" ) << QString::fromLatin1( "{{ var.SecondEnum }}" ) << dict << QString() << NoError;

  QTest::newRow( "class-enums35" ) << QString::fromLatin1( "{% with var.SecondEnum as result %}"
                                        "{{ result.0 }},{{ result.1 }},{{ result.2 }},"
                                        "{{ result.0.key }},{{ result.1.key }},{{ result.2.key }},"
                                        "{{ result }},{{ result.scope }}"
                                      "{% endwith %}" )
                                   << dict << QString::fromLatin1( "2,4,8,Voter,Consumer,Citizen,,SomeClass" ) << NoError;

  dict.insert( QLatin1String( "var" ), QVariant::fromValue( otherClass ) );

  QTest::newRow( "enums-loops01" ) << QString::fromLatin1( "{% for enum in var.Animals %}{% ifequal enum var.Tigers %}"
                                      "<b>{{ enum.key }}</b>{% else %}{{ enum.key }}{% endifequal %},"
                                      "{% empty %}No content{% endfor %}" )
                                   << dict << QString::fromLatin1( "Lions,<b>Tigers</b>,Bears," ) << NoError;

  QTest::newRow( "enums-loops02" ) << QString::fromLatin1( "{% for enum in var.Tigers %}"
                                        "{% ifequal enum result %}<b>{{ enum.key }}</b>"
                                        "{% else %}{{ enum.key }}{% endifequal %},"
                                      "{% empty %}No content"
                                      "{% endfor %}" )
                                   << dict << QString::fromLatin1( "No content" ) << NoError;

  QTest::newRow( "enums-loops03" ) << QString::fromLatin1( "{% with var.animals as result %}"
                                        "{% for enum in var.Animals %}"
                                          "{% ifequal enum result %}<b>{{ enum.key }}</b>"
                                          "{% else %}{{ enum.key }}{% endifequal %},"
                                        "{% empty %}No content"
                                        "{% endfor %}"
                                      "{% endwith %}" )
                                   << dict << QString::fromLatin1( "Lions,<b>Tigers</b>,Bears," ) << NoError;

  QTest::newRow( "enums-keycount01" ) << QString::fromLatin1( "{{ var.Animals.keyCount }}" ) << dict << QString::fromLatin1( "3" ) << NoError;
  QTest::newRow( "enums-keycount02" ) << QString::fromLatin1( "{{ var.Lions.keyCount }}" ) << dict << QString::fromLatin1( "3" ) << NoError;
  QTest::newRow( "enums-keycount02" ) << QString::fromLatin1( "{{ var.animals.keyCount }}" ) << dict << QString::fromLatin1( "3" ) << NoError;

  QTest::newRow( "qt-enums01" ) << QString::fromLatin1( "{{ Qt.AlignRight }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "qt-enums02" ) << QString::fromLatin1( "{{ Qt.AlignRight.scope }}" ) << dict << QString::fromLatin1( "Qt" ) << NoError;
  QTest::newRow( "qt-enums03" ) << QString::fromLatin1( "{{ Qt.AlignRight.name }}" ) << dict << QString::fromLatin1( "Alignment" ) << NoError;
  QTest::newRow( "qt-enums04" ) << QString::fromLatin1( "{{ Qt.AlignRight.value }}" ) << dict << QString::fromLatin1( "2" ) << NoError;
  QTest::newRow( "qt-enums05" ) << QString::fromLatin1( "{{ Qt.AlignRight.key }}" ) << dict << QString::fromLatin1( "AlignRight" ) << NoError;
  QTest::newRow( "qt-enums06" ) << QString::fromLatin1( "{{ Qt.Alignment.2.key }}" ) << dict << QString::fromLatin1( "AlignRight" ) << NoError;
}

void TestBuiltinSyntax::testListIndex_data()
{

  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantList l;
  l << QString::fromLatin1( "first item" ) << QString::fromLatin1( "second item" );

  dict.insert( QLatin1String( "var" ), l );

  // List-index syntax allows a template to access a certain item of a subscriptable object.
  QTest::newRow( "list-index01" ) << QString::fromLatin1( "{{ var.1 }}" ) << dict << QString::fromLatin1( "second item" ) << NoError;
  // Fail silently when the list index is out of range.
  QTest::newRow( "list-index02" ) << QString::fromLatin1( "{{ var.5 }}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariant() );

  // Fail silently when the variable is not a subscriptable object.
  QTest::newRow( "list-index03" ) << QString::fromLatin1( "{{ var.1 }}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariantHash() );
  // Fail silently when variable is a dict without the specified key.
  QTest::newRow( "list-index04" ) << QString::fromLatin1( "{{ var.1 }}" ) << dict << QString() << NoError;


  dict.clear();

  QVariantHash hash;
  hash.insert( QLatin1String( "1" ), QLatin1String( "hello" ) );
  dict.insert( QLatin1String( "var" ), hash );
  // Dictionary lookup wins out when dict's key is a string.
  QTest::newRow( "list-index05" ) << QString::fromLatin1( "{{ var.1 }}" ) << dict << QString::fromLatin1( "hello" ) << NoError;

  // QVariantHash can only use strings as keys, so list-index06 and list-index07 are not valid.

  dict.clear();

  QStringList sl;
  sl.append( QLatin1String( "hello" ) );
  sl.append( QLatin1String( "world" ) );
  dict.insert( QLatin1String( "var" ), sl );
  // QStringList lookup
  QTest::newRow( "list-index08" ) << QString::fromLatin1( "{{ var.0 }}, {{ var.1 }}!" ) << dict << QString::fromLatin1( "hello, world!" ) << NoError;
}


void TestBuiltinSyntax::testFilterSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  // Basic filter usage
  dict.insert( QLatin1String( "var" ), QLatin1String( "Django is the greatest!" ) );
  QTest::newRow( "filter-syntax01" ) << QString::fromLatin1( "{{ var|upper }}" ) << dict << QString::fromLatin1( "DJANGO IS THE GREATEST!" ) << NoError;

  // Chained filters
  QTest::newRow( "filter-syntax02" ) << QString::fromLatin1( "{{ var|upper|lower }}" ) << dict << QString::fromLatin1( "django is the greatest!" ) << NoError;

  // Raise TemplateSyntaxError for space between a variable and filter pipe
  dict.clear();
  QTest::newRow( "filter-syntax03" ) << QString::fromLatin1( "{{ var |upper }}" ) << dict << QString() << TagSyntaxError;

  // Raise TemplateSyntaxError for space after a filter pipe
  QTest::newRow( "filter-syntax04" ) << QString::fromLatin1( "{{ var| upper }}" ) << dict << QString() << TagSyntaxError;

  // Raise TemplateSyntaxError for a nonexistent filter
  QTest::newRow( "filter-syntax05" ) << QString::fromLatin1( "{{ var|does_not_exist }}" ) << dict << QString() << UnknownFilterError;

  // Raise TemplateSyntaxError when trying to access a filter containing an illegal character
  QTest::newRow( "filter-syntax06" ) << QString::fromLatin1( "{{ var|fil(ter) }}" ) << dict << QString() << UnknownFilterError;

  // Raise TemplateSyntaxError for invalid block tags
  QTest::newRow( "filter-syntax07" ) << QString::fromLatin1( "{% nothing_to_see_here %}" ) << dict << QString() << InvalidBlockTagError;
  // Raise TemplateSyntaxError for empty block tags
  QTest::newRow( "filter-syntax08" ) << QString::fromLatin1( "{% %}" ) << dict << QString() << InvalidBlockTagError;

  // Chained filters, with an argument to the first one
  dict.insert( QLatin1String( "var" ), QLatin1String( "<b><i>Yes</i></b>" ) );
  QTest::newRow( "filter-syntax09" ) << "{{ var|removetags:\"b i\"|upper|lower }}" << dict << QString::fromLatin1( "yes" ) << NoError;
  // Literal string as argument is always "safe" from auto-escaping..
  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariant() );
  QTest::newRow( "filter-syntax10" ) << "{{ var|default_if_none:\" endquote\\\" hah\" }}" << dict << " endquote\" hah" << NoError;
  // Variable as argument
  dict.insert( QLatin1String( "var2" ), QLatin1String( "happy" ) );
  QTest::newRow( "filter-syntax11" ) << QString::fromLatin1( "{{ var|default_if_none:var2 }}" ) << dict << QString::fromLatin1( "happy" ) << NoError;
  // Default argument testing
  dict.clear();
  dict.insert( QLatin1String( "var" ), true );
  QTest::newRow( "filter-syntax12" ) << "{{ var|yesno:\"yup,nup,mup\" }} {{ var|yesno }}" << dict << QString::fromLatin1( "yup yes" ) << NoError;

  // Fail silently for methods that raise an exception with a
  // "silent_variable_failure" attribute
//   dict.clear();
//   QObject *someClass = new SomeClass(this);
//   dict.insert( QLatin1String( "var" ), QVariant::fromValue(someClass));
//   QTest::newRow("filter-syntax13") << QString::fromLatin1( "1{{ var.method3 }}2" ) << dict << QString::fromLatin1( "12" ) << NoError;
//   // In methods that raise an exception without a
//   // "silent_variable_attribute" set to True, the exception propagates
//   // #C# SomeOtherException)
//   QTest::newRow("filter-syntax14") << QString::fromLatin1( "var" ) << dict << QString() << TagSyntaxError;

  // Escaped backslash in argument
  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariant() );
  QTest::newRow( "filter-syntax15" ) << "{{ var|default_if_none:\"foo\\bar\" }}" << dict << "foo\\bar" << NoError;
  // Escaped backslash using known escape char
  QTest::newRow( "filter-syntax16" ) << "{{ var|default_if_none:\"foo\\now\" }}" << dict << "foo\\now" << NoError;
  // Empty strings can be passed as arguments to filters
  dict.clear();
  dict.insert( QLatin1String( "var" ), QVariantList() << QString::fromLatin1( "a" ) << QString::fromLatin1( "b" ) << QString::fromLatin1( "c" ) );
  QTest::newRow( "filter-syntax17" ) << "{{ var|join:\"\" }}" << dict << QString::fromLatin1( "abc" ) << NoError;

  // Make sure that any unicode strings are converted to bytestrings
  // in the final output.
//   FAIL'filter-syntax18': (r'{{ var }}', {'var': UTF8Class()}, u'\u0160\u0110\u0106\u017d\u0107\u017e\u0161\u0111'),

  // Numbers as filter arguments should work
  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "hello world" ) );
  QTest::newRow( "filter-syntax19" ) << QString::fromLatin1( "{{ var|truncatewords:1 }}" ) << dict << QString::fromLatin1( "hello ..." ) << NoError;
  //filters should accept empty string constants
  dict.clear();
  QTest::newRow( "filter-syntax20" ) << "{{ \"\"|default_if_none:\"was none\" }}" << dict << QString() << NoError;

}

void TestBuiltinSyntax::testCommentSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "comment-syntax01" ) << QString::fromLatin1( "{# this is hidden #}hello" ) << dict << QString::fromLatin1( "hello" ) << NoError;
  QTest::newRow( "comment-syntax02" ) << QString::fromLatin1( "{# this is hidden #}hello{# foo #}" ) << dict << QString::fromLatin1( "hello" ) << NoError;
  // Comments can contain invalid stuff.
  QTest::newRow( "comment-syntax03" ) << QString::fromLatin1( "foo{#  {% if %}  #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax04" ) << QString::fromLatin1( "foo{#  {% endblock %}  #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax05" ) << QString::fromLatin1( "foo{#  {% somerandomtag %}  #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax06" ) << QString::fromLatin1( "foo{# {% #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax07" ) << QString::fromLatin1( "foo{# %} #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax08" ) << QString::fromLatin1( "foo{# %} #}bar" ) << dict << QString::fromLatin1( "foobar" ) << NoError;
  QTest::newRow( "comment-syntax09" ) << QString::fromLatin1( "foo{# {{ #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax10" ) << QString::fromLatin1( "foo{# }} #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax11" ) << QString::fromLatin1( "foo{# { #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
  QTest::newRow( "comment-syntax12" ) << QString::fromLatin1( "foo{# } #}" ) << dict << QString::fromLatin1( "foo" ) << NoError;
}

void TestBuiltinSyntax::testMultiline_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "multiline01" ) << "Hello,\nboys.\nHow\nare\nyou\ngentlemen?" << dict << "Hello,\nboys.\nHow\nare\nyou\ngentlemen?" << NoError;

}

void TestBuiltinSyntax::testEscaping_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;


  // html escaping is not to be confused with for example url escaping.
  dict.insert( QLatin1String( "var" ), QLatin1String( "< > & \" \' # = % $" ) );
  QTest::newRow( "escape01" ) << QString::fromLatin1( "{{ var }}" ) << dict << "&lt; &gt; &amp; \" &#39; # = % $" << NoError;

  dict.clear();
  dict.insert( QLatin1String( "var" ), QLatin1String( "this & that" ) );
  QTest::newRow( "escape02" ) << QString::fromLatin1( "{{ var }}" ) << dict << QString::fromLatin1( "this &amp; that" ) << NoError;

  // Strings are compared unescaped.
  QTest::newRow( "escape03" ) << "{% ifequal var \"this & that\" %}yes{% endifequal %}" << dict << QString::fromLatin1( "yes" ) << NoError;

  // Arguments to filters are 'safe' and manipulate their input unescaped.
  QTest::newRow( "escape04" ) << "{{ var|cut:\"&\" }}" << dict << QString::fromLatin1( "this  that" ) << NoError;

  dict.insert( QLatin1String( "varList" ), QVariantList() << QString::fromLatin1( "Tom" ) << QString::fromLatin1( "Dick" ) << QString::fromLatin1( "Harry" ) );
  QTest::newRow( "escape05" ) << "{{ varList|join:\" & \" }}" << dict << QString::fromLatin1( "Tom & Dick & Harry" ) << NoError;

  // Same with variable args.
  dict.insert( QLatin1String( "amp" ), QLatin1String( " & " ) );
  QTest::newRow( "escape06" ) << QString::fromLatin1( "{{ varList|join:amp }}" ) << dict << QString::fromLatin1( "Tom & Dick & Harry" ) << NoError;

  // Literal strings are safe.
  QTest::newRow( "escape07" ) << "{{ \"this & that\" }}" << dict << QString::fromLatin1( "this & that" ) << NoError;

  // Iterating outputs safe characters.
  dict.clear();
  QVariantList list;
  list << QString::fromLatin1( "K" ) << QString::fromLatin1( "&" ) << QString::fromLatin1( "R" );
  dict.insert( QLatin1String( "list" ), list );
  QTest::newRow( "escape08" ) << QString::fromLatin1( "{% for letter in list %}{{ letter }},{% endfor %}" ) << dict << QString::fromLatin1( "K,&amp;,R," ) << NoError;

  dict.clear();
  // escape requirement survives lookup.
  QVariantHash hash;
  hash.insert( QLatin1String( "key" ), QLatin1String( "this & that" ) );
  dict.insert( QLatin1String( "var" ), hash );
  QTest::newRow( "escape09" ) << QString::fromLatin1( "{{ var.key }}" ) << dict << QString::fromLatin1( "this &amp; that" ) << NoError;

  dict.clear();

}

void TestBuiltinSyntax::testMultipleStates()
{
  Engine *engine1 = getEngine();

  InMemoryTemplateLoader::Ptr loader1 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader1->setTemplate( QLatin1String( "template1" ), QLatin1String( "Template 1" ) );
  engine1->addTemplateLoader( loader1 );

  Template t1 = engine1->newTemplate( QLatin1String( "{% include \"template1\" %}" ), QLatin1String( "\"template1\"" ) );

  Engine *engine2 = getEngine();

  InMemoryTemplateLoader::Ptr loader2 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader2->setTemplate( QLatin1String( "template2" ), QLatin1String( "Template 2" ) );

  engine2->addTemplateLoader( loader2 );

  Template t2 = engine2->newTemplate( QLatin1String( "{% include \"template2\" %}" ), QLatin1String( "\"template2\"" ) );

  Engine *engine3 = getEngine();

  InMemoryTemplateLoader::Ptr loader3 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader3->setTemplate( QLatin1String( "template3" ), QLatin1String( "Template 3" ) );

  engine3->addTemplateLoader( loader3 );

  Template t3 = engine3->newTemplate( QLatin1String( "{% include var %}" ), QLatin1String( "var" ) );

  QVariantHash h;
  h.insert( QLatin1String( "var" ), QLatin1String( "template3" ) );
  Context c( h );
  t1->render( &c );

  QString expected1 = QLatin1String( "Template 1" );
  QString expected2 = QLatin1String( "Template 2" );
  QString expected3 = QLatin1String( "Template 3" );
  QCOMPARE( t1->render( &c ), expected1 );
  QCOMPARE( t2->render( &c ), expected2 );
  QCOMPARE( t3->render( &c ), expected3 );
}

void TestBuiltinSyntax::testAlternativeEscaping()
{
  Engine *engine1 = getEngine();

  Template t1 = engine1->newTemplate( QLatin1String( "{{ var }}" ), QLatin1String( "\"template1\"" ) );

  QString input = QLatin1String( "< > \r\n & \" \' # = % $" );

  QVariantHash h;
  h.insert( QLatin1String( "var" ), input );
  Context c( h );

  QString output;
  QTextStream ts( &output );

  NoEscapeOutputStream noEscapeOs( &ts );

  t1->render( &noEscapeOs, &c );

  QCOMPARE( output, input );
  output.clear();

  JSOutputStream jsOs( &ts );

  t1->render( &jsOs, &c );

  QString jsOutput( QLatin1String( "\\u003C \\u003E \\u000D\\u000A \\u0026 \\u0022 \\u0027 # \\u003D % $" ) );

  QCOMPARE( output, jsOutput );
}

void TestBuiltinSyntax::testTemplatePathSafety_data()
{
  QTest::addColumn<QString>( "inputPath" );
  QTest::addColumn<QString>( "output" );

  QTest::newRow( "template-path-safety01" ) << QString::fromLatin1( "visible_file" ) << QString::fromLatin1( "visible_file" );
  QTest::newRow( "template-path-safety02" ) << QString::fromLatin1( "../invisible_file" ) << QString();
}

void TestBuiltinSyntax::testTemplatePathSafety()
{
  QFETCH( QString, inputPath );
  QFETCH( QString, output );

  Grantlee::FileSystemTemplateLoader *loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs( QStringList() << QString::fromLatin1( "." ) );

  QFile f( inputPath );
  bool opened = f.open( QFile::WriteOnly | QFile::Text );
  QVERIFY( opened );
  f.write( inputPath.toUtf8() );
  f.close();

  Template t = loader->loadByName( inputPath, m_engine );
  Context c;
  if ( output.isEmpty() )
    QVERIFY( !t );
  else
    QCOMPARE( t->render( &c ), inputPath );

  MutableTemplate mt = loader->loadMutableByName( inputPath, m_engine );
  if ( output.isEmpty() )
    QVERIFY( !mt );
  else
    QCOMPARE( mt->render( &c ), inputPath );

  delete loader;
  f.remove();
}

void TestBuiltinSyntax::testMediaPathSafety_data()
{
  QTest::addColumn<QString>( "inputPath" );
  QTest::addColumn<QString>( "output" );

  QTest::newRow( "media-path-safety01" ) << QString::fromLatin1( "visible_file" ) << QString::fromLatin1( "./visible_file" );
  QTest::newRow( "media-path-safety02" ) << QString::fromLatin1( "../invisible_file" ) << QString();
}

void TestBuiltinSyntax::testMediaPathSafety()
{
  QFETCH( QString, inputPath );
  QFETCH( QString, output );

  Grantlee::FileSystemTemplateLoader *loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs( QStringList() << QString::fromLatin1( "." ) );

  QFile f( inputPath );
  bool opened = f.open( QFile::WriteOnly | QFile::Text );
  QVERIFY( opened );
  f.write( inputPath.toUtf8() );
  f.close();

  QPair<QString, QString> uri = loader->getMediaUri( inputPath );
  if ( output.isEmpty() )
    QVERIFY( uri.second.isEmpty() );
  else
    QCOMPARE( QFileInfo( uri.first + uri.second ).absoluteFilePath(), QFileInfo( output ).absoluteFilePath() );

  delete loader;
  f.remove();
}

void TestBuiltinSyntax::testTypeAccessors_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantHash itemsHash;
  itemsHash.insert( QLatin1String( "one" ), 1 );
  itemsHash.insert( QLatin1String( "two" ), 2 );
  itemsHash.insert( QLatin1String( "three" ), 3 );

  dict.insert( QLatin1String( "hash" ), itemsHash );

  QTest::newRow( "type-accessors-hash01" ) << QString::fromLatin1( "{{ hash.items|length }}" ) << dict << QString::fromLatin1( "3" ) << NoError;
  QTest::newRow( "type-accessors-hash02" ) << QString::fromLatin1( "{% for key,value in hash.items %}{{ key }}:{{ value }};{% endfor %}" )
                                      << dict << QString::fromLatin1( "one:1;two:2;three:3;" ) << NoError;
  QTest::newRow( "type-accessors-hash03" ) << QString::fromLatin1( "{{ hash.keys|length }}" ) << dict << QString::fromLatin1( "3" ) << NoError;
  QTest::newRow( "type-accessors-hash04" ) << QString::fromLatin1( "{% for key in hash.keys %}{{ key }};{% endfor %}" )
                                      << dict << QString::fromLatin1( "one;two;three;" ) << NoError;
  QTest::newRow( "type-accessors-hash05" ) << QString::fromLatin1( "{{ hash.values|length }}" ) << dict << QString::fromLatin1( "3" ) << NoError;
  QTest::newRow( "type-accessors-hash06" ) << QString::fromLatin1( "{% for value in hash.values %}{{ value }};{% endfor %}" )
                                      << dict << QString::fromLatin1( "1;2;3;" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "my string" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "mystring" ) );

  QTest::newRow( "type-accessors-string01" ) << QString::fromLatin1( "{{ str1.capitalize }}" ) << dict << QString::fromLatin1( "My string" ) << NoError;
  QTest::newRow( "type-accessors-string02" ) << QString::fromLatin1( "{{ str2.capitalize }}" ) << dict << QString::fromLatin1( "Mystring" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "de24335fre" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "de435f3.-5r" ) );

  QTest::newRow( "type-accessors-string03" ) << QString::fromLatin1( "{{ str1.isalnum }}" ) << dict << QString::fromLatin1( "True" ) << NoError;
  QTest::newRow( "type-accessors-string04" ) << QString::fromLatin1( "{{ str2.isalnum }}" ) << dict << QString::fromLatin1( "False" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "24335" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "de435f35r" ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "de435f3.-5r" ) );

  QTest::newRow( "type-accessors-string05" ) << QString::fromLatin1( "{{ str1.isdigit }}" ) << dict << QString::fromLatin1( "True" ) << NoError;
  QTest::newRow( "type-accessors-string06" ) << QString::fromLatin1( "{{ str2.isdigit }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string07" ) << QString::fromLatin1( "{{ str3.isdigit }}" ) << dict << QString::fromLatin1( "False" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str" ), QLatin1String( "MyString" ) );
  dict.insert( QLatin1String( "lowerStr" ), QLatin1String( "mystring" ) );

  QTest::newRow( "type-accessors-string08" ) << QString::fromLatin1( "{{ str.islower }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string09" ) << QString::fromLatin1( "{{ lowerStr.islower }}" ) << dict << QString::fromLatin1( "True" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "        " ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "     r  " ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( " \t\nr  " ) );
  dict.insert( QLatin1String( "str4" ), QLatin1String( " \t\n   " ) );

  QTest::newRow( "type-accessors-string10" ) << QString::fromLatin1( "{{ str1.isspace }}" ) << dict << QString::fromLatin1( "True" ) << NoError;
  QTest::newRow( "type-accessors-string11" ) << QString::fromLatin1( "{{ str2.isspace }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string12" ) << QString::fromLatin1( "{{ str3.isspace }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string13" ) << QString::fromLatin1( "{{ str4.isspace }}" ) << dict << QString::fromLatin1( "True" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "My String" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "Mystring" ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "My string" ) );
  dict.insert( QLatin1String( "str4" ), QLatin1String( "my string" ) );

  QTest::newRow( "type-accessors-string14" ) << QString::fromLatin1( "{{ str1.istitle }}" ) << dict << QString::fromLatin1( "True" ) << NoError;
  QTest::newRow( "type-accessors-string15" ) << QString::fromLatin1( "{{ str2.istitle }}" ) << dict << QString::fromLatin1( "True" ) << NoError;
  QTest::newRow( "type-accessors-string16" ) << QString::fromLatin1( "{{ str3.istitle }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string17" ) << QString::fromLatin1( "{{ str4.istitle }}" ) << dict << QString::fromLatin1( "False" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str" ), QLatin1String( "MyString" ) );
  dict.insert( QLatin1String( "upperStr" ), QLatin1String( "MYSTRING" ) );

  QTest::newRow( "type-accessors-string18" ) << QString::fromLatin1( "{{ str.isupper }}" ) << dict << QString::fromLatin1( "False" ) << NoError;
  QTest::newRow( "type-accessors-string19" ) << QString::fromLatin1( "{{ upperStr.isupper }}" ) << dict << QString::fromLatin1( "True" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "My String" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "MYSTRING" ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "MY STRING" ) );

  QTest::newRow( "type-accessors-string20" ) << QString::fromLatin1( "{{ str1.lower }}" ) << dict << QString::fromLatin1( "my string" ) << NoError;
  QTest::newRow( "type-accessors-string21" ) << QString::fromLatin1( "{{ str2.lower }}" ) << dict << QString::fromLatin1( "mystring" ) << NoError;
  QTest::newRow( "type-accessors-string22" ) << QString::fromLatin1( "{{ str3.lower }}" ) << dict << QString::fromLatin1( "my string" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str" ), QLatin1String( "one\ntwo three\nfour" ) );

  QTest::newRow( "type-accessors-string23" ) << QString::fromLatin1( "{% for line in str.splitlines %}{{ line }};{% endfor %}" ) << dict << QString::fromLatin1( "one;two three;four;" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "          one" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "     one     " ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "one          " ) );
  dict.insert( QLatin1String( "str4" ), QLatin1String( "             " ) );
  dict.insert( QLatin1String( "str5" ), QLatin1String( "" ) );

  QTest::newRow( "type-accessors-string24" ) << QString::fromLatin1( "{{ str1.strip }}" ) << dict << QString::fromLatin1( "one" ) << NoError;
  QTest::newRow( "type-accessors-string25" ) << QString::fromLatin1( "{{ str2.strip }}" ) << dict << QString::fromLatin1( "one" ) << NoError;
  QTest::newRow( "type-accessors-string26" ) << QString::fromLatin1( "{{ str3.strip }}" ) << dict << QString::fromLatin1( "one" ) << NoError;
  QTest::newRow( "type-accessors-string27" ) << QString::fromLatin1( "{{ str4.strip }}" ) << dict << QString() << NoError;
  QTest::newRow( "type-accessors-string28" ) << QString::fromLatin1( "{{ str5.strip }}" ) << dict << QString() << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "My String" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "mY sTRING" ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "My StrInG" ) );
  dict.insert( QLatin1String( "str4" ), QLatin1String( "my string" ) );
  dict.insert( QLatin1String( "str5" ), QLatin1String( "MY STRING" ) );

  // Yes, this really is a python built-in.
  QTest::newRow( "type-accessors-string29" ) << QString::fromLatin1( "{{ str1.swapcase }}" ) << dict << QString::fromLatin1( "mY sTRING" ) << NoError;
  QTest::newRow( "type-accessors-string30" ) << QString::fromLatin1( "{{ str2.swapcase }}" ) << dict << QString::fromLatin1( "My String" ) << NoError;
  QTest::newRow( "type-accessors-string31" ) << QString::fromLatin1( "{{ str3.swapcase }}" ) << dict << QString::fromLatin1( "mY sTRiNg" ) << NoError;
  QTest::newRow( "type-accessors-string32" ) << QString::fromLatin1( "{{ str4.swapcase }}" ) << dict << QString::fromLatin1( "MY STRING" ) << NoError;
  QTest::newRow( "type-accessors-string33" ) << QString::fromLatin1( "{{ str5.swapcase }}" ) << dict << QString::fromLatin1( "my string" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "str1" ), QLatin1String( "My String" ) );
  dict.insert( QLatin1String( "str2" ), QLatin1String( "mystring" ) );
  dict.insert( QLatin1String( "str3" ), QLatin1String( "my string" ) );
  dict.insert( QLatin1String( "str4" ), QLatin1String( "my String" ) );
  dict.insert( QLatin1String( "str5" ), QLatin1String( "My string" ) );

  QTest::newRow( "type-accessors-string34" ) << QString::fromLatin1( "{{ str1.title }}" ) << dict << QString::fromLatin1( "My String" ) << NoError;
  QTest::newRow( "type-accessors-string35" ) << QString::fromLatin1( "{{ str2.title }}" ) << dict << QString::fromLatin1( "Mystring" ) << NoError;
  QTest::newRow( "type-accessors-string36" ) << QString::fromLatin1( "{{ str3.title }}" ) << dict << QString::fromLatin1( "My String" ) << NoError;
  QTest::newRow( "type-accessors-string37" ) << QString::fromLatin1( "{{ str4.title }}" ) << dict << QString::fromLatin1( "My String" ) << NoError;
  QTest::newRow( "type-accessors-string38" ) << QString::fromLatin1( "{{ str5.title }}" ) << dict << QString::fromLatin1( "My String" ) << NoError;

  QTest::newRow( "type-accessors-string39" ) << QString::fromLatin1( "{{ str1.upper }}" ) << dict << QString::fromLatin1( "MY STRING" ) << NoError;
  QTest::newRow( "type-accessors-string40" ) << QString::fromLatin1( "{{ str2.upper }}" ) << dict << QString::fromLatin1( "MYSTRING" ) << NoError;
  QTest::newRow( "type-accessors-string41" ) << QString::fromLatin1( "{{ str3.upper }}" ) << dict << QString::fromLatin1( "MY STRING" ) << NoError;

  dict.clear();

#define SON(obj) obj->setObjectName( QLatin1String( #obj ) )

  QObject *obj1 = new QObject(this);
  SON(obj1);
  QObject *obj2 = new QObject(this);
  SON(obj2);
  obj2->setParent(obj1);
  QObject *obj3 = new QObject(this);
  obj3->setParent(obj2);
  SON(obj3);
  QObject *obj4 = new QObject(this);
  obj4->setParent(obj2);
  SON(obj4);

  dict.insert( QLatin1String( "object" ), QVariant::fromValue( obj1 ) );

  QTest::newRow( "type-accessors-qobject01" ) << QString::fromLatin1( "{{ object.objectName }}" ) << dict << QString::fromLatin1( "obj1" ) << NoError;

  const QLatin1String objectDumper( "<li>{{ object.objectName }}</li>"
                                    "{% if object.children %}"
                                      "<ul>"
                                        "{% for object in object.children %}"
                                          "{% include 'objectdumper.html' %}"
                                        "{% endfor %}"
                                      "</ul>"
                                    "{% endif %}" );

  m_loader->setTemplate( QLatin1String( "objectdumper.html" ), objectDumper );

  QTest::newRow( "type-accessors-qobject02" ) << QString::fromLatin1( "<ul>{% include 'objectdumper.html' %}</ul>" ) << dict <<
                              QString::fromLatin1( "<ul>"
                                                   "<li>obj1</li>"
                                                      "<ul>"
                                                        "<li>obj2</li>"
                                                          "<ul>"
                                                          "<li>obj3</li>"
                                                          "<li>obj4</li>"
                                                        "</ul>"
                                                      "</ul>"
                                                    "</ul>" ) << NoError;

}


QTEST_MAIN( TestBuiltinSyntax )
#include "testbuiltins.moc"

#endif

