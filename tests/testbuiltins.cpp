/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef BUILTINSTEST_H
#define BUILTINSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "context.h"
#include "grantlee.h"
#include "filterexpression.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Dict)
Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

/**
  For use with tests.
*/
class OtherClass : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString method READ method)

public:
  OtherClass(QObject *parent = 0)
    : QObject(parent)
  { }

  QString method() const
  { return "OtherClass::method"; }

};

/**
  For use with tests.
*/
class SomeClass : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString method READ method)
  Q_PROPERTY(QVariant otherClass READ otherClass)

public:
  SomeClass(QObject *parent = 0)
    : QObject(parent)
  { }

  QString method() const
  { return "SomeClass::method"; }

  QVariant otherClass() const
  { QObject *o = new OtherClass(); return QVariant::fromValue(o); }

  QString nonAccessibleMethod(const QString &str)
  { return str; }
};

class TestBuiltinSyntax : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() { doTest(); }

  void testListIndex_data();
  void testListIndex() { doTest(); }

  void testFilterSyntax_data();
  void testFilterSyntax() { QSKIP("Haven't even attempted to implement it yet", SkipSingle); /* doTest(); */ }

  void testCommentSyntax_data();
  void testCommentSyntax() { doTest(); }

private:

  void doTest();

  TemplateLoader *m_tl;

};

void TestBuiltinSyntax::initTestCase()
{
  m_tl = TemplateLoader::instance();

  m_tl->setPluginDirs(QStringList() << "/home/kde-devel/kde/lib/");
}

void TestBuiltinSyntax::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);


  Template* t = m_tl->getTemplate(this);
  QSignalSpy spy(t, SIGNAL(error(int, QString)));
  t->setContent(input);

  if (spy.count() > 0)
  {
    QVariantList l = spy.takeFirst();
    QCOMPARE(l.at(0).toInt(), (int)error );
    return;
  }
  
  Context context(dict);
  
  QString result = t->render(&context);

  if (spy.count() > 0)
  {
    QVariantList l = spy.takeFirst();
    QCOMPARE(l.at(0).toInt(), (int)error );
    QCOMPARE(result, QString());
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  QCOMPARE(result, output);
}

void TestBuiltinSyntax::testBasicSyntax_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  // Plain text should go through the template parser untouched
  QTest::newRow("basic-syntax01") << "something cool" << dict << "something cool" << NoError;

  // Variables should be replaced with their value in the current
  // context
  dict.insert("headline", "Success");
  QTest::newRow("basic-syntax02") << "{{ headline }}" << dict << "Success" << NoError;

  dict.clear();
  dict.insert("first", 1);
  dict.insert("second", 2);

  // More than one replacement variable is allowed in a template
  QTest::newRow("basic-syntax03") << "{{ first }} --- {{ second }}" << dict << "1 --- 2" << NoError;

  dict.clear();
  // Fail silently when a variable is not found in the current context
  QTest::newRow("basic-syntax04") << "as{{ missing }}df" << dict << "asdf" << NoError;

  // A variable may not contain more than one word
  QTest::newRow("basic-syntax06") << "{{ multi word variable }}" << dict << "" << TagSyntaxError;
  // Raise TemplateSyntaxError for empty variable tags
  QTest::newRow("basic-syntax07") << "{{ }}" << dict << "" << TagSyntaxError;
  QTest::newRow("basic-syntax08") << "{{        }}" << dict << "" << TagSyntaxError;


  // Attribute syntax allows a template to call an object's attribute

  QObject *someClass = new SomeClass(this);
  dict.insert("var", QVariant::fromValue(someClass));

  QTest::newRow("basic-syntax09") << "{{ var.method }}" << dict << "SomeClass::method" << NoError;

  // Multiple levels of attribute access are allowed
  QTest::newRow("basic-syntax10") << "{{ var.otherClass.method }}" << dict << "OtherClass::method" << NoError;

  // Fail silently when a variable's attribute isn't found
  QTest::newRow("basic-syntax11") << "{{ var.blech }}" << dict << "" << NoError;

  // TODO: Needed?
  // Raise TemplateSyntaxError when trying to access a variable beginning with an underscore
  // #C# {"var": SomeClass()}
//   QTest::newRow("basic-syntax12") << "{{ var.__dict__ }}" << dict << " template.TemplateSyntaxError),";

  dict.clear();
  // Raise TemplateSyntaxError when trying to access a variable containing an illegal character
  QTest::newRow("basic-syntax13") << "{{ va>r }}" << dict << "" << TagSyntaxError;
  QTest::newRow("basic-syntax14") << "{{ (var.r) }}" << dict << "" << TagSyntaxError;
  QTest::newRow("basic-syntax15") << "{{ sp%am }}" << dict << "" << TagSyntaxError;
  QTest::newRow("basic-syntax16") << "{{ eggs! }}" << dict << "" << TagSyntaxError;
  QTest::newRow("basic-syntax17") << "{{ moo? }}" << dict << "" << TagSyntaxError;

  // Attribute syntax allows a template to call a dictionary key's value

  QMap<QString, QVariant> map;
  map.insert("bar", "baz");
  dict.insert("foo", map);
  QTest::newRow("basic-syntax18") << "{{ foo.bar }}" << dict << "baz" << NoError;

  // Fail silently when a variable's dictionary key isn't found
  QTest::newRow("basic-syntax19") << "{{ foo.spam }}" << dict << "" << NoError;

  dict.clear();
  dict.insert("var", QVariant::fromValue(someClass));
  // Fail silently when attempting to access an unavailable method
  QTest::newRow("basic-syntax20") << "{{ var.nonAccessibleMethod }}" << dict << "" << NoError;

  dict.clear();
  // Don't get confused when parsing something that is almost, but not
  // quite, a template tag.
  QTest::newRow("basic-syntax21") << "a {{ moo %} b" << dict << "a {{ moo %} b" << NoError;
  QTest::newRow("basic-syntax22") << "{{ moo #}" << dict << "{{ moo #}" << NoError;

  dict.insert("cow", "cow");
  // Will try to treat "moo #} {{ cow" as the variable. Not ideal, but
  // costly to work around, so this triggers an error.
  QTest::newRow("basic-syntax23") << "{{ moo #} {{ cow }}" << dict << "" << TagSyntaxError;

  dict.clear();
  // Embedded newlines make it not-a-tag.
  QTest::newRow("basic-syntax24") << "{{ moo\n }}" << dict << "{{ moo\\n }}" << NoError;
  // Literal strings are permitted inside variables, mostly for i18n
  // purposes.
  QTest::newRow("basic-syntax25") << "{{ \"fred\" }}" << dict << "fred" << NoError;
  QTest::newRow("basic-syntax26") << "{{ \"\\\"fred\\\"\" }}" << dict << "\\\"fred\\\"" << NoError;
//   QTest::newRow("basic-syntax27") << "{{ _(\"\\\"fred\\\"\") }" << dict << "\\\"fred\\\"" << NoError;


}

void TestBuiltinSyntax::testListIndex_data()
{

  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantList l;
  l << "first item" << "second item";

  dict.insert("var", l);

  // List-index syntax allows a template to access a certain item of a subscriptable object.
  QTest::newRow("list-index01") << "{{ var.0 }}" << dict << "first item" << NoError;
  QTest::newRow("list-index01") << "{{ var.1 }}" << dict << "second item" << NoError;
  // Fail silently when the list index is out of range.
  QTest::newRow("list-index02") << "{{ var.5 }}" << dict << "" << NoError;

  dict.clear();
  dict.insert("var", QVariant());

  // Fail silently when the variable is not a subscriptable object.
  QTest::newRow("list-index03") << "{{ var.1 }}" << dict << "" << NoError;

  dict.clear();
  dict.insert("var", QVariantMap());
  // Fail silently when variable is a dict without the specified key.
  QTest::newRow("list-index04") << "{{ var.1 }}" << dict << "" << NoError;


  dict.clear();

  QVariantMap map;
  map.insert("1", "hello");
  dict.insert("var", map);
  // Dictionary lookup wins out when dict's key is a string.
  QTest::newRow("list-index05") << "{{ var.1 }}" << dict << "hello" << NoError;

  // QVariantMap can only use strings as keys, so list-index06 and list-index07 are not valid.
}


void TestBuiltinSyntax::testFilterSyntax_data()
{
//   // Basic filter usage
}

void TestBuiltinSyntax::testCommentSyntax_data()
{

  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("comment-syntax01") << "{# this is hidden #}hello" << dict << "hello" << NoError;
  QTest::newRow("comment-syntax02") << "{# this is hidden #}hello{# foo #}" << dict << "hello" << NoError;
  // Comments can contain invalid stuff.
  QTest::newRow("comment-syntax03") << "foo{#  {% if %}  #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax04") << "foo{#  {% endblock %}  #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax05") << "foo{#  {% somerandomtag %}  #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax06") << "foo{# {% #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax07") << "foo{# %} #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax08") << "foo{# %} #}bar" << dict << "foobar" << NoError;
  QTest::newRow("comment-syntax09") << "foo{# {{ #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax10") << "foo{# }} #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax11") << "foo{# { #}" << dict << "foo" << NoError;
  QTest::newRow("comment-syntax12") << "foo{# } #}" << dict << "foo" << NoError;
}

QTEST_MAIN(TestBuiltinSyntax)
#include "testbuiltins.moc"

#endif
