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
#include <QObject>

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

public:
  OtherClass( QObject *parent = 0 )
      : QObject( parent ) { }

  QString method() const {
    return "OtherClass::method";
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
  SomeClass( QObject *parent = 0 )
      : QObject( parent ), m_other( new OtherClass( this ) ) { }

  QString method() const {
    return "SomeClass::method";
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

  virtual QSharedPointer< OutputStream > clone() const
  {
    return QSharedPointer<OutputStream>( new NoEscapeOutputStream );
  }

  virtual QString escape(const QString& input) const
  {
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

  virtual QSharedPointer< OutputStream > clone() const
  {
    return QSharedPointer<OutputStream>( new JSOutputStream );
  }

  virtual QString escape(const QString& input) const
  {
    QList<QPair<QString, QString> > jsEscapes;
    jsEscapes << QPair<QString, QString>( "\\", "\\x5C" )
              << QPair<QString, QString>( "\'",  "\\x27" )
              << QPair<QString, QString>( "\"",  "\\x22" )
              << QPair<QString, QString>( ">", "\\x3E" )
              << QPair<QString, QString>( "<", "\\x3C" )
              << QPair<QString, QString>( "&", "\\x26" )
              << QPair<QString, QString>( "=", "\\x3D" )
              << QPair<QString, QString>( "-", "\\x2D" )
              << QPair<QString, QString>( ";", "\\x3B" )
              << QPair<QString, QString>( QChar( 0x2028 ), "\\u2028" )
              << QPair<QString, QString>( QChar( 0x2029 ), "\\u2029" );

    for( int i = 0; i < 32; ++i )
    {
      jsEscapes << QPair<QString, QString>( QChar( i ), "\\x" + QString( "%1" ).arg( i, 2, 16, QChar('0') ).toUpper() );
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

private slots:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() {
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

  Engine* getEngine();

  void doTest();

};

void TestBuiltinSyntax::initTestCase()
{
  m_engine = getEngine();
}

Engine* TestBuiltinSyntax::getEngine()
{
  Engine *engine = new Engine( this );
  engine->setPluginDirs( QStringList() << GRANTLEE_PLUGIN_PATH );
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

  Template t = m_engine->newTemplate( input, QTest::currentDataTag() );

  Context context( dict );

  QString result = t->render( &context );
  if ( t->error() != NoError ) {
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
  QTest::newRow( "basic-syntax01" ) << "something cool" << dict << "something cool" << NoError;

  // Variables should be replaced with their value in the current
  // context
  dict.insert( "headline", "Success" );
  QTest::newRow( "basic-syntax02" ) << "{{ headline }}" << dict << "Success" << NoError;

  dict.clear();
  dict.insert( "first", 1 );
  dict.insert( "second", 2 );

  // More than one replacement variable is allowed in a template
  QTest::newRow( "basic-syntax03" ) << "{{ first }} --- {{ second }}" << dict << "1 --- 2" << NoError;

  dict.clear();
  // Fail silently when a variable is not found in the current context
  QTest::newRow( "basic-syntax04" ) << "as{{ missing }}df" << dict << "asdf" << NoError;

  // A variable may not contain more than one word
  QTest::newRow( "basic-syntax06" ) << "{{ multi word variable }}" << dict << "" << TagSyntaxError;
  // Raise TemplateSyntaxError for empty variable tags
  QTest::newRow( "basic-syntax07" ) << "{{ }}" << dict << "" << EmptyVariableError;
  QTest::newRow( "basic-syntax08" ) << "{{        }}" << dict << "" << EmptyVariableError;


  // Attribute syntax allows a template to call an object's attribute

  QObject *someClass = new SomeClass( this );
  dict.insert( "var", QVariant::fromValue( someClass ) );

  QTest::newRow( "basic-syntax09" ) << "{{ var.method }}" << dict << "SomeClass::method" << NoError;

  // Multiple levels of attribute access are allowed
  QTest::newRow( "basic-syntax10" ) << "{{ var.otherClass.method }}" << dict << "OtherClass::method" << NoError;

  // Fail silently when a variable's attribute isn't found
  QTest::newRow( "basic-syntax11" ) << "{{ var.blech }}" << dict << "" << NoError;

  // TODO: Needed?
  // Raise TemplateSyntaxError when trying to access a variable beginning with an underscore
  // #C# {"var": SomeClass()}
  dict.clear();
  QVariantHash hash;
  hash.insert( "__dict__",  "foo" );
  dict.insert( "var", hash );
  QTest::newRow( "basic-syntax12" ) << "{{ var.__dict__ }}" << dict << "" << TagSyntaxError;

  dict.clear();
  // Raise TemplateSyntaxError when trying to access a variable containing an illegal character
  QTest::newRow( "basic-syntax13" ) << "{{ va>r }}" << dict << "" << TagSyntaxError;
  QTest::newRow( "basic-syntax14" ) << "{{ (var.r) }}" << dict << "" << TagSyntaxError;
  QTest::newRow( "basic-syntax15" ) << "{{ sp%am }}" << dict << "" << TagSyntaxError;
  QTest::newRow( "basic-syntax16" ) << "{{ eggs! }}" << dict << "" << TagSyntaxError;
  QTest::newRow( "basic-syntax17" ) << "{{ moo? }}" << dict << "" << TagSyntaxError;

  // Attribute syntax allows a template to call a dictionary key's value

  hash.clear();
  hash.insert( "bar", "baz" );
  dict.insert( "foo", hash );
  QTest::newRow( "basic-syntax18" ) << "{{ foo.bar }}" << dict << "baz" << NoError;

  // Fail silently when a variable's dictionary key isn't found
  QTest::newRow( "basic-syntax19" ) << "{{ foo.spam }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "var", QVariant::fromValue( someClass ) );
  // Fail silently when attempting to access an unavailable method
  QTest::newRow( "basic-syntax20" ) << "{{ var.nonAccessibleMethod }}" << dict << "" << NoError;

  dict.clear();
  // Don't get confused when parsing something that is almost, but not
  // quite, a template tag.
  QTest::newRow( "basic-syntax21" ) << "a {{ moo %} b" << dict << "a {{ moo %} b" << NoError;
  QTest::newRow( "basic-syntax22" ) << "{{ moo #}" << dict << "{{ moo #}" << NoError;

  dict.insert( "cow", "cow" );
  // Will try to treat "moo #} {{ cow" as the variable. Not ideal, but
  // costly to work around, so this triggers an error.
  QTest::newRow( "basic-syntax23" ) << "{{ moo #} {{ cow }}" << dict << "" << TagSyntaxError;

  dict.clear();
  // Embedded newlines make it not-a-tag.
  QTest::newRow( "basic-syntax24" ) << "{{ moo\n }}" << dict << "{{ moo\n }}" << NoError;
  // Literal strings are permitted inside variables, mostly for i18n
  // purposes.
  QTest::newRow( "basic-syntax25" ) << "{{ \"fred\" }}" << dict << "fred" << NoError;
  QTest::newRow( "basic-syntax26" ) << "{{ \"\\\"fred\\\"\" }}" << dict << "\"fred\"" << NoError;
  QTest::newRow( "basic-syntax27" ) << "{{ _(\"\\\"fred\\\"\") }}" << dict << "\"fred\"" << NoError;


  dict.clear();
  hash.clear();
  QVariantHash innerHash;
  innerHash.insert( "3", "d" );
  hash.insert( "2", innerHash );
  dict.insert( "1", hash );

  QTest::newRow( "basic-syntax28" ) << "{{ 1.2.3 }}" << dict << "d" << NoError;

  dict.clear();
  hash.clear();
  QVariantList list;
  list << "a" << "b" << "c" << "d";
  hash.insert( "2", list );
  dict.insert( "1", hash );
  QTest::newRow( "basic-syntax29" ) << "{{ 1.2.3 }}" << dict << "d" << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << "x" << "x" << "x" << "x";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "y" << "y" << "y" << "y";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "a" << "b" << "c" << "d";
  list.append( QVariant( innerList ) );
  dict.insert( "1", list );

  QTest::newRow( "basic-syntax30" ) << "{{ 1.2.3 }}" << dict << "d" << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << "x" << "x" << "x" << "x";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "y" << "y" << "y" << "y";
  list.append( QVariant( innerList ) );
  innerList.clear();
  innerList << "a" << "b" << "c" << "d";
  list.append( QVariant( innerList ) );
  dict.insert( "1", list );

  QTest::newRow( "basic-syntax31" ) << "{{ 1.2.3 }}" << dict << "d" << NoError;

  dict.clear();
  list.clear();
  hash.clear();
  hash.insert( "x", "x" );
  list.append( hash );
  hash.clear();
  hash.insert( "y", "y" );
  list.append( hash );
  hash.clear();
  hash.insert( "3", "d" );
  list.append( hash );

  dict.insert( "1", list );

  QTest::newRow( "basic-syntax32" ) << "{{ 1.2.3 }}" << dict << "d" << NoError;

  dict.clear();

  dict.insert( "1", "abc" );
  QTest::newRow( "basic-syntax33" ) << "{{ 1 }}" << dict << "1" << NoError;
  QTest::newRow( "basic-syntax34" ) << "{{ 1.2 }}" << dict << "1.2" << NoError;

}

void TestBuiltinSyntax::testListIndex_data()
{

  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QVariantList l;
  l << "first item" << "second item";

  dict.insert( "var", l );

  // List-index syntax allows a template to access a certain item of a subscriptable object.
  QTest::newRow( "list-index01" ) << "{{ var.1 }}" << dict << "second item" << NoError;
  // Fail silently when the list index is out of range.
  QTest::newRow( "list-index02" ) << "{{ var.5 }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "var", QVariant() );

  // Fail silently when the variable is not a subscriptable object.
  QTest::newRow( "list-index03" ) << "{{ var.1 }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "var", QVariantHash() );
  // Fail silently when variable is a dict without the specified key.
  QTest::newRow( "list-index04" ) << "{{ var.1 }}" << dict << "" << NoError;


  dict.clear();

  QVariantHash hash;
  hash.insert( "1", "hello" );
  dict.insert( "var", hash );
  // Dictionary lookup wins out when dict's key is a string.
  QTest::newRow( "list-index05" ) << "{{ var.1 }}" << dict << "hello" << NoError;

  // QVariantHash can only use strings as keys, so list-index06 and list-index07 are not valid.
}


void TestBuiltinSyntax::testFilterSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  // Basic filter usage
  dict.insert( "var", "Django is the greatest!" );
  QTest::newRow( "filter-syntax01" ) << "{{ var|upper }}" << dict << "DJANGO IS THE GREATEST!" << NoError;

  // Chained filters
  QTest::newRow( "filter-syntax02" ) << "{{ var|upper|lower }}" << dict << "django is the greatest!" << NoError;

  // Raise TemplateSyntaxError for space between a variable and filter pipe
  dict.clear();
  QTest::newRow( "filter-syntax03" ) << "{{ var |upper }}" << dict << "" << TagSyntaxError;

  // Raise TemplateSyntaxError for space after a filter pipe
  QTest::newRow( "filter-syntax04" ) << "{{ var| upper }}" << dict << "" << TagSyntaxError;

  // Raise TemplateSyntaxError for a nonexistent filter
  QTest::newRow( "filter-syntax05" ) << "{{ var|does_not_exist }}" << dict << "" << UnknownFilterError;

  // Raise TemplateSyntaxError when trying to access a filter containing an illegal character
  QTest::newRow( "filter-syntax06" ) << "{{ var|fil(ter) }}" << dict << "" << UnknownFilterError;

  // Raise TemplateSyntaxError for invalid block tags
  QTest::newRow( "filter-syntax07" ) << "{% nothing_to_see_here %}" << dict << "" << InvalidBlockTagError;
  // Raise TemplateSyntaxError for empty block tags
  QTest::newRow( "filter-syntax08" ) << "{% %}" << dict << "" << InvalidBlockTagError;

  // Chained filters, with an argument to the first one
  dict.insert( "var", "<b><i>Yes</i></b>" );
  QTest::newRow( "filter-syntax09" ) << "{{ var|removetags:\"b i\"|upper|lower }}" << dict << "yes" << NoError;
  // Literal string as argument is always "safe" from auto-escaping..
  dict.clear();
  dict.insert( "var", QVariant() );
  QTest::newRow( "filter-syntax10" ) << "{{ var|default_if_none:\" endquote\\\" hah\" }}" << dict << " endquote\" hah" << NoError;
  // Variable as argument
  dict.insert( "var2", "happy" );
  QTest::newRow( "filter-syntax11" ) << "{{ var|default_if_none:var2 }}" << dict << "happy" << NoError;
  // Default argument testing
  dict.clear();
  dict.insert( "var", true );
  QTest::newRow( "filter-syntax12" ) << "{{ var|yesno:\"yup,nup,mup\" }} {{ var|yesno }}" << dict << "yup yes" << NoError;

  // Fail silently for methods that raise an exception with a
  // "silent_variable_failure" attribute
//   dict.clear();
//   QObject *someClass = new SomeClass(this);
//   dict.insert("var", QVariant::fromValue(someClass));
//   QTest::newRow("filter-syntax13") << "1{{ var.method3 }}2" << dict << "12" << NoError;
//   // In methods that raise an exception without a
//   // "silent_variable_attribute" set to True, the exception propagates
//   // #C# SomeOtherException)
//   QTest::newRow("filter-syntax14") << "var" << dict << "" << TagSyntaxError;

  // Escaped backslash in argument
  dict.clear();
  dict.insert( "var", QVariant() );
  QTest::newRow( "filter-syntax15" ) << "{{ var|default_if_none:\"foo\\bar\" }}" << dict << "foo\\bar" << NoError;
  // Escaped backslash using known escape char
  QTest::newRow( "filter-syntax16" ) << "{{ var|default_if_none:\"foo\\now\" }}" << dict << "foo\\now" << NoError;
  // Empty strings can be passed as arguments to filters
  dict.clear();
  dict.insert( "var", QVariantList() << "a" << "b" << "c" );
  QTest::newRow( "filter-syntax17" ) << "{{ var|join:\"\" }}" << dict << "abc" << NoError;

  // Make sure that any unicode strings are converted to bytestrings
  // in the final output.
//   FAIL'filter-syntax18': (r'{{ var }}', {'var': UTF8Class()}, u'\u0160\u0110\u0106\u017d\u0107\u017e\u0161\u0111'),

  // Numbers as filter arguments should work
  dict.clear();
  dict.insert( "var", "hello world" );
  QTest::newRow( "filter-syntax19" ) << "{{ var|truncatewords:1 }}" << dict << "hello ..." << NoError;
  //filters should accept empty string constants
  dict.clear();
  QTest::newRow( "filter-syntax20" ) << "{{ \"\"|default_if_none:\"was none\" }}" << dict << "" << NoError;

}

void TestBuiltinSyntax::testCommentSyntax_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  QTest::newRow( "comment-syntax01" ) << "{# this is hidden #}hello" << dict << "hello" << NoError;
  QTest::newRow( "comment-syntax02" ) << "{# this is hidden #}hello{# foo #}" << dict << "hello" << NoError;
  // Comments can contain invalid stuff.
  QTest::newRow( "comment-syntax03" ) << "foo{#  {% if %}  #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax04" ) << "foo{#  {% endblock %}  #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax05" ) << "foo{#  {% somerandomtag %}  #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax06" ) << "foo{# {% #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax07" ) << "foo{# %} #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax08" ) << "foo{# %} #}bar" << dict << "foobar" << NoError;
  QTest::newRow( "comment-syntax09" ) << "foo{# {{ #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax10" ) << "foo{# }} #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax11" ) << "foo{# { #}" << dict << "foo" << NoError;
  QTest::newRow( "comment-syntax12" ) << "foo{# } #}" << dict << "foo" << NoError;
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
  dict.insert( "var", "< > & \" \' # = % $" );
  QTest::newRow( "escape01" ) << "{{ var }}" << dict << "&lt; &gt; &amp; \" &#39; # = % $" << NoError;

  dict.clear();
  dict.insert( "var", "this & that" );
  QTest::newRow( "escape02" ) << "{{ var }}" << dict << "this &amp; that" << NoError;

  // Strings are compared unescaped.
  QTest::newRow( "escape03" ) << "{% ifequal var \"this & that\" %}yes{% endifequal %}" << dict << "yes" << NoError;

  // Arguments to filters are 'safe' and manipulate their input unescaped.
  QTest::newRow( "escape04" ) << "{{ var|cut:\"&\" }}" << dict << "this  that" << NoError;

  dict.insert( "varList", QVariantList() << "Tom" << "Dick" << "Harry" );
  QTest::newRow( "escape05" ) << "{{ varList|join:\" & \" }}" << dict << "Tom & Dick & Harry" << NoError;

  // Same with variable args.
  dict.insert( "amp", " & " );
  QTest::newRow( "escape06" ) << "{{ varList|join:amp }}" << dict << "Tom & Dick & Harry" << NoError;

  // Literal strings are safe.
  QTest::newRow( "escape07" ) << "{{ \"this & that\" }}" << dict << "this & that" << NoError;

  // Iterating outputs safe characters.
  dict.clear();
  QVariantList list;
  list << "K" << "&" << "R";
  dict.insert( "list", list );
  QTest::newRow( "escape08" ) << "{% for letter in list %}{{ letter }},{% endfor %}" << dict << "K,&amp;,R," << NoError;

  dict.clear();
  // escape requirement survives lookup.
  QVariantHash hash;
  hash.insert( "key", "this & that" );
  dict.insert( "var", hash );
  QTest::newRow( "escape09" ) << "{{ var.key }}" << dict << "this &amp; that" << NoError;

  dict.clear();

}

void TestBuiltinSyntax::testMultipleStates()
{
  Engine *engine1 = getEngine();

  InMemoryTemplateLoader::Ptr loader1 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader1->setTemplate( "template1", "Template 1" );
  engine1->addTemplateLoader( loader1 );

  Template t1 = engine1->newTemplate( "{% include \"template1\" %}", "\"template1\"" );

  Engine *engine2 = getEngine();

  InMemoryTemplateLoader::Ptr loader2 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader2->setTemplate( "template2", "Template 2" );

  engine2->addTemplateLoader( loader2 );

  Template t2 = engine2->newTemplate( "{% include \"template2\" %}", "\"template2\"" );

  Engine *engine3 = getEngine();

  InMemoryTemplateLoader::Ptr loader3 = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );

  loader3->setTemplate( "template3", "Template 3" );

  engine3->addTemplateLoader( loader3 );

  Template t3 = engine3->newTemplate( "{% include var %}", "var" );

  QVariantHash h;
  h.insert( "var", "template3" );
  Context c( h );
  t1->render( &c );

  QString expected1 = "Template 1";
  QString expected2 = "Template 2";
  QString expected3 = "Template 3";
  QCOMPARE( t1->render( &c ), expected1 );
  QCOMPARE( t2->render( &c ), expected2 );
  QCOMPARE( t3->render( &c ), expected3 );
}

void TestBuiltinSyntax::testAlternativeEscaping()
{
  Engine *engine1 = getEngine();

  Template t1 = engine1->newTemplate( "{{ var }}", "\"template1\"" );

  QString input = "< > \r\n & \" \' # = % $";

  QVariantHash h;
  h.insert( "var", input );
  Context c( h );

  QString output;
  QTextStream ts(&output);
  QSharedPointer<OutputStream> noEscapeOs = QSharedPointer<OutputStream>( new NoEscapeOutputStream( &ts ) );

  t1->render( noEscapeOs.data(), &c );

  QCOMPARE( output, input );
  output.clear();

  QSharedPointer<OutputStream> jsOs = QSharedPointer<OutputStream>( new JSOutputStream( &ts ) );

  t1->render( jsOs.data(), &c );

  QString jsOutput("\\x3C \\x3E \\x0D\\x0A \\x26 \\x22 \\x27 # \\x3D % $");

  QCOMPARE( output, jsOutput );
}


void TestBuiltinSyntax::testTemplatePathSafety_data()
{
  QTest::addColumn<QString>( "inputPath" );
  QTest::addColumn<QString>( "output" );

  QTest::newRow( "template-path-safety01" ) << "visible_file" << "visible_file";
  QTest::newRow( "template-path-safety02" ) << "../invisible_file" << "";
}

void TestBuiltinSyntax::testTemplatePathSafety()
{
  QFETCH( QString, inputPath );
  QFETCH( QString, output );

  Grantlee::FileSystemTemplateLoader *loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs( QStringList() << "." );

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

  QTest::newRow( "media-path-safety01" ) << "visible_file" << "./visible_file";
  QTest::newRow( "media-path-safety02" ) << "../invisible_file" << "";
}

void TestBuiltinSyntax::testMediaPathSafety()
{
  QFETCH( QString, inputPath );
  QFETCH( QString, output );

  Grantlee::FileSystemTemplateLoader *loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs( QStringList() << "." );

  QFile f( inputPath );
  bool opened = f.open( QFile::WriteOnly | QFile::Text );
  QVERIFY( opened );
  f.write( inputPath.toUtf8() );
  f.close();

  QString uri = loader->getMediaUri( inputPath );
  if ( output.isEmpty() )
    QVERIFY( uri.isEmpty() );
  else
    QCOMPARE( QFileInfo( uri ).absoluteFilePath(), QFileInfo( output ).absoluteFilePath() );

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
  itemsHash.insert("one", 1);
  itemsHash.insert("two", 2);
  itemsHash.insert("three", 3);

  dict.insert( "hash", itemsHash );

  QTest::newRow( "type-accessors-hash01" ) << "{{ hash.items|length }}" << dict << "3" << NoError;
  QTest::newRow( "type-accessors-hash02" ) << "{% for key,value in hash.items %}{{ key }}:{{ value }};{% endfor %}"
                                      << dict << "one:1;two:2;three:3;" << NoError;
  QTest::newRow( "type-accessors-hash03" ) << "{{ hash.keys|length }}" << dict << "3" << NoError;
  QTest::newRow( "type-accessors-hash04" ) << "{% for key in hash.keys %}{{ key }};{% endfor %}"
                                      << dict << "one;two;three;" << NoError;
  QTest::newRow( "type-accessors-hash05" ) << "{{ hash.values|length }}" << dict << "3" << NoError;
  QTest::newRow( "type-accessors-hash06" ) << "{% for value in hash.values %}{{ value }};{% endfor %}"
                                      << dict << "1;2;3;" << NoError;

  dict.clear();
  dict.insert( "str1", "my string" );
  dict.insert( "str2", "mystring" );

  QTest::newRow( "type-accessors-string01" ) << "{{ str1.capitalize }}" << dict << "My string" << NoError;
  QTest::newRow( "type-accessors-string02" ) << "{{ str2.capitalize }}" << dict << "Mystring" << NoError;

  dict.clear();
  dict.insert( "str1", "de24335fre" );
  dict.insert( "str2", "de435f3.-5r" );

  QTest::newRow( "type-accessors-string03" ) << "{{ str1.isalnum }}" << dict << "True" << NoError;
  QTest::newRow( "type-accessors-string04" ) << "{{ str2.isalnum }}" << dict << "False" << NoError;

  dict.clear();
  dict.insert( "str1", "24335" );
  dict.insert( "str2", "de435f35r" );
  dict.insert( "str3", "de435f3.-5r" );

  QTest::newRow( "type-accessors-string05" ) << "{{ str1.isdigit }}" << dict << "True" << NoError;
  QTest::newRow( "type-accessors-string06" ) << "{{ str2.isdigit }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string07" ) << "{{ str3.isdigit }}" << dict << "False" << NoError;

  dict.clear();
  dict.insert( "str", "MyString" );
  dict.insert( "lowerStr", "mystring" );

  QTest::newRow( "type-accessors-string08" ) << "{{ str.islower }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string09" ) << "{{ lowerStr.islower }}" << dict << "True" << NoError;

  dict.clear();
  dict.insert( "str1", "        ");
  dict.insert( "str2", "     r  ");
  dict.insert( "str3", " \t\nr  ");
  dict.insert( "str4", " \t\n   ");

  QTest::newRow( "type-accessors-string10" ) << "{{ str1.isspace }}" << dict << "True" << NoError;
  QTest::newRow( "type-accessors-string11" ) << "{{ str2.isspace }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string12" ) << "{{ str3.isspace }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string13" ) << "{{ str4.isspace }}" << dict << "True" << NoError;

  dict.clear();
  dict.insert( "str1", "My String" );
  dict.insert( "str2", "Mystring" );
  dict.insert( "str3", "My string" );
  dict.insert( "str4", "my string" );

  QTest::newRow( "type-accessors-string14" ) << "{{ str1.istitle }}" << dict << "True" << NoError;
  QTest::newRow( "type-accessors-string15" ) << "{{ str2.istitle }}" << dict << "True" << NoError;
  QTest::newRow( "type-accessors-string16" ) << "{{ str3.istitle }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string17" ) << "{{ str4.istitle }}" << dict << "False" << NoError;

  dict.clear();
  dict.insert( "str", "MyString" );
  dict.insert( "upperStr", "MYSTRING" );

  QTest::newRow( "type-accessors-string18" ) << "{{ str.isupper }}" << dict << "False" << NoError;
  QTest::newRow( "type-accessors-string19" ) << "{{ upperStr.isupper }}" << dict << "True" << NoError;

  dict.clear();
  dict.insert( "str1", "My String" );
  dict.insert( "str2", "MYSTRING" );
  dict.insert( "str3", "MY STRING" );

  QTest::newRow( "type-accessors-string20" ) << "{{ str1.lower }}" << dict << "my string" << NoError;
  QTest::newRow( "type-accessors-string21" ) << "{{ str2.lower }}" << dict << "mystring" << NoError;
  QTest::newRow( "type-accessors-string22" ) << "{{ str3.lower }}" << dict << "my string" << NoError;

  dict.clear();
  dict.insert( "str", "one\ntwo three\nfour" );

  QTest::newRow( "type-accessors-string23" ) << "{% for line in str.splitlines %}{{ line }};{% endfor %}" << dict << "one;two three;four;" << NoError;

  dict.clear();
  dict.insert( "str1", "          one" );
  dict.insert( "str2", "     one     " );
  dict.insert( "str3", "one          " );
  dict.insert( "str4", "             " );
  dict.insert( "str5", "" );

  QTest::newRow( "type-accessors-string24" ) << "{{ str1.strip }}" << dict << "one" << NoError;
  QTest::newRow( "type-accessors-string25" ) << "{{ str2.strip }}" << dict << "one" << NoError;
  QTest::newRow( "type-accessors-string26" ) << "{{ str3.strip }}" << dict << "one" << NoError;
  QTest::newRow( "type-accessors-string27" ) << "{{ str4.strip }}" << dict << "" << NoError;
  QTest::newRow( "type-accessors-string28" ) << "{{ str5.strip }}" << dict << "" << NoError;

  dict.clear();
  dict.insert( "str1", "My String" );
  dict.insert( "str2", "mY sTRING" );
  dict.insert( "str3", "My StrInG" );
  dict.insert( "str4", "my string" );
  dict.insert( "str5", "MY STRING" );

  // Yes, this really is a python built-in.
  QTest::newRow( "type-accessors-string29" ) << "{{ str1.swapcase }}" << dict << "mY sTRING" << NoError;
  QTest::newRow( "type-accessors-string30" ) << "{{ str2.swapcase }}" << dict << "My String" << NoError;
  QTest::newRow( "type-accessors-string31" ) << "{{ str3.swapcase }}" << dict << "mY sTRiNg" << NoError;
  QTest::newRow( "type-accessors-string32" ) << "{{ str4.swapcase }}" << dict << "MY STRING" << NoError;
  QTest::newRow( "type-accessors-string33" ) << "{{ str5.swapcase }}" << dict << "my string" << NoError;

  dict.clear();
  dict.insert( "str1", "My String" );
  dict.insert( "str2", "mystring" );
  dict.insert( "str3", "my string" );
  dict.insert( "str4", "my String" );
  dict.insert( "str5", "My string" );

  QTest::newRow( "type-accessors-string34" ) << "{{ str1.title }}" << dict << "My String" << NoError;
  QTest::newRow( "type-accessors-string35" ) << "{{ str2.title }}" << dict << "Mystring" << NoError;
  QTest::newRow( "type-accessors-string36" ) << "{{ str3.title }}" << dict << "My String" << NoError;
  QTest::newRow( "type-accessors-string37" ) << "{{ str4.title }}" << dict << "My String" << NoError;
  QTest::newRow( "type-accessors-string38" ) << "{{ str5.title }}" << dict << "My String" << NoError;

  QTest::newRow( "type-accessors-string39" ) << "{{ str1.upper }}" << dict << "MY STRING" << NoError;
  QTest::newRow( "type-accessors-string40" ) << "{{ str2.upper }}" << dict << "MYSTRING" << NoError;
  QTest::newRow( "type-accessors-string41" ) << "{{ str3.upper }}" << dict << "MY STRING" << NoError;

}


QTEST_MAIN( TestBuiltinSyntax )
#include "testbuiltins.moc"

#endif

