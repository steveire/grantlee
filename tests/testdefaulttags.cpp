/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef DEFAULTTAGSTEST_H
#define DEFAULTTAGSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "context.h"

#include "grantlee.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Dict)
Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestDefaultTags : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testCommentTag_data();
  void testCommentTag() { doTest(); }

  void testFirstOfTag_data();
  void testFirstOfTag() {  doTest(); }

  void testIfTag_data();
  void testIfTag() {  doTest();  }

  void testForTag_data();
  void testForTag() {  doTest();  }

  void testIfEqualTag_data();
  void testIfEqualTag() {  doTest();  }

  void testIfNotEqualTag_data();
  void testIfNotEqualTag() {  doTest();  }

  void testTemplateTagTag_data();
  void testTemplateTagTag() {  doTest();  }

  void testWithTag_data();
  void testWithTag() {  doTest();  }

  void testCycleTag_data();
  void testCycleTag() {  /* doTest(); */ }

  void testWidthRatioTag_data();
  void testWidthRatioTag() {  doTest();  }

  void testFilterTag_data();
  void testFilterTag() {  doTest();  }

  void testNowTag_data();
  void testNowTag() {  doTest();  }

  void testSpacelessTag_data();
  void testSpacelessTag() {  doTest();  }

private:

  void doTest();

  TemplateLoader *m_tl;

};

void TestDefaultTags::initTestCase()
{
  m_tl = TemplateLoader::instance();

  m_tl->setPluginDirs(QStringList() << "/home/kde-devel/kde/lib/");

}

void TestDefaultTags::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);

  Template* t = m_tl->getTemplate();

  QSignalSpy spy(t, SIGNAL(error(int, const QString &)));
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

void TestDefaultTags::testCommentTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("comment-tag01") << "{% comment %}this is hidden{% endcomment %}hello" << dict << "hello" << NoError;

  QTest::newRow("comment-tag02") << "{% comment %}this is hidden{% endcomment %}hello{% comment %}foo{% endcomment %}" << dict << "hello" << NoError;
  // Comment tag can contain invalid stuff.
  QTest::newRow("comment-tag03") << "foo{% comment %} {% if %} {% endcomment %}" << dict << "foo" << NoError;
  QTest::newRow("comment-tag04") << "foo{% comment %} {% endblock %} {% endcomment %}" << dict << "foo" << NoError;
  QTest::newRow("comment-tag05") << "foo{% comment %} {% somerandomtag %} {% endcomment %}" << dict << "foo" << NoError;
}

void TestDefaultTags::testFirstOfTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  dict.insert("a", 0);
  dict.insert("b", 0);
  dict.insert("c", 0);
  QTest::newRow("firstof01") << "{% firstof a b c %}" << dict << "" << NoError;

  dict.clear();
  dict.insert("a", 1);
  dict.insert("b", 0);
  dict.insert("c", 0);
  QTest::newRow("firstof02") << "{% firstof a b c %}" << dict << "1" << NoError;

  dict.clear();
  dict.insert("a", 0);
  dict.insert("b", 2);
  dict.insert("c", 0);
  QTest::newRow("firstof03") << "{% firstof a b c %}" << dict << "2" << NoError;

  dict.clear();
  dict.insert("a", 0);
  dict.insert("b", 0);
  dict.insert("c", 3);
  QTest::newRow("firstof04") << "{% firstof a b c %}" << dict << "3" << NoError;

  dict.clear();
  dict.insert("a", 1);
  dict.insert("b", 2);
  dict.insert("c", 3);
  QTest::newRow("firstof05") << "{% firstof a b c %}" << dict << "1" << NoError;

  dict.clear();
  dict.insert("b", 0);
  dict.insert("c", 3);
  QTest::newRow("firstof06") << "{% firstof a b c %}" << dict << "3" << NoError;

  dict.clear();
  dict.insert("a", 0);
  QTest::newRow("firstof07") << "{% firstof a b \"c\" %}" << dict << "c" << NoError;

  dict.clear();
  dict.insert("a", 0);
  dict.insert("b", 0);
  QTest::newRow("firstof08") << "{% firstof a b \"c and d\" %}" << dict << "c and d" << NoError;
  QTest::newRow("firstof09") << "{% firstof %}" << dict << "a" << TagSyntaxError;

}


void TestDefaultTags::testIfTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert("foo", true);

  QTest::newRow("if-tag01") << "{% if foo %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  QTest::newRow("if-tag02") << "{% if foo %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow("if-tag03") << "{% if foo %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  // AND

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-and01") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-and02") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-and03") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-and04") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  QTest::newRow("if-tag-and05") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("bar", false);
  QTest::newRow("if-tag-and06") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  QTest::newRow("if-tag-and07") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("bar", true);
  QTest::newRow("if-tag-and08") << "{% if foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  // OR

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-or01") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-or02") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-or03") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-or04") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  QTest::newRow("if-tag-or05") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("bar", false);
  QTest::newRow("if-tag-or06") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  QTest::newRow("if-tag-or07") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("bar", true);
  QTest::newRow("if-tag-or08") << "{% if foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  // TODO: multiple ORs


  // NOT

  dict.clear();
  dict.insert("foo", true);
  QTest::newRow("if-tag-not01") << "{% if not foo %}no{% else %}yes{% endif %}" << dict << "yes" << NoError;
  QTest::newRow("if-tag-not02") << "{% if not %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("not", true);
  QTest::newRow("if-tag-not03") << "{% if not %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;
  QTest::newRow("if-tag-not04") << "{% if not not %}no{% else %}yes{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not05") << "{% if not not %}no{% else %}yes{% endif %}" << dict << "no" << NoError;
  QTest::newRow("if-tag-not06") << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not07") << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not08") << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not09") << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not10") << "{% if foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not11") << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not12") << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not13") << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not14") << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not15") << "{% if not foo and bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not16") << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not17") << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not18") << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not19") << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not20") << "{% if foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not21") << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not22") << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not23") << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not24") << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not25") << "{% if not foo or bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not26") << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not27") << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not28") << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not29") << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not30") << "{% if not foo and not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  QTest::newRow("if-tag-not31") << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not32") << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("foo", true);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not33") << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", true);
  QTest::newRow("if-tag-not34") << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("foo", false);
  dict.insert("bar", false);
  QTest::newRow("if-tag-not35") << "{% if not foo or not bar %}yes{% else %}no{% endif %}" << dict << "yes" << NoError;

  // AND and OR raises a TemplateSyntaxError
  QTest::newRow("if-tag-error01") << "{% if foo or bar and baz %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;

  dict.clear();
  dict.insert("foo", true);
  QTest::newRow("if-tag-error02") << "{% if foo and %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow("if-tag-error03") << "{% if foo or %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow("if-tag-error04") << "{% if not foo and %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;
  QTest::newRow("if-tag-error05") << "{% if not foo or %}yes{% else %}no{% endif %}" << dict << "" << TagSyntaxError;

}

void TestDefaultTags::testForTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantList list;
  list << 1 << 2 << 3;
  dict.insert("values", list);
  QTest::newRow("for-tag01") << "{% for val in values %}{{ val }}{% endfor %}" << dict << "123" << NoError;
  QTest::newRow("for-tag02") << "{% for val in values reversed %}{{ val }}{% endfor %}" << dict << "321" << NoError;
  list.clear();
  list << 6 << 6 << 6;
  QTest::newRow("for-tag-vars01") << "{% for val in values %}{{ forloop.counter }}{% endfor %}" << dict << "123" << NoError;
  QTest::newRow("for-tag-vars02") << "{% for val in values %}{{ forloop.counter0 }}{% endfor %}" << dict << "012" << NoError;
  QTest::newRow("for-tag-vars03") << "{% for val in values %}{{ forloop.revcounter }}{% endfor %}" << dict << "321" << NoError;
  QTest::newRow("for-tag-vars04") << "{% for val in values %}{{ forloop.revcounter0 }}{% endfor %}" << dict << "210" << NoError;
  QTest::newRow("for-tag-vars05") << "{% for val in values %}{% if forloop.first %}f{% else %}x{% endif %}{% endfor %}" << dict << "fxx" << NoError;
  QTest::newRow("for-tag-vars06") << "{% for val in values %}{% if forloop.last %}l{% else %}x{% endif %}{% endfor %}" << dict << "xxl" << NoError;
}

void TestDefaultTags::testIfEqualTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert("a", 1);
  dict.insert("b", 2);

  QTest::newRow("ifequal01") << "{% ifequal a b %}yes{% endifequal %}" << dict << "" << NoError;
  QTest::newRow("ifequal03") << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("a", 1);
  dict.insert("b", 1);

  QTest::newRow("ifequal02") << "{% ifequal a b %}yes{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow("ifequal04") << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "test");

  QTest::newRow("ifequal05") << "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "no");

  QTest::newRow("ifequal06") << "{% ifequal a 'test' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("a", "test");

  QTest::newRow("ifequal07") << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "no");

  QTest::newRow("ifequal08") << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();

  QTest::newRow("ifequal09") << "{% ifequal a \"test\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  QTest::newRow("ifequal10") << "{% ifequal a b %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;


  QTest::newRow("ifequal-split01") << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.insert("a", "foo");
  QTest::newRow("ifequal-split01") << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;
  QTest::newRow("ifequal-split02") << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("a", "test man");
  QTest::newRow("ifequal-split03") << "{% ifequal a \"test man\" %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow("ifequal-split04") << "{% ifequal a 'test man' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "");
  QTest::newRow("ifequal-split05") << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("a", "i \"love\" you");
  QTest::newRow("ifequal-split06") << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "i love you");
  QTest::newRow("ifequal-split07") << "{% ifequal a 'i \"love\" you' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;

  dict.clear();
  dict.insert("a", "I'm happy");
  QTest::newRow("ifequal-split08") << "{% ifequal a 'I\\'m happy' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "slash\\man");
  QTest::newRow("ifequal-split09") << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", "slashman");
  QTest::newRow("ifequal-split10") << "{% ifequal a 'slash\\man' %}yes{% else %}no{% endifequal %}" << dict << "no" << NoError;
// NUMERIC RESOLUTION

  dict.clear();
  dict.insert("x", "5");

  QTest::newRow("ifequal-numeric01") << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert("x", 5);
  QTest::newRow("ifequal-numeric02") << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", 5.2);
  QTest::newRow("ifequal-numeric03") << "{% ifequal x 5 %}yes{% endifequal %}" << dict << "" << NoError;
  QTest::newRow("ifequal-numeric04") << "{% ifequal x 5.2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", .2);

  QTest::newRow("ifequal-numeric05") << "{% ifequal x 0.2 %}yes{% endifequal %}" << dict << "yes" << NoError;
  QTest::newRow("ifequal-numeric06") << "{% ifequal x .2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", 2);

  QTest::newRow("ifequal-numeric07") << "{% ifequal x 2. %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert("x", 5);
  QTest::newRow("ifequal-numeric08") << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << "" << NoError;

  dict.clear();
  dict.insert("x", "5");
  QTest::newRow("ifequal-numeric09") << "{% ifequal x \"5\" %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", -5);
  QTest::newRow("ifequal-numeric10") << "{% ifequal x -5 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", -5.2);
  QTest::newRow("ifequal-numeric11") << "{% ifequal x -5.2 %}yes{% endifequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("x", 5);
  QTest::newRow("ifequal-numeric12") << "{% ifequal x +5 %}yes{% endifequal %}" << dict << "yes" << NoError;

}

void TestDefaultTags::testIfNotEqualTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert("a", 1);
  dict.insert("b", 2);

  QTest::newRow("ifnotequal01") << "{% ifnotequal a b %}yes{% endifnotequal %}" << dict << "yes" << NoError;
  QTest::newRow("ifnotequal03") << "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" << dict << "yes" << NoError;

  dict.clear();
  dict.insert("a", 1);
  dict.insert("b", 1);

  QTest::newRow("ifnotequal02") << "{% ifnotequal a b %}yes{% endifnotequal %}" << dict << "" << NoError;
  QTest::newRow("ifnotequal04") << "{% ifnotequal a b %}yes{% else %}no{% endifnotequal %}" << dict << "no" << NoError;

}

void TestDefaultTags::testTemplateTagTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("templatetag01") << "{% templatetag openblock %}" << dict << "{%" << NoError;
  QTest::newRow("templatetag02") << "{% templatetag closeblock %}" << dict << "%}" << NoError;
  QTest::newRow("templatetag03") << "{% templatetag openvariable %}" << dict << "{{" << NoError;
  QTest::newRow("templatetag04") << "{% templatetag closevariable %}" << dict << "}}" << NoError;
  QTest::newRow("templatetag05") << "{% templatetag %}" << dict << "" << TagSyntaxError;
  QTest::newRow("templatetag06") << "{% templatetag foo %}" << dict << "" << TagSyntaxError;
  QTest::newRow("templatetag07") << "{% templatetag openbrace %}" << dict << "{" << NoError;
  QTest::newRow("templatetag08") << "{% templatetag closebrace %}" << dict << "}" << NoError;
  QTest::newRow("templatetag09") << "{% templatetag openbrace %}{% templatetag openbrace %}" << dict << "{{" << NoError;
  QTest::newRow("templatetag10") << "{% templatetag closebrace %}{% templatetag closebrace %}" << dict << "}}" << NoError;
  QTest::newRow("templatetag11") << "{% templatetag opencomment %}" << dict << "{#" << NoError;
  QTest::newRow("templatetag12") << "{% templatetag closecomment %}" << dict << "#}" << NoError;

}

void TestDefaultTags::testWithTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantMap map;
  map.insert("key", 50);
  dict.insert("dict", map);
  QTest::newRow("with01") << "{% with dict.key as key %}{{ key }}{% endwith %}" << dict << "50" << NoError;
  QTest::newRow("with02")
        << "{{ key }}{% with dict.key as key %}{{ key }}-{{ dict.key }}-{{ key }}{% endwith %}{{ key }}"
        << dict << "50-50-50" << NoError;
  QTest::newRow("with03") << "{% with dict.key xx key %}{{ key }}{% endwith %}" << dict << "" << TagSyntaxError;
  QTest::newRow("with04") << "{% with dict.key as %}{{ key }}{% endwith %}" << dict << "" << TagSyntaxError;
}

void TestDefaultTags::testCycleTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("cycle01") << "{% cycle a %}" << dict << "" << TagSyntaxError;
  QTest::newRow("cycle02") << "{% cycle a,b,c as abc %}{% cycle abc %}" << dict << "ab" << NoError;
  QTest::newRow("cycle03") << "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}" << dict << "abc" << NoError;
  QTest::newRow("cycle04") << "{% cycle a,b,c as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" << dict << "abca" << NoError;
  QTest::newRow("cycle05") << "{% cycle a %}" << dict << "" << TagSyntaxError;
  // TODO: This is the same as cycle01. Remove.
  QTest::newRow("cycle06") << "{% cycle a %}" << dict << "" << TagSyntaxError;
  QTest::newRow("cycle06") << "{% cycle a,b,c as foo %}{% cycle bar %}" << dict << "" << TagSyntaxError;
  QTest::newRow("cycle06") << "{% cycle a,b,c as foo %}{% cycle foo %}{{ foo }}{{ foo }}{% cycle foo %}{{ foo }}" << dict << "abbbcc" << TagSyntaxError;

  dict.insert("test", QVariantList() << 0 << 1 << 2 << 3 << 4);
  QTest::newRow("cycle09") << "{% for i in test %}{% cycle a,b %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," << NoError;

  dict.clear();
  QTest::newRow("cycle10") << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}" << dict << "ab" << NoError;
  QTest::newRow("cycle11") << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}" << dict << "abc" << NoError;
  QTest::newRow("cycle12") << "{% cycle 'a' 'b' 'c' as abc %}{% cycle abc %}{% cycle abc %}{% cycle abc %}" << dict << "abca" << NoError;

  dict.insert("test", QVariantList() << 0 << 1 << 2 << 3 << 4);
  QTest::newRow("cycle13") << "{% for i in test %}{% cycle 'a' 'b' %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," << NoError;

  dict.clear();
  dict.insert("one", "1");
  dict.insert("two", "2");
  QTest::newRow("cycle14") << "{% cycle one two as foo %}{% cycle foo %}" << dict << "12" << NoError;
  dict.insert("test", QVariantList() << 0 << 1 << 2 << 3 << 4);
  // TODO: Same name as cycle13 in Django. Fix upstream.
  //  'aye': 'a', 'bee': 'b'}, '
//   QTest::newRow("cycle15") << "{% for i in test %}{% cycle aye bee %}{{ i }},{% endfor %}" << dict << "a0,b1,a2,b3,a4," ;
}

void TestDefaultTags::testWidthRatioTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert("a", 50);
  dict.insert("b", 100);
  QTest::newRow("widthratio01") << "{% widthratio a b 0 %}" << dict << "0" << NoError;

  dict.clear();
  dict.insert("a", 0);
  dict.insert("b", 0);
  QTest::newRow("widthratio02") << "{% widthratio a b 0 %}" << dict << "" << NoError;

  dict.clear();
  dict.insert("a", 0);
  dict.insert("b", 100);
  QTest::newRow("widthratio03") << "{% widthratio a b 100 %}" << dict << "0" << NoError;

  dict.clear();
  dict.insert("a", 50);
  dict.insert("b", 100);
  QTest::newRow("widthratio04") << "{% widthratio a b 100 %}" << dict << "50" << NoError;

  dict.clear();
  dict.insert("a", 100);
  dict.insert("b", 100);
  QTest::newRow("widthratio05") << "{% widthratio a b 100 %}" << dict << "100" << NoError;

  dict.clear();
  dict.insert("a", 50);
  dict.insert("b", 80);
  QTest::newRow("widthratio06") << "{% widthratio a b 100 %}" << dict << "63" << NoError;

  dict.clear();
  dict.insert("a", 50);
  dict.insert("b", 70);
  QTest::newRow("widthratio07") << "{% widthratio a b 100 %}" << dict << "71" << NoError;

  dict.clear();
// Raise exception if we don't have 3 args, last one an integer
  QTest::newRow("widthratio08") << "{% widthratio %}" << dict << "" << TagSyntaxError;

  dict.clear();
  QTest::newRow("widthratio09") << "{% widthratio a b %}" << dict << "" << TagSyntaxError;

  dict.clear();
  QTest::newRow("widthratio10") << "{% widthratio a b 100.0 %}" << dict << "" << TagSyntaxError;
}


void TestDefaultTags::testFilterTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("filter01") << "{% filter upper %}{% endfilter %}" << dict << "" << NoError;
  QTest::newRow("filter02") << "{% filter upper %}django{% endfilter %}" << dict << "DJANGO" << NoError;
  QTest::newRow("filter03") << "{% filter upper|lower %}django{% endfilter %}" << dict << "django" << NoError;

  dict.insert("remove", "spam");
  QTest::newRow("filter04") << "{% filter cut:remove %}djangospam{% endfilter %}" << dict << "django" << NoError;

}

void TestDefaultTags::testNowTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QDate today = QDateTime::currentDateTime().date();

  QTest::newRow("now01") << "{% now \"d M yyyy\"%}" << dict << (QString::number(today.day()) + " " +  QString::number(today.month()) + " " + QString::number(today.year())) << NoError;


}

void TestDefaultTags::testSpacelessTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("spaceless01") << "{% spaceless %} <b>    <i> text </i>    </b> {% endspaceless %}" << dict << "<b><i> text </i></b>" << NoError;
  QTest::newRow("spaceless02") << "{% spaceless %} <b> \n <i> text </i> \n </b> {% endspaceless %}" << dict << "<b><i> text </i></b>" << NoError;
  QTest::newRow("spaceless03") << "{% spaceless %}<b><i>text</i></b>{% endspaceless %}" << dict << "<b><i>text</i></b>" << NoError;

}


QTEST_MAIN(TestDefaultTags)
#include "testdefaulttags.moc"

#endif
