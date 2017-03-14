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

#ifndef BUILTINSTEST_H
#define BUILTINSTEST_H

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtTest/QTest>

#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "filterexpression.h"
#include "grantlee_paths.h"
#include "template.h"
#include <metaenumvariable_p.h>

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

/**
  For use with tests.
*/
class OtherClass : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString method READ method)
  Q_PROPERTY(Animals animals READ animals)
public:
  OtherClass(QObject *parent = 0) : QObject(parent) {}

  enum Animals { Lions, Tigers, Bears };

  Q_ENUMS(Animals)
  Animals animals() const { return Tigers; }

  QString method() const { return QStringLiteral("OtherClass::method"); }
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
  enum FirstEnum { Employee, Employer, Manager };

  enum SecondEnum { Voter = 2, Consumer = 4, Citizen = 8 };

  Q_ENUMS(FirstEnum SecondEnum)

  SomeClass(QObject *parent = 0)
      : QObject(parent), m_other(new OtherClass(this))
  {
  }

  QString method() const { return QStringLiteral("SomeClass::method"); }

  QVariant otherClass() const { return QVariant::fromValue(m_other); }

  QString nonAccessibleMethod(const QString &str) { return str; }

private:
  QObject *m_other;
};

class NoEscapeOutputStream : public OutputStream
{
public:
  NoEscapeOutputStream() : OutputStream() {}

  NoEscapeOutputStream(QTextStream *stream) : OutputStream(stream) {}

  QSharedPointer<OutputStream> clone(QTextStream *stream) const override
  {
    return QSharedPointer<OutputStream>(new NoEscapeOutputStream(stream));
  }

  QString escape(const QString &input) const override { return input; }
};

class JSOutputStream : public OutputStream
{
public:
  JSOutputStream() : OutputStream() {}

  JSOutputStream(QTextStream *stream) : OutputStream(stream) {}

  QSharedPointer<OutputStream> clone(QTextStream *stream) const override
  {
    return QSharedPointer<OutputStream>(new JSOutputStream(stream));
  }

  QString escape(const QString &input) const override
  {
    QList<QPair<QString, QString>> jsEscapes;
    jsEscapes << QPair<QString, QString>(QChar::fromLatin1('\\'),
                                         QStringLiteral("\\u005C"))
              << QPair<QString, QString>(QChar::fromLatin1('\''),
                                         QStringLiteral("\\u0027"))
              << QPair<QString, QString>(QChar::fromLatin1('\"'),
                                         QStringLiteral("\\u0022"))
              << QPair<QString, QString>(QChar::fromLatin1('>'),
                                         QStringLiteral("\\u003E"))
              << QPair<QString, QString>(QChar::fromLatin1('<'),
                                         QStringLiteral("\\u003C"))
              << QPair<QString, QString>(QChar::fromLatin1('&'),
                                         QStringLiteral("\\u0026"))
              << QPair<QString, QString>(QChar::fromLatin1('='),
                                         QStringLiteral("\\u003D"))
              << QPair<QString, QString>(QChar::fromLatin1('-'),
                                         QStringLiteral("\\u002D"))
              << QPair<QString, QString>(QChar::fromLatin1(';'),
                                         QStringLiteral("\\u003B"))
              << QPair<QString, QString>(QChar(0x2028),
                                         QStringLiteral("\\u2028"))
              << QPair<QString, QString>(QChar(0x2029),
                                         QStringLiteral("\\u2029"));

    for (auto i = 0; i < 32; ++i) {
      jsEscapes << QPair<QString, QString>(
          QChar(i),
          QStringLiteral("\\u00")
              + QStringLiteral("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper());
    }

    QListIterator<QPair<QString, QString>> it(jsEscapes);
    auto retString = input;
    while (it.hasNext()) {
      auto escape = it.next();
      retString = retString.replace(escape.first, escape.second);
    }
    return retString;
  }
};

class TestBuiltinSyntax : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testObjects();

  void testRenderAfterError();

  void testBasicSyntax_data();
  void testBasicSyntax() { doTest(); }

  void testEnums_data();
  void testEnums() { doTest(); }

  void testListIndex_data();
  void testListIndex() { doTest(); }

  void testFilterSyntax_data();
  void testFilterSyntax() { doTest(); }

  void testCommentSyntax_data();
  void testCommentSyntax() { doTest(); }

  void testMultiline_data();
  void testMultiline() { doTest(); }

  void testEscaping_data();
  void testEscaping() { doTest(); }

  void testTypeAccessors_data();
  void testTypeAccessors() { doTest(); }
  void testTypeAccessorsUnordered_data();
  void testTypeAccessorsUnordered();

  void testMultipleStates();
  void testAlternativeEscaping();

  void testTemplatePathSafety_data();
  void testTemplatePathSafety();

  void testMediaPathSafety_data();
  void testMediaPathSafety();

  void testDynamicProperties_data();
  void testDynamicProperties() { doTest(); }

  void testGarbageInput_data();
  void testGarbageInput();

  void testInsignificantWhitespace_data();
  void testInsignificantWhitespace();

  void cleanupTestCase();

private:
  Engine *m_engine;

  QSharedPointer<InMemoryTemplateLoader> m_loader;

  Engine *getEngine();

  void doTest();
};

void TestBuiltinSyntax::testObjects()
{
  Context c1, c2;
  c1 = c1;
  c1 = c2;
  Context c3(c1);
  Q_UNUSED(c3);

  FilterExpression f1, f2;
  f1 = f1;
  f1 = f2;
  FilterExpression f3(f1);
  Q_UNUSED(f3);

  Variable v1;
  v1 = v1;
  v1 = f1.variable();
  Variable v3(v1);
  Q_UNUSED(v3);
  QVERIFY(!v1.isTrue(&c1));
  QVERIFY(!v1.isLocalized());

  c1.setMutating(true);
  QVERIFY(c1.isMutating());

  SafeString s1, s2;
  s1 = s1;
  s2 = s1;
  SafeString s3(s1);
  Q_UNUSED(s3);

  QMetaType::construct(qMetaTypeId<MetaEnumVariable>(), 0, 0);
}

void TestBuiltinSyntax::testRenderAfterError()
{

  Engine engine;
  engine.setPluginPaths(QStringList() << QStringLiteral(GRANTLEE_PLUGIN_PATH));

  QSharedPointer<InMemoryTemplateLoader> loader(new InMemoryTemplateLoader);
  loader->setTemplate(QStringLiteral("template1"),
                      QStringLiteral("This template has an error {{ va>r }}"));
  loader->setTemplate(QStringLiteral("template2"), QStringLiteral("Ok"));
  loader->setTemplate(QStringLiteral("main"),
                      QStringLiteral("{% include template_var %}"));

  engine.addTemplateLoader(loader);

  Context c;
  Template t;

  t = engine.loadByName(QStringLiteral("main"));

  c.insert(QStringLiteral("template_var"), QLatin1String("template1"));
  auto output = t->render(&c);
  QCOMPARE(output, QString());
  QCOMPARE(t->error(), TagSyntaxError);

  c.insert(QStringLiteral("template_var"), QLatin1String("template2"));
  QCOMPARE(t->render(&c), QLatin1String("Ok"));
  QCOMPARE(t->error(), NoError);
}

void TestBuiltinSyntax::initTestCase()
{
  m_engine = getEngine();
  m_loader
      = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());
  m_engine->addTemplateLoader(m_loader);
  QVERIFY(m_engine->templateLoaders().contains(m_loader));
}

Engine *TestBuiltinSyntax::getEngine()
{
  auto engine = new Engine(this);
  engine->setPluginPaths(QStringList() << QStringLiteral(GRANTLEE_PLUGIN_PATH));
  return engine;
}

void TestBuiltinSyntax::cleanupTestCase() { delete m_engine; }

void TestBuiltinSyntax::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

  if (t->error() != NoError) {
    if (t->error() != error)
      qDebug() << t->errorString();
    QCOMPARE(t->error(), error);
    return;
  }

  Context context(dict);

  auto result = t->render(&context);
  if (t->error() != NoError) {
    if (t->error() != error)
      qDebug() << t->errorString();
    QCOMPARE(t->error(), error);
    return;
  }

  QCOMPARE(t->error(), NoError);

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

  QTest::newRow("basic-syntax00") << QString() << dict << QString() << NoError;

  // Plain text should go through the template parser untouched
  QTest::newRow("basic-syntax01") << QStringLiteral("something cool") << dict
                                  << QStringLiteral("something cool")
                                  << NoError;

  // Variables should be replaced with their value in the current
  // context
  dict.insert(QStringLiteral("headline"), QStringLiteral("Success"));
  QTest::newRow("basic-syntax02") << QStringLiteral("{{ headline }}") << dict
                                  << QStringLiteral("Success") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("first"), 1);
  dict.insert(QStringLiteral("second"), 2);

  // More than one replacement variable is allowed in a template
  QTest::newRow("basic-syntax03")
      << QStringLiteral("{{ first }} --- {{ second }}") << dict
      << QStringLiteral("1 --- 2") << NoError;

  dict.clear();
  // Fail silently when a variable is not found in the current context
  QTest::newRow("basic-syntax04") << QStringLiteral("as{{ missing }}df") << dict
                                  << QStringLiteral("asdf") << NoError;

  // A variable may not contain more than one word
  QTest::newRow("basic-syntax06") << QStringLiteral("{{ multi word variable }}")
                                  << dict << QString() << TagSyntaxError;
  // Raise TemplateSyntaxError for empty variable tags
  QTest::newRow("basic-syntax07") << QStringLiteral("{{ }}") << dict
                                  << QString() << EmptyVariableError;
  QTest::newRow("basic-syntax08") << QStringLiteral("{{        }}") << dict
                                  << QString() << EmptyVariableError;

  // Attribute syntax allows a template to call an object's attribute

  auto someClass = new SomeClass(this);
  dict.insert(QStringLiteral("var"), QVariant::fromValue(someClass));

  QTest::newRow("basic-syntax09") << QStringLiteral("{{ var.method }}") << dict
                                  << QStringLiteral("SomeClass::method")
                                  << NoError;

  // Multiple levels of attribute access are allowed
  QTest::newRow("basic-syntax10")
      << QStringLiteral("{{ var.otherClass.method }}") << dict
      << QStringLiteral("OtherClass::method") << NoError;

  // Fail silently when a variable's attribute isn't found
  QTest::newRow("basic-syntax11") << QStringLiteral("{{ var.blech }}") << dict
                                  << QString() << NoError;

  // TODO: Needed?
  // Raise TemplateSyntaxError when trying to access a variable beginning with
  // an underscore
  // #C# {"var": SomeClass()}
  dict.clear();
  QVariantHash hash;
  hash.insert(QStringLiteral("__dict__"), QStringLiteral("foo"));
  dict.insert(QStringLiteral("var"), hash);
  QTest::newRow("basic-syntax12") << QStringLiteral("{{ var.__dict__ }}")
                                  << dict << QString() << TagSyntaxError;

  dict.clear();
  // Raise TemplateSyntaxError when trying to access a variable containing an
  // illegal character
  QTest::newRow("basic-syntax13") << QStringLiteral("{{ va>r }}") << dict
                                  << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax14") << QStringLiteral("{{ (var.r) }}") << dict
                                  << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax15") << QStringLiteral("{{ sp%am }}") << dict
                                  << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax16") << QStringLiteral("{{ eggs! }}") << dict
                                  << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax17") << QStringLiteral("{{ moo? }}") << dict
                                  << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax-error01") << QStringLiteral("{{ moo:arg }}")
                                        << dict << QString() << TagSyntaxError;
  QTest::newRow("basic-syntax-error02")
      << QStringLiteral("{{ moo|cut:'foo':'bar' }}") << dict << QString()
      << TagSyntaxError;

  // Attribute syntax allows a template to call a dictionary key's value

  hash.clear();
  hash.insert(QStringLiteral("bar"), QStringLiteral("baz"));
  dict.insert(QStringLiteral("foo"), hash);
  QTest::newRow("basic-syntax18") << QStringLiteral("{{ foo.bar }}") << dict
                                  << QStringLiteral("baz") << NoError;

  // Fail silently when a variable's dictionary key isn't found
  QTest::newRow("basic-syntax19") << QStringLiteral("{{ foo.spam }}") << dict
                                  << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("var"), QVariant::fromValue(someClass));
  // Fail silently when attempting to access an unavailable method
  QTest::newRow("basic-syntax20")
      << QStringLiteral("{{ var.nonAccessibleMethod }}") << dict << QString()
      << NoError;

  dict.clear();
  // Don't get confused when parsing something that is almost, but not
  // quite, a template tag.
  QTest::newRow("basic-syntax21") << QStringLiteral("a {{ moo %} b") << dict
                                  << QStringLiteral("a {{ moo %} b") << NoError;
  QTest::newRow("basic-syntax22") << QStringLiteral("{{ moo #}") << dict
                                  << QStringLiteral("{{ moo #}") << NoError;

  dict.insert(QStringLiteral("cow"), QStringLiteral("cow"));
  // Will try to treat "moo #} {{ cow" as the variable. Not ideal, but
  // costly to work around, so this triggers an error.
  QTest::newRow("basic-syntax23") << QStringLiteral("{{ moo #} {{ cow }}")
                                  << dict << QString() << TagSyntaxError;

  dict.clear();
  // Embedded newlines make it not-a-tag.
  QTest::newRow("basic-syntax24") << "{{ moo\n }}" << dict << "{{ moo\n }}"
                                  << NoError;
  // Literal strings are permitted inside variables, mostly for i18n
  // purposes.
  QTest::newRow("basic-syntax25") << "{{ \"fred\" }}" << dict
                                  << QStringLiteral("fred") << NoError;
  QTest::newRow("basic-syntax26") << "{{ \"\\\"fred\\\"\" }}" << dict
                                  << "\"fred\"" << NoError;
  QTest::newRow("basic-syntax27") << "{{ _(\"\\\"fred\\\"\") }}" << dict
                                  << "\"fred\"" << NoError;

  dict.clear();
  hash.clear();
  QVariantHash innerHash;
  innerHash.insert(QStringLiteral("3"), QStringLiteral("d"));
  hash.insert(QStringLiteral("2"), innerHash);
  dict.insert(QStringLiteral("1"), hash);

  QTest::newRow("basic-syntax28") << QStringLiteral("{{ 1.2.3 }}") << dict
                                  << QStringLiteral("d") << NoError;

  dict.clear();
  hash.clear();
  QVariantList list;
  list << QStringLiteral("a") << QStringLiteral("b") << QStringLiteral("c")
       << QStringLiteral("d");
  hash.insert(QStringLiteral("2"), list);
  dict.insert(QStringLiteral("1"), hash);
  QTest::newRow("basic-syntax29") << QStringLiteral("{{ 1.2.3 }}") << dict
                                  << QStringLiteral("d") << NoError;

  dict.clear();
  list.clear();
  QVariantList innerList;
  innerList << QStringLiteral("x") << QStringLiteral("x") << QStringLiteral("x")
            << QStringLiteral("x");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("y") << QStringLiteral("y") << QStringLiteral("y")
            << QStringLiteral("y");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("a") << QStringLiteral("b") << QStringLiteral("c")
            << QStringLiteral("d");
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("1"), list);

  QTest::newRow("basic-syntax30") << QStringLiteral("{{ 1.2.3 }}") << dict
                                  << QStringLiteral("d") << NoError;

  dict.clear();
  list.clear();
  innerList.clear();
  innerList << QStringLiteral("x") << QStringLiteral("x") << QStringLiteral("x")
            << QStringLiteral("x");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("y") << QStringLiteral("y") << QStringLiteral("y")
            << QStringLiteral("y");
  list.append(QVariant(innerList));
  innerList.clear();
  innerList << QStringLiteral("a") << QStringLiteral("b") << QStringLiteral("c")
            << QStringLiteral("d");
  list.append(QVariant(innerList));
  dict.insert(QStringLiteral("1"), list);

  QTest::newRow("basic-syntax31") << QStringLiteral("{{ 1.2.3 }}") << dict
                                  << QStringLiteral("d") << NoError;

  dict.clear();
  list.clear();
  hash.clear();
  hash.insert(QStringLiteral("x"), QStringLiteral("x"));
  list.append(hash);
  hash.clear();
  hash.insert(QStringLiteral("y"), QStringLiteral("y"));
  list.append(hash);
  hash.clear();
  hash.insert(QStringLiteral("3"), QStringLiteral("d"));
  list.append(hash);

  dict.insert(QStringLiteral("1"), list);

  QTest::newRow("basic-syntax32") << QStringLiteral("{{ 1.2.3 }}") << dict
                                  << QStringLiteral("d") << NoError;

  dict.clear();

  dict.insert(QStringLiteral("1"), QStringLiteral("abc"));
  QTest::newRow("basic-syntax33") << QStringLiteral("{{ 1 }}") << dict
                                  << QStringLiteral("1") << NoError;
  QTest::newRow("basic-syntax34") << QStringLiteral("{{ 1.2 }}") << dict
                                  << QStringLiteral("1.2") << NoError;

  dict.clear();

  dict.insert(QStringLiteral("abc"), QStringLiteral("def"));

  QTest::newRow("basic-syntax35")
      << QStringLiteral("{{ abc._something }} {{ abc._something|upper }}")
      << dict << QString() << TagSyntaxError;

  QTest::newRow("basic-syntax36") << "{{ \"fred }}" << dict << QString()
                                  << TagSyntaxError;
  QTest::newRow("basic-syntax37") << "{{ \'fred }}" << dict << QString()
                                  << TagSyntaxError;
  QTest::newRow("basic-syntax38") << "{{ \"fred\' }}" << dict << QString()
                                  << TagSyntaxError;
  QTest::newRow("basic-syntax39") << "{{ \'fred\" }}" << dict << QString()
                                  << TagSyntaxError;
  QTest::newRow("basic-syntax40") << "{{ _(\'fred }}" << dict << QString()
                                  << TagSyntaxError;
  QTest::newRow("basic-syntax41") << "{{ abc|removetags:_(\'fred }}" << dict
                                  << QString() << TagSyntaxError;
}

void TestBuiltinSyntax::testEnums_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  auto otherClass = new OtherClass(this);
  dict.insert(QStringLiteral("var"), QVariant::fromValue(otherClass));

  QTest::newRow("class-enums01") << QStringLiteral("{{ var.Lions }}") << dict
                                 << QStringLiteral("0") << NoError;
  QTest::newRow("class-enums02") << QStringLiteral("{{ var.Tigers }}") << dict
                                 << QStringLiteral("1") << NoError;
  QTest::newRow("class-enums03") << QStringLiteral("{{ var.Bears }}") << dict
                                 << QStringLiteral("2") << NoError;
  QTest::newRow("class-enums04") << QStringLiteral("{{ var.Hamsters }}") << dict
                                 << QString() << NoError;
  QTest::newRow("class-enums05") << QStringLiteral("{{ var.Tigers.name }}")
                                 << dict << QStringLiteral("Animals")
                                 << NoError;
  QTest::newRow("class-enums06") << QStringLiteral("{{ var.Tigers.scope }}")
                                 << dict << QStringLiteral("OtherClass")
                                 << NoError;
  QTest::newRow("class-enums07") << QStringLiteral("{{ var.Tigers.value }}")
                                 << dict << QStringLiteral("1") << NoError;
  QTest::newRow("class-enums08") << QStringLiteral("{{ var.Tigers.key }}")
                                 << dict << QStringLiteral("Tigers") << NoError;
  QTest::newRow("class-enums09") << QStringLiteral("{{ var.animals }}") << dict
                                 << QStringLiteral("1") << NoError;
  QTest::newRow("class-enums10") << QStringLiteral("{{ var.animals.name }}")
                                 << dict << QStringLiteral("Animals")
                                 << NoError;
  QTest::newRow("class-enums11") << QStringLiteral("{{ var.animals.scope }}")
                                 << dict << QStringLiteral("OtherClass")
                                 << NoError;
  QTest::newRow("class-enums12") << QStringLiteral("{{ var.animals.value }}")
                                 << dict << QStringLiteral("1") << NoError;
  QTest::newRow("class-enums13") << QStringLiteral("{{ var.animals.key }}")
                                 << dict << QStringLiteral("Tigers") << NoError;
  QTest::newRow("class-enums14") << QStringLiteral("{{ var.Animals.0 }}")
                                 << dict << QStringLiteral("0") << NoError;
  QTest::newRow("class-enums15") << QStringLiteral("{{ var.Animals.2 }}")
                                 << dict << QStringLiteral("2") << NoError;
  QTest::newRow("class-enums16") << QStringLiteral("{{ var.Animals.3 }}")
                                 << dict << QString() << NoError;
  QTest::newRow("class-enums17") << QStringLiteral("{{ var.Animals.0.name }}")
                                 << dict << QStringLiteral("Animals")
                                 << NoError;
  QTest::newRow("class-enums18") << QStringLiteral("{{ var.Animals.0.scope }}")
                                 << dict << QStringLiteral("OtherClass")
                                 << NoError;
  QTest::newRow("class-enums19") << QStringLiteral("{{ var.Animals.0.value }}")
                                 << dict << QStringLiteral("0") << NoError;
  QTest::newRow("class-enums20") << QStringLiteral("{{ var.Animals.0.key }}")
                                 << dict << QStringLiteral("Lions") << NoError;
  QTest::newRow("class-enums21") << QStringLiteral("{{ var.Animals.2.key }}")
                                 << dict << QStringLiteral("Bears") << NoError;
  QTest::newRow("class-enums22") << QStringLiteral("{{ var.Tigers.samba }}")
                                 << dict << QString() << NoError;
  QTest::newRow("class-enums23")
      << QStringLiteral("{% with var.animals as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}")
      << dict << QStringLiteral("Tigers,1,OtherClass") << NoError;
  QTest::newRow("class-enums24")
      << QStringLiteral("{% with var.Animals.2 as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}")
      << dict << QStringLiteral("Bears,2,OtherClass") << NoError;
  QTest::newRow("class-enums25")
      << QStringLiteral("{% with var.Bears as result %}{{ result.key }},{{ result }},{{ result.scope }}{% endwith %}")
      << dict << QStringLiteral("Bears,2,OtherClass") << NoError;
  QTest::newRow("class-enums26")
      << QStringLiteral("{% with var.Animals as result %}{{ result.0.key }},{{ result.1.key }},{{ result.2.key }}{% endwith %}")
      << dict << QStringLiteral("Lions,Tigers,Bears") << NoError;

  dict.clear();

  auto someClass = new SomeClass(this);
  dict.insert(QStringLiteral("var"), QVariant::fromValue(someClass));

  QTest::newRow("class-enums27") << QStringLiteral("{{ var.Employee }}") << dict
                                 << QStringLiteral("0") << NoError;
  QTest::newRow("class-enums28") << QStringLiteral("{{ var.Employer }}") << dict
                                 << QStringLiteral("1") << NoError;
  QTest::newRow("class-enums29") << QStringLiteral("{{ var.Manager }}") << dict
                                 << QStringLiteral("2") << NoError;
  QTest::newRow("class-enums30") << QStringLiteral("{{ var.Voter }}") << dict
                                 << QStringLiteral("2") << NoError;
  QTest::newRow("class-enums31") << QStringLiteral("{{ var.Consumer }}") << dict
                                 << QStringLiteral("4") << NoError;
  QTest::newRow("class-enums32") << QStringLiteral("{{ var.Citizen }}") << dict
                                 << QStringLiteral("8") << NoError;
  QTest::newRow("class-enums33") << QStringLiteral("{{ var.FirstEnum }}")
                                 << dict << QString() << NoError;
  QTest::newRow("class-enums34") << QStringLiteral("{{ var.SecondEnum }}")
                                 << dict << QString() << NoError;

  QTest::newRow("class-enums35")
      << QString::fromLatin1(
             "{% with var.SecondEnum as result %}"
             "{{ result.0 }},{{ result.1 }},{{ result.2 }},"
             "{{ result.0.key }},{{ result.1.key }},{{ result.2.key }},"
             "{{ result }},{{ result.scope }}"
             "{% endwith %}")
      << dict << QStringLiteral("2,4,8,Voter,Consumer,Citizen,,SomeClass")
      << NoError;

  QTest::newRow("class-enums36")
      << QStringLiteral("{% ifequal var.Employee 2 %}{% endifequal %}") << dict
      << QString() << NoError;

  dict.insert(QStringLiteral("var"), QVariant::fromValue(otherClass));

  QTest::newRow("enums-loops01")
      << QString::fromLatin1(
             "{% for enum in var.Animals %}{% ifequal enum var.Tigers %}"
             "<b>{{ enum.key }}</b>{% else %}{{ enum.key }}{% endifequal %},"
             "{% empty %}No content{% endfor %}")
      << dict << QStringLiteral("Lions,<b>Tigers</b>,Bears,") << NoError;

  QTest::newRow("enums-loops02")
      << QString::fromLatin1("{% for enum in var.Tigers %}"
                             "{% ifequal enum result %}<b>{{ enum.key }}</b>"
                             "{% else %}{{ enum.key }}{% endifequal %},"
                             "{% empty %}No content"
                             "{% endfor %}")
      << dict << QStringLiteral("No content") << NoError;

  QTest::newRow("enums-loops03")
      << QString::fromLatin1("{% with var.animals as result %}"
                             "{% for enum in var.Animals %}"
                             "{% ifequal enum result %}<b>{{ enum.key }}</b>"
                             "{% else %}{{ enum.key }}{% endifequal %},"
                             "{% empty %}No content"
                             "{% endfor %}"
                             "{% endwith %}")
      << dict << QStringLiteral("Lions,<b>Tigers</b>,Bears,") << NoError;

  QTest::newRow("enums-keycount01")
      << QStringLiteral("{{ var.Animals.keyCount }}") << dict
      << QStringLiteral("3") << NoError;
  QTest::newRow("enums-keycount02")
      << QStringLiteral("{{ var.Lions.keyCount }}") << dict
      << QStringLiteral("3") << NoError;
  QTest::newRow("enums-keycount02")
      << QStringLiteral("{{ var.animals.keyCount }}") << dict
      << QStringLiteral("3") << NoError;

  QTest::newRow("qt-enums01") << QStringLiteral("{{ Qt.AlignRight }}") << dict
                              << QStringLiteral("2") << NoError;
  QTest::newRow("qt-enums02") << QStringLiteral("{{ Qt.AlignRight.scope }}")
                              << dict << QStringLiteral("Qt") << NoError;
  QTest::newRow("qt-enums03") << QStringLiteral("{{ Qt.AlignRight.name }}")
                              << dict << QStringLiteral("Alignment") << NoError;
  QTest::newRow("qt-enums04") << QStringLiteral("{{ Qt.AlignRight.value }}")
                              << dict << QStringLiteral("2") << NoError;
  QTest::newRow("qt-enums05") << QStringLiteral("{{ Qt.AlignRight.key }}")
                              << dict << QStringLiteral("AlignRight")
                              << NoError;
  QTest::newRow("qt-enums06") << QStringLiteral("{{ Qt.Alignment.2.key }}")
                              << dict << QStringLiteral("AlignRight")
                              << NoError;
  QTest::newRow("qt-enums07") << QStringLiteral("{{ Qt.DoesNotExist }}") << dict
                              << QString() << NoError;
  QTest::newRow("qt-enums08") << QStringLiteral("{{ Qt }}") << dict
                              << QString() << NoError;
}

void TestBuiltinSyntax::testListIndex_data()
{

  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantList l;
  l << QStringLiteral("first item") << QStringLiteral("second item");

  dict.insert(QStringLiteral("var"), l);

  // List-index syntax allows a template to access a certain item of a
  // subscriptable object.
  QTest::newRow("list-index01") << QStringLiteral("{{ var.1 }}") << dict
                                << QStringLiteral("second item") << NoError;
  // Fail silently when the list index is out of range.
  QTest::newRow("list-index02") << QStringLiteral("{{ var.5 }}") << dict
                                << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("var"), QVariant());

  // Fail silently when the variable is not a subscriptable object.
  QTest::newRow("list-index03") << QStringLiteral("{{ var.1 }}") << dict
                                << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("var"), QVariantHash());
  // Fail silently when variable is a dict without the specified key.
  QTest::newRow("list-index04") << QStringLiteral("{{ var.1 }}") << dict
                                << QString() << NoError;

  dict.clear();

  QVariantHash hash;
  hash.insert(QStringLiteral("1"), QStringLiteral("hello"));
  dict.insert(QStringLiteral("var"), hash);
  // Dictionary lookup wins out when dict's key is a string.
  QTest::newRow("list-index05") << QStringLiteral("{{ var.1 }}") << dict
                                << QStringLiteral("hello") << NoError;

  // QVariantHash can only use strings as keys, so list-index06 and
  // list-index07
  // are not valid.

  dict.clear();

  QStringList sl;
  sl.append(QStringLiteral("hello"));
  sl.append(QStringLiteral("world"));
  dict.insert(QStringLiteral("var"), sl);
  // QStringList lookup
  QTest::newRow("list-index08") << QStringLiteral("{{ var.0 }}, {{ var.1 }}!")
                                << dict << QStringLiteral("hello, world!")
                                << NoError;
}

void TestBuiltinSyntax::testFilterSyntax_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  // Basic filter usage
  dict.insert(QStringLiteral("var"), QStringLiteral("Django is the greatest!"));
  QTest::newRow("filter-syntax01") << QStringLiteral("{{ var|upper }}") << dict
                                   << QStringLiteral("DJANGO IS THE GREATEST!")
                                   << NoError;

  // Chained filters
  QTest::newRow("filter-syntax02")
      << QStringLiteral("{{ var|upper|lower }}") << dict
      << QStringLiteral("django is the greatest!") << NoError;

  // Raise TemplateSyntaxError for space between a variable and filter pipe
  dict.clear();
  QTest::newRow("filter-syntax03") << QStringLiteral("{{ var |upper }}") << dict
                                   << QString() << TagSyntaxError;

  // Raise TemplateSyntaxError for space after a filter pipe
  QTest::newRow("filter-syntax04") << QStringLiteral("{{ var| upper }}") << dict
                                   << QString() << TagSyntaxError;

  // Raise TemplateSyntaxError for a nonexistent filter
  QTest::newRow("filter-syntax05") << QStringLiteral("{{ var|does_not_exist }}")
                                   << dict << QString() << UnknownFilterError;

  // Raise TemplateSyntaxError when trying to access a filter containing an
  // illegal character
  QTest::newRow("filter-syntax06") << QStringLiteral("{{ var|fil(ter) }}")
                                   << dict << QString() << UnknownFilterError;

  // Raise TemplateSyntaxError for invalid block tags
  QTest::newRow("filter-syntax07")
      << QStringLiteral("{% nothing_to_see_here %}") << dict << QString()
      << InvalidBlockTagError;
  // Raise TemplateSyntaxError for empty block tags
  QTest::newRow("filter-syntax08") << QStringLiteral("{% %}") << dict
                                   << QString() << EmptyBlockTagError;

  // Chained filters, with an argument to the first one
  dict.insert(QStringLiteral("var"), QStringLiteral("<b><i>Yes</i></b>"));
  QTest::newRow("filter-syntax09") << "{{ var|removetags:\"b i\"|upper|lower }}"
                                   << dict << QStringLiteral("yes") << NoError;
  // Literal string as argument is always "safe" from auto-escaping..
  dict.clear();
  dict.insert(QStringLiteral("var"), QVariant());
  QTest::newRow("filter-syntax10")
      << "{{ var|default_if_none:\" endquote\\\" hah\" }}" << dict
      << " endquote\" hah" << NoError;
  // Variable as argument
  dict.insert(QStringLiteral("var2"), QStringLiteral("happy"));
  QTest::newRow("filter-syntax11")
      << QStringLiteral("{{ var|default_if_none:var2 }}") << dict
      << QStringLiteral("happy") << NoError;
  // Default argument testing
  dict.clear();
  dict.insert(QStringLiteral("var"), true);
  QTest::newRow("filter-syntax12")
      << "{{ var|yesno:\"yup,nup,mup\" }} {{ var|yesno }}" << dict
      << QStringLiteral("yup yes") << NoError;

  // Fail silently for methods that raise an exception with a
  // "silent_variable_failure" attribute
  //   dict.clear();
  //   QObject *someClass = new SomeClass(this);
  //   dict.insert( QStringLiteral("var"), QVariant::fromValue(someClass));
  //   QTest::newRow("filter-syntax13") << QString::fromLatin1( "1{{
  //   var.method3
  //   }}2" ) << dict << QString::fromLatin1( "12" ) << NoError;
  //   // In methods that raise an exception without a
  //   // "silent_variable_attribute" set to True, the exception propagates
  //   // #C# SomeOtherException)
  //   QTest::newRow("filter-syntax14") << QString::fromLatin1( "var" ) <<
  //   dict
  //   << QString() << TagSyntaxError;

  // Escaped backslash in argument
  dict.clear();
  dict.insert(QStringLiteral("var"), QVariant());
  QTest::newRow("filter-syntax15") << "{{ var|default_if_none:\"foo\\bar\" }}"
                                   << dict << "foo\\bar" << NoError;
  // Escaped backslash using known escape char
  QTest::newRow("filter-syntax16") << "{{ var|default_if_none:\"foo\\now\" }}"
                                   << dict << "foo\\now" << NoError;
  // Empty strings can be passed as arguments to filters
  dict.clear();
  dict.insert(QStringLiteral("var"), QVariantList() << QStringLiteral("a")
                                                    << QStringLiteral("b")
                                                    << QStringLiteral("c"));
  QTest::newRow("filter-syntax17") << "{{ var|join:\"\" }}" << dict
                                   << QStringLiteral("abc") << NoError;

  // Make sure that any unicode strings are converted to bytestrings
  // in the final output.
  //   FAIL'filter-syntax18': (r'{{ var }}', {'var': UTF8Class()},
  //   u'\u0160\u0110\u0106\u017d\u0107\u017e\u0161\u0111'),

  // Numbers as filter arguments should work
  dict.clear();
  dict.insert(QStringLiteral("var"), QStringLiteral("hello world"));
  QTest::newRow("filter-syntax19")
      << QStringLiteral("{{ var|truncatewords:1 }}") << dict
      << QStringLiteral("hello ...") << NoError;
  // filters should accept empty string constants
  dict.clear();
  QTest::newRow("filter-syntax20") << "{{ \"\"|default_if_none:\"was none\" }}"
                                   << dict << QString() << NoError;

  QTest::newRow("filter-syntax21")
      << "{{ \"\"|default_if_none:|truncatewords }}" << dict << QString()
      << EmptyVariableError;
}

void TestBuiltinSyntax::testCommentSyntax_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("comment-syntax01")
      << QStringLiteral("{# this is hidden #}hello") << dict
      << QStringLiteral("hello") << NoError;
  QTest::newRow("comment-syntax02")
      << QStringLiteral("{# this is hidden #}hello{# foo #}") << dict
      << QStringLiteral("hello") << NoError;
  // Comments can contain invalid stuff.
  QTest::newRow("comment-syntax03") << QStringLiteral("foo{#  {% if %}  #}")
                                    << dict << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax04")
      << QStringLiteral("foo{#  {% endblock %}  #}") << dict
      << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax05")
      << QStringLiteral("foo{#  {% somerandomtag %}  #}") << dict
      << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax06") << QStringLiteral("foo{# {% #}") << dict
                                    << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax07") << QStringLiteral("foo{# %} #}") << dict
                                    << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax08") << QStringLiteral("foo{# %} #}bar") << dict
                                    << QStringLiteral("foobar") << NoError;
  QTest::newRow("comment-syntax09") << QStringLiteral("foo{# {{ #}") << dict
                                    << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax10") << QStringLiteral("foo{# }} #}") << dict
                                    << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax11") << QStringLiteral("foo{# { #}") << dict
                                    << QStringLiteral("foo") << NoError;
  QTest::newRow("comment-syntax12") << QStringLiteral("foo{# } #}") << dict
                                    << QStringLiteral("foo") << NoError;
}

void TestBuiltinSyntax::testMultiline_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("multiline01")
      << "Hello,\nboys.\nHow\nare\nyou\ngentlemen?" << dict
      << "Hello,\nboys.\nHow\nare\nyou\ngentlemen?" << NoError;
}

void TestBuiltinSyntax::testEscaping_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  // html escaping is not to be confused with for example url escaping.
  dict.insert(QStringLiteral("var"), QStringLiteral("< > & \" \' # = % $"));
  QTest::newRow("escape01") << QStringLiteral("{{ var }}") << dict
                            << "&lt; &gt; &amp; \" &#39; # = % $" << NoError;

  dict.clear();
  dict.insert(QStringLiteral("var"), QStringLiteral("this & that"));
  QTest::newRow("escape02") << QStringLiteral("{{ var }}") << dict
                            << QStringLiteral("this &amp; that") << NoError;

  // Strings are compared unescaped.
  QTest::newRow("escape03")
      << "{% ifequal var \"this & that\" %}yes{% endifequal %}" << dict
      << QStringLiteral("yes") << NoError;

  // Arguments to filters are 'safe' and manipulate their input unescaped.
  QTest::newRow("escape04") << "{{ var|cut:\"&\" }}" << dict
                            << QStringLiteral("this  that") << NoError;

  dict.insert(QStringLiteral("varList"),
              QVariantList() << QStringLiteral("Tom") << QStringLiteral("Dick")
                             << QStringLiteral("Harry"));
  QTest::newRow("escape05") << "{{ varList|join:\" & \" }}" << dict
                            << QStringLiteral("Tom & Dick & Harry") << NoError;

  // Unlike variable args.
  dict.insert(QStringLiteral("amp"), QStringLiteral(" & "));
  QTest::newRow("escape06") << QStringLiteral("{{ varList|join:amp }}") << dict
                            << QStringLiteral("Tom &amp; Dick &amp; Harry")
                            << NoError;

  // Literal strings are safe.
  QTest::newRow("escape07") << "{{ \"this & that\" }}" << dict
                            << QStringLiteral("this & that") << NoError;

  // Iterating outputs safe characters.
  dict.clear();
  QVariantList list;
  list << QStringLiteral("K") << QStringLiteral("&") << QStringLiteral("R");
  dict.insert(QStringLiteral("list"), list);
  QTest::newRow("escape08")
      << QStringLiteral("{% for letter in list %}{{ letter }},{% endfor %}")
      << dict << QStringLiteral("K,&amp;,R,") << NoError;

  dict.clear();
  // escape requirement survives lookup.
  QVariantHash hash;
  hash.insert(QStringLiteral("key"), QStringLiteral("this & that"));
  dict.insert(QStringLiteral("var"), hash);
  QTest::newRow("escape09") << QStringLiteral("{{ var.key }}") << dict
                            << QStringLiteral("this &amp; that") << NoError;

  dict.clear();
}

void TestBuiltinSyntax::testMultipleStates()
{
  auto engine1 = getEngine();

  auto loader1
      = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());

  loader1->setTemplate(QStringLiteral("template1"),
                       QStringLiteral("Template 1"));
  engine1->addTemplateLoader(loader1);

  auto t1 = engine1->newTemplate(QStringLiteral("{% include \"template1\" %}"),
                                 QStringLiteral("\"template1\""));

  auto engine2 = getEngine();

  auto loader2
      = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());

  loader2->setTemplate(QStringLiteral("template2"),
                       QStringLiteral("Template 2"));

  engine2->addTemplateLoader(loader2);

  auto t2 = engine2->newTemplate(QStringLiteral("{% include \"template2\" %}"),
                                 QStringLiteral("\"template2\""));

  auto engine3 = getEngine();

  auto loader3
      = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());

  loader3->setTemplate(QStringLiteral("template3"),
                       QStringLiteral("Template 3"));

  engine3->addTemplateLoader(loader3);

  auto t3 = engine3->newTemplate(QStringLiteral("{% include var %}"),
                                 QStringLiteral("var"));

  QVariantHash h;
  h.insert(QStringLiteral("var"), QStringLiteral("template3"));
  Context c(h);
  t1->render(&c);

  auto expected1 = QStringLiteral("Template 1");
  auto expected2 = QStringLiteral("Template 2");
  auto expected3 = QStringLiteral("Template 3");
  QCOMPARE(t1->render(&c), expected1);
  QCOMPARE(t2->render(&c), expected2);
  QCOMPARE(t3->render(&c), expected3);
}

void TestBuiltinSyntax::testAlternativeEscaping()
{
  auto engine1 = getEngine();

  auto t1 = engine1->newTemplate(
      QStringLiteral("{{ var }} {% spaceless %}{{ var }}{% endspaceless %}"),
      QStringLiteral("\"template1\""));

  auto input = QStringLiteral("< > \r\n & \" \' # = % $");

  QVariantHash h;
  h.insert(QStringLiteral("var"), input);
  Context c(h);

  QString output;
  QTextStream ts(&output);

  NoEscapeOutputStream noEscapeOs(&ts);

  t1->render(&noEscapeOs, &c);

  QCOMPARE(output, QString(input + QLatin1String(" ") + input));
  output.clear();

  JSOutputStream jsOs(&ts);

  t1->render(&jsOs, &c);

  QString jsOutput(QStringLiteral(
      "\\u003C \\u003E \\u000D\\u000A \\u0026 \\u0022 \\u0027 # \\u003D % $"));

  jsOutput = jsOutput + QLatin1String(" ") + jsOutput;

  QCOMPARE(output, jsOutput);
}

void TestBuiltinSyntax::testTemplatePathSafety_data()
{
  QTest::addColumn<QString>("inputPath");
  QTest::addColumn<QString>("output");

  QTest::newRow("template-path-safety01") << QStringLiteral("visible_file")
                                          << QStringLiteral("visible_file");
  QTest::newRow("template-path-safety02") << QStringLiteral("../invisible_file")
                                          << QString();
}

void TestBuiltinSyntax::testTemplatePathSafety()
{
  QFETCH(QString, inputPath);
  QFETCH(QString, output);

  auto loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs(QStringList() << QStringLiteral("."));

  QFile f(inputPath);
  auto opened = f.open(QFile::WriteOnly | QFile::Text);
  QVERIFY(opened);
  f.write(inputPath.toUtf8());
  f.close();

  auto t = loader->loadByName(inputPath, m_engine);
  Context c;
  if (output.isEmpty())
    QVERIFY(!t);
  else
    QCOMPARE(t->render(&c), inputPath);

  delete loader;
  f.remove();
}

void TestBuiltinSyntax::testMediaPathSafety_data()
{
  QTest::addColumn<QString>("inputPath");
  QTest::addColumn<QString>("output");

  QTest::newRow("media-path-safety01") << QStringLiteral("visible_file")
                                       << QStringLiteral("./visible_file");
  QTest::newRow("media-path-safety02") << QStringLiteral("../invisible_file")
                                       << QString();
}

void TestBuiltinSyntax::testMediaPathSafety()
{
  QFETCH(QString, inputPath);
  QFETCH(QString, output);

  auto loader = new FileSystemTemplateLoader();

  loader->setTemplateDirs(QStringList() << QStringLiteral("."));

  QFile f(inputPath);
  auto opened = f.open(QFile::WriteOnly | QFile::Text);
  QVERIFY(opened);
  f.write(inputPath.toUtf8());
  f.close();

  auto uri = loader->getMediaUri(inputPath);
  if (output.isEmpty())
    QVERIFY(uri.second.isEmpty());
  else
    QCOMPARE(QFileInfo(uri.first + uri.second).absoluteFilePath(),
             QFileInfo(output).absoluteFilePath());

  delete loader;
  f.remove();
}

void TestBuiltinSyntax::testTypeAccessorsUnordered()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QStringList, output);
  QFETCH(Grantlee::Error, error);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

  Context context(dict);

  auto result = t->render(&context);
  if (t->error() != NoError) {
    if (t->error() != error)
      qDebug() << t->errorString();
    QCOMPARE(t->error(), error);
    return;
  }

  QCOMPARE(t->error(), NoError);

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  Q_FOREACH (const QString &s, output) {
    QVERIFY(result.contains(s));
  }

  QCOMPARE(result.length(), output.join(QString()).length());
}

void TestBuiltinSyntax::testTypeAccessorsUnordered_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QStringList>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantHash itemsHash;
  itemsHash.insert(QStringLiteral("one"), 1);
  itemsHash.insert(QStringLiteral("two"), 2);
  itemsHash.insert(QStringLiteral("three"), 3);

  dict.insert(QStringLiteral("hash"), itemsHash);

  QTest::newRow("type-accessors-hash-unordered01")
      << QStringLiteral("{% for key,value in hash.items %}{{ key }}:{{ value }};{% endfor %}")
      << dict
      << (QStringList() << QStringLiteral("one:1;") << QStringLiteral("two:2;")
                        << QStringLiteral("three:3;"))
      << NoError;
  QTest::newRow("type-accessors-hash-unordered02")
      << QStringLiteral("{% for key in hash.keys %}{{ key }};{% endfor %}")
      << dict
      << (QStringList() << QStringLiteral("one;") << QStringLiteral("two;")
                        << QStringLiteral("three;"))
      << NoError;
  QTest::newRow("type-accessors-hash-unordered03")
      << QStringLiteral(
             "{% for value in hash.values %}{{ value }};{% endfor %}")
      << dict << (QStringList() << QStringLiteral("1;") << QStringLiteral("2;")
                                << QStringLiteral("3;"))
      << NoError;
}

void TestBuiltinSyntax::testTypeAccessors_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QVariantHash itemsHash;
  itemsHash.insert(QStringLiteral("one"), 1);
  itemsHash.insert(QStringLiteral("two"), 2);
  itemsHash.insert(QStringLiteral("three"), 3);

  dict.insert(QStringLiteral("hash"), itemsHash);

  QTest::newRow("type-accessors-hash01")
      << QStringLiteral("{{ hash.items|length }}") << dict
      << QStringLiteral("3") << NoError;
  QTest::newRow("type-accessors-hash02")
      << QStringLiteral("{{ hash.keys|length }}") << dict << QStringLiteral("3")
      << NoError;
  QTest::newRow("type-accessors-hash03")
      << QStringLiteral("{{ hash.values|length }}") << dict
      << QStringLiteral("3") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("my string"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("mystring"));

  QTest::newRow("type-accessors-string01")
      << QStringLiteral("{{ str1.capitalize }}") << dict
      << QStringLiteral("My string") << NoError;
  QTest::newRow("type-accessors-string02")
      << QStringLiteral("{{ str2.capitalize }}") << dict
      << QStringLiteral("Mystring") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("de24335fre"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("de435f3.-5r"));

  QTest::newRow("type-accessors-string03")
      << QStringLiteral("{{ str1.isalnum }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string04")
      << QStringLiteral("{{ str2.isalnum }}") << dict << QStringLiteral("False")
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("24335"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("de435f35r"));
  dict.insert(QStringLiteral("str3"), QStringLiteral("de435f3.-5r"));

  QTest::newRow("type-accessors-string05")
      << QStringLiteral("{{ str1.isdigit }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string06")
      << QStringLiteral("{{ str2.isdigit }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string07")
      << QStringLiteral("{{ str3.isdigit }}") << dict << QStringLiteral("False")
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str"), QStringLiteral("MyString"));
  dict.insert(QStringLiteral("lowerStr"), QStringLiteral("mystring"));

  QTest::newRow("type-accessors-string08")
      << QStringLiteral("{{ str.islower }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string09")
      << QStringLiteral("{{ lowerStr.islower }}") << dict
      << QStringLiteral("True") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("        "));
  dict.insert(QStringLiteral("str2"), QStringLiteral("     r  "));
  dict.insert(QStringLiteral("str3"), QStringLiteral(" \t\nr  "));
  dict.insert(QStringLiteral("str4"), QStringLiteral(" \t\n   "));

  QTest::newRow("type-accessors-string10")
      << QStringLiteral("{{ str1.isspace }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string11")
      << QStringLiteral("{{ str2.isspace }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string12")
      << QStringLiteral("{{ str3.isspace }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string13")
      << QStringLiteral("{{ str4.isspace }}") << dict << QStringLiteral("True")
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("My String"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("Mystring"));
  dict.insert(QStringLiteral("str3"), QStringLiteral("My string"));
  dict.insert(QStringLiteral("str4"), QStringLiteral("my string"));

  QTest::newRow("type-accessors-string14")
      << QStringLiteral("{{ str1.istitle }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string15")
      << QStringLiteral("{{ str2.istitle }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string16")
      << QStringLiteral("{{ str3.istitle }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string17")
      << QStringLiteral("{{ str4.istitle }}") << dict << QStringLiteral("False")
      << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str"), QStringLiteral("MyString"));
  dict.insert(QStringLiteral("upperStr"), QStringLiteral("MYSTRING"));

  QTest::newRow("type-accessors-string18")
      << QStringLiteral("{{ str.isupper }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string19")
      << QStringLiteral("{{ upperStr.isupper }}") << dict
      << QStringLiteral("True") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("My String"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("MYSTRING"));
  dict.insert(QStringLiteral("str3"), QStringLiteral("MY STRING"));

  QTest::newRow("type-accessors-string20")
      << QStringLiteral("{{ str1.lower }}") << dict
      << QStringLiteral("my string") << NoError;
  QTest::newRow("type-accessors-string21") << QStringLiteral("{{ str2.lower }}")
                                           << dict << QStringLiteral("mystring")
                                           << NoError;
  QTest::newRow("type-accessors-string22")
      << QStringLiteral("{{ str3.lower }}") << dict
      << QStringLiteral("my string") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str"), QStringLiteral("one\ntwo three\nfour"));

  QTest::newRow("type-accessors-string23")
      << QStringLiteral(
             "{% for line in str.splitlines %}{{ line }};{% endfor %}")
      << dict << QStringLiteral("one;two three;four;") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("          one"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("     one     "));
  dict.insert(QStringLiteral("str3"), QStringLiteral("one          "));
  dict.insert(QStringLiteral("str4"), QStringLiteral("             "));
  dict.insert(QStringLiteral("str5"), QStringLiteral(""));

  QTest::newRow("type-accessors-string24") << QStringLiteral("{{ str1.strip }}")
                                           << dict << QStringLiteral("one")
                                           << NoError;
  QTest::newRow("type-accessors-string25") << QStringLiteral("{{ str2.strip }}")
                                           << dict << QStringLiteral("one")
                                           << NoError;
  QTest::newRow("type-accessors-string26") << QStringLiteral("{{ str3.strip }}")
                                           << dict << QStringLiteral("one")
                                           << NoError;
  QTest::newRow("type-accessors-string27") << QStringLiteral("{{ str4.strip }}")
                                           << dict << QString() << NoError;
  QTest::newRow("type-accessors-string28") << QStringLiteral("{{ str5.strip }}")
                                           << dict << QString() << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("My String"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("mY sTRING"));
  dict.insert(QStringLiteral("str3"), QStringLiteral("My StrInG"));
  dict.insert(QStringLiteral("str4"), QStringLiteral("my string"));
  dict.insert(QStringLiteral("str5"), QStringLiteral("MY STRING"));

  // Yes, this really is a python built-in.
  QTest::newRow("type-accessors-string29")
      << QStringLiteral("{{ str1.swapcase }}") << dict
      << QStringLiteral("mY sTRING") << NoError;
  QTest::newRow("type-accessors-string30")
      << QStringLiteral("{{ str2.swapcase }}") << dict
      << QStringLiteral("My String") << NoError;
  QTest::newRow("type-accessors-string31")
      << QStringLiteral("{{ str3.swapcase }}") << dict
      << QStringLiteral("mY sTRiNg") << NoError;
  QTest::newRow("type-accessors-string32")
      << QStringLiteral("{{ str4.swapcase }}") << dict
      << QStringLiteral("MY STRING") << NoError;
  QTest::newRow("type-accessors-string33")
      << QStringLiteral("{{ str5.swapcase }}") << dict
      << QStringLiteral("my string") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("str1"), QStringLiteral("My String"));
  dict.insert(QStringLiteral("str2"), QStringLiteral("mystring"));
  dict.insert(QStringLiteral("str3"), QStringLiteral("my string"));
  dict.insert(QStringLiteral("str4"), QStringLiteral("my String"));
  dict.insert(QStringLiteral("str5"), QStringLiteral("My string"));
  dict.insert(QStringLiteral("str6"), QStringLiteral("123"));
  dict.insert(QStringLiteral("str7"), QString());

  QTest::newRow("type-accessors-string34")
      << QStringLiteral("{{ str1.title }}") << dict
      << QStringLiteral("My String") << NoError;
  QTest::newRow("type-accessors-string35") << QStringLiteral("{{ str2.title }}")
                                           << dict << QStringLiteral("Mystring")
                                           << NoError;
  QTest::newRow("type-accessors-string36")
      << QStringLiteral("{{ str3.title }}") << dict
      << QStringLiteral("My String") << NoError;
  QTest::newRow("type-accessors-string37")
      << QStringLiteral("{{ str4.title }}") << dict
      << QStringLiteral("My String") << NoError;
  QTest::newRow("type-accessors-string38")
      << QStringLiteral("{{ str5.title }}") << dict
      << QStringLiteral("My String") << NoError;

  QTest::newRow("type-accessors-string39")
      << QStringLiteral("{{ str1.upper }}") << dict
      << QStringLiteral("MY STRING") << NoError;
  QTest::newRow("type-accessors-string40") << QStringLiteral("{{ str2.upper }}")
                                           << dict << QStringLiteral("MYSTRING")
                                           << NoError;
  QTest::newRow("type-accessors-string41")
      << QStringLiteral("{{ str3.upper }}") << dict
      << QStringLiteral("MY STRING") << NoError;
  QTest::newRow("type-accessors-string42") << QStringLiteral("{{ str3.dne }}")
                                           << dict << QString() << NoError;
  QTest::newRow("type-accessors-string43")
      << QStringLiteral("{{ str2.isalpha }}") << dict << QStringLiteral("True")
      << NoError;
  QTest::newRow("type-accessors-string44")
      << QStringLiteral("{{ str3.isalpha }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string45")
      << QStringLiteral("{{ str6.isalpha }}") << dict << QStringLiteral("False")
      << NoError;
  QTest::newRow("type-accessors-string46")
      << QStringLiteral("{{ str7.isalpha }}") << dict << QStringLiteral("False")
      << NoError;

  dict.clear();

#define SON(obj) obj->setObjectName(QStringLiteral(#obj))

  auto obj1 = new QObject(this);
  SON(obj1);
  auto obj2 = new QObject(this);
  SON(obj2);
  obj2->setParent(obj1);
  auto obj3 = new QObject(this);
  obj3->setParent(obj2);
  SON(obj3);
  auto obj4 = new QObject(this);
  obj4->setParent(obj2);
  SON(obj4);

  dict.insert(QStringLiteral("object"), QVariant::fromValue(obj1));

  QTest::newRow("type-accessors-qobject01")
      << QStringLiteral("{{ object.objectName }}") << dict
      << QStringLiteral("obj1") << NoError;

  const QLatin1String objectDumper("<li>{{ object.objectName }}</li>"
                                   "{% if object.children %}"
                                   "<ul>"
                                   "{% for object in object.children %}"
                                   "{% include 'objectdumper.html' %}"
                                   "{% endfor %}"
                                   "</ul>"
                                   "{% endif %}");

  m_loader->setTemplate(QStringLiteral("objectdumper.html"), objectDumper);

  QTest::newRow("type-accessors-qobject02")
      << QStringLiteral("<ul>{% include 'objectdumper.html' %}</ul>") << dict
      << QString::fromLatin1("<ul>"
                             "<li>obj1</li>"
                             "<ul>"
                             "<li>obj2</li>"
                             "<ul>"
                             "<li>obj3</li>"
                             "<li>obj4</li>"
                             "</ul>"
                             "</ul>"
                             "</ul>")
      << NoError;
}

void TestBuiltinSyntax::testDynamicProperties_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  auto obj = new QObject(this);
  obj->setProperty("prop", 7);
  dict.insert(QStringLiteral("var"),
              QVariant::fromValue(static_cast<QObject *>(obj)));

  QTest::newRow("dynamic-properties01") << QStringLiteral("{{ var.prop }}")
                                        << dict << QStringLiteral("7")
                                        << NoError;
}

void TestBuiltinSyntax::testGarbageInput()
{
  QFETCH(QString, input);

  auto t = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

  Dict dict;

  Context context(dict);

  auto result = t->render(&context);

  QCOMPARE(t->error(), NoError);

  QCOMPARE(result, input);
}

void TestBuiltinSyntax::testGarbageInput_data()
{

  QTest::addColumn<QString>("input");

  Dict dict;

  QTest::newRow("garbage-input01") << QStringLiteral("content %}");
  QTest::newRow("garbage-input02") << QStringLiteral(" content %}");
  QTest::newRow("garbage-input03") << QStringLiteral("content #}");
  QTest::newRow("garbage-input04") << QStringLiteral(" content #}");
  QTest::newRow("garbage-input05") << QStringLiteral("content }}");
  QTest::newRow("garbage-input06") << QStringLiteral(" content }}");
  QTest::newRow("garbage-input07") << QStringLiteral("% content %}");
  QTest::newRow("garbage-input08") << QStringLiteral("# content #}");
  QTest::newRow("garbage-input09") << QStringLiteral("{ content }}");
  QTest::newRow("garbage-input10") << QStringLiteral("{% content }");
  QTest::newRow("garbage-input11") << QStringLiteral("{% content %");
  QTest::newRow("garbage-input12") << QStringLiteral("{# content }");
  QTest::newRow("garbage-input13") << QStringLiteral("{# content #");
  QTest::newRow("garbage-input14") << QStringLiteral("{{ content }");
  QTest::newRow("garbage-input15") << QStringLiteral("{{ content }");
  QTest::newRow("garbage-input16") << QStringLiteral("{{ content %}");
  QTest::newRow("garbage-input17") << QStringLiteral("{% content }}");
  QTest::newRow("garbage-input18") << QStringLiteral("{{ content #}");
  QTest::newRow("garbage-input19") << QStringLiteral("{# content }}");
  QTest::newRow("garbage-input20") << QStringLiteral("{{ con #} tent #}");
  QTest::newRow("garbage-input21") << QStringLiteral("{{ con %} tent #}");
  QTest::newRow("garbage-input22") << QStringLiteral("{{ con #} tent %}");
  QTest::newRow("garbage-input23") << QStringLiteral("{{ con %} tent %}");
  QTest::newRow("garbage-input24") << QStringLiteral("{% con #} tent #}");
  QTest::newRow("garbage-input25") << QStringLiteral("{% con }} tent #}");
  QTest::newRow("garbage-input26") << QStringLiteral("{% con #} tent }}");
  QTest::newRow("garbage-input27") << QStringLiteral("{% con }} tent }}");
  QTest::newRow("garbage-input28") << QStringLiteral("{# con %} tent %}");
  QTest::newRow("garbage-input29") << QStringLiteral("{# con }} tent %}");
  QTest::newRow("garbage-input30") << QStringLiteral("{# con %} tent }}");
  QTest::newRow("garbage-input31") << QStringLiteral("{# con }} tent }}");
  QTest::newRow("garbage-input32") << QStringLiteral("{# con {# tent }}");
  QTest::newRow("garbage-input33") << QStringLiteral("{# con {% tent }}");
  QTest::newRow("garbage-input34") << QStringLiteral("{% con {% tent }}");
  QTest::newRow("garbage-input35") << QStringLiteral("{ { content }}");
  QTest::newRow("garbage-input36") << QStringLiteral("{ % content %}");
  QTest::newRow("garbage-input37") << QStringLiteral("{ # content #}");
  QTest::newRow("garbage-input38") << QStringLiteral("{\n{ content }}");
  QTest::newRow("garbage-input39") << QStringLiteral("{\n# content #}");
  QTest::newRow("garbage-input40") << QStringLiteral("{\n% content %}");
  QTest::newRow("garbage-input41") << QStringLiteral("{{\n content }}");
  QTest::newRow("garbage-input42") << QStringLiteral("{#\n content #}");
  QTest::newRow("garbage-input43") << QStringLiteral("{%\n content %}");
  QTest::newRow("garbage-input44") << QStringLiteral("{{ content \n}}");
  QTest::newRow("garbage-input45") << QStringLiteral("{# content \n#}");
  QTest::newRow("garbage-input46") << QStringLiteral("{% content \n%}");
  QTest::newRow("garbage-input47") << QStringLiteral("{{ content }\n}");
  QTest::newRow("garbage-input48") << QStringLiteral("{# content #\n}");
  QTest::newRow("garbage-input49") << QStringLiteral("{% content %\n}");
  QTest::newRow("garbage-input50") << QStringLiteral("{{ content } }");
  QTest::newRow("garbage-input51") << QStringLiteral("{% content % }");
  QTest::newRow("garbage-input52") << QStringLiteral("{# content # }");
  QTest::newRow("garbage-input53") << QStringLiteral("{ { content } }");
  QTest::newRow("garbage-input54") << QStringLiteral("{ % content % }");
  QTest::newRow("garbage-input55") << QStringLiteral("{ # content # }");
  QTest::newRow("garbage-input56") << QStringLiteral("{{ content }%");
  QTest::newRow("garbage-input57") << QStringLiteral("{# content #%");
  QTest::newRow("garbage-input58") << QStringLiteral("{% content %%");
  QTest::newRow("garbage-input59") << QStringLiteral("{{ content }A");
  QTest::newRow("garbage-input60") << QStringLiteral("{# content #A");
  QTest::newRow("garbage-input61") << QStringLiteral("{% content %A");
  QTest::newRow("garbage-input62") << QStringLiteral("{{ content A}");
  QTest::newRow("garbage-input63") << QStringLiteral("{# content A#");
  QTest::newRow("garbage-input64") << QStringLiteral("{% content A%");
  QTest::newRow("garbage-input65") << QStringLiteral("{# content A}");
  QTest::newRow("garbage-input66") << QStringLiteral("{% content A}");
  QTest::newRow("garbage-input67") << QStringLiteral("A{ content }}");
  QTest::newRow("garbage-input68") << QStringLiteral("A# content #}");
  QTest::newRow("garbage-input69") << QStringLiteral("A% content %}");
  QTest::newRow("garbage-input60") << QStringLiteral("{A content }}");
  QTest::newRow("garbage-input71") << QStringLiteral("{A content #}");
  QTest::newRow("garbage-input72") << QStringLiteral("{A content %}");
  QTest::newRow("garbage-input73") << QStringLiteral("{A content #}");
  QTest::newRow("garbage-input74") << QStringLiteral("{A content %}");
  QTest::newRow("garbage-input75") << QStringLiteral("{A content A}");
  QTest::newRow("garbage-input76") << QStringLiteral("}} content }}");
  QTest::newRow("garbage-input77") << QStringLiteral("}} content {{");
  QTest::newRow("garbage-input78") << QStringLiteral("#} content #}");
  QTest::newRow("garbage-input79") << QStringLiteral("#} content {#");
  QTest::newRow("garbage-input80") << QStringLiteral("%} content %}");
  QTest::newRow("garbage-input81") << QStringLiteral("%} content {%");
  QTest::newRow("garbage-input82") << QStringLiteral("#{ content }#");
  QTest::newRow("garbage-input83") << QStringLiteral("%{ content }%");
}

void TestBuiltinSyntax::testInsignificantWhitespace()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, stripped_output);
  QFETCH(QString, unstripped_output);

  Context context(dict);

  QVERIFY(!m_engine->smartTrimEnabled());
  m_engine->setSmartTrimEnabled(true);
  QVERIFY(m_engine->smartTrimEnabled());

  {
    auto t
        = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

    auto result = t->render(&context);

    QCOMPARE(t->error(), NoError);

    QCOMPARE(result, stripped_output);
  }
  m_engine->setSmartTrimEnabled(false);
  {
    auto t
        = m_engine->newTemplate(input, QLatin1String(QTest::currentDataTag()));

    auto result = t->render(&context);

    QCOMPARE(t->error(), NoError);

    QCOMPARE(result, unstripped_output);
  }
}

void TestBuiltinSyntax::testInsignificantWhitespace_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("stripped_output");
  QTest::addColumn<QString>("unstripped_output");

  Dict dict;

  QTest::newRow("insignificant-whitespace01")
      << QStringLiteral("\n {% templatetag openblock %}\n") << dict
      << QStringLiteral("{%\n") << QStringLiteral("\n {%\n");

  QTest::newRow("insignificant-whitespace02")
      << QStringLiteral("\n{% templatetag openblock %}\n") << dict
      << QStringLiteral("{%\n") << QStringLiteral("\n{%\n");

  QTest::newRow("insignificant-whitespace03")
      << QStringLiteral("{% templatetag openblock %}\n") << dict
      << QStringLiteral("{%\n") << QStringLiteral("{%\n");

  QTest::newRow("insignificant-whitespace04")
      << QStringLiteral("\n\t \t {% templatetag openblock %}\n") << dict
      << QStringLiteral("{%\n") << QStringLiteral("\n\t \t {%\n");

  // Leading whitespace with text before single template tag
  QTest::newRow("insignificant-whitespace05")
      << QStringLiteral("\n some\ttext {% templatetag openblock %}\n") << dict
      << QStringLiteral("\n some\ttext {%\n")
      << QStringLiteral("\n some\ttext {%\n");

  // Leading line with text before single template tag
  QTest::newRow("insignificant-whitespace06")
      << QStringLiteral("\n some\ttext\n {% templatetag openblock %}\n") << dict
      << QStringLiteral("\n some\ttext{%\n")
      << QStringLiteral("\n some\ttext\n {%\n");
  QTest::newRow("insignificant-whitespace07")
      << QStringLiteral("\n some\ttext \n \t {% templatetag openblock %}\n")
      << dict << QStringLiteral("\n some\ttext {%\n")
      << QStringLiteral("\n some\ttext \n \t {%\n");

  // whitespace leading /before/ the newline is not stripped.
  QTest::newRow("insignificant-whitespace08")
      << QStringLiteral("\n some\ttext \t \n {% templatetag openblock %}\n")
      << dict << QStringLiteral("\n some\ttext \t {%\n")
      << QStringLiteral("\n some\ttext \t \n {%\n");

  // Multiple text lines before tag
  QTest::newRow("insignificant-whitespace09")
      << QStringLiteral("\n some\ntext \t \n {% templatetag openblock %}\n")
      << dict << QStringLiteral("\n some\ntext \t {%\n")
      << QStringLiteral("\n some\ntext \t \n {%\n");
  QTest::newRow("insignificant-whitespace10")
      << QStringLiteral(
             "\n some \t \n \t text \t \n {% templatetag openblock %}\n")
      << dict << QStringLiteral("\n some \t \n \t text \t {%\n")
      << QStringLiteral("\n some \t \n \t text \t \n {%\n");

  // Leading whitespace before tag, some text after
  QTest::newRow("insignificant-whitespace11")
      << QStringLiteral("\n   \t {% templatetag openblock %} some text\n")
      << dict << QStringLiteral("\n   \t {% some text\n")
      << QStringLiteral("\n   \t {% some text\n");

  // Leading whitespace before tag, some text with trailing whitespace after
  QTest::newRow("insignificant-whitespace12")
      << QStringLiteral("\n   \t {% templatetag openblock %} some text  \t \n")
      << dict << QStringLiteral("\n   \t {% some text  \t \n")
      << QStringLiteral("\n   \t {% some text  \t \n");

  // Whitespace after tag is not removed
  QTest::newRow("insignificant-whitespace13")
      << QStringLiteral(
             "\n \t {% templatetag openblock %} \t \n \t some text  \t \n")
      << dict << QStringLiteral("{% \t \n \t some text  \t \n")
      << QStringLiteral("\n \t {% \t \n \t some text  \t \n");

  // Multiple lines of leading whitespace. Only one leading newline is removed
  QTest::newRow("insignificant-whitespace14")
      << QStringLiteral("\n\n\n{% templatetag openblock %}\n some text\n")
      << dict << QStringLiteral("\n\n{%\n some text\n")
      << QStringLiteral("\n\n\n{%\n some text\n");

  // Trailing whitespace after tag
  QTest::newRow("insignificant-whitespace15")
      << QStringLiteral(
             "\n\n\n{% templatetag openblock %}\t \t \t\n some text\n")
      << dict << QStringLiteral("\n\n{%\t \t \t\n some text\n")
      << QStringLiteral("\n\n\n{%\t \t \t\n some text\n");

  // Removable newline followed by leading whitespace
  QTest::newRow("insignificant-whitespace16")
      << QStringLiteral(
             "\n\n\n\t \t \t{% templatetag openblock %}\n some text\n")
      << dict << QStringLiteral("\n\n{%\n some text\n")
      << QStringLiteral("\n\n\n\t \t \t{%\n some text\n");

  // Removable leading whitespace and trailing whitespace
  QTest::newRow("insignificant-whitespace17")
      << QStringLiteral(
             "\n\n\n\t \t \t{% templatetag openblock %}\t \t \t\n some text\n")
      << dict << QStringLiteral("\n\n{%\t \t \t\n some text\n")
      << QStringLiteral("\n\n\n\t \t \t{%\t \t \t\n some text\n");

  // Multiple lines of trailing whitespace. No trailing newline is removed.
  QTest::newRow("insignificant-whitespace18")
      << QStringLiteral("\n{% templatetag openblock %}\n\n\n some text\n")
      << dict << QStringLiteral("{%\n\n\n some text\n")
      << QStringLiteral("\n{%\n\n\n some text\n");
  QTest::newRow("insignificant-whitespace19")
      << QStringLiteral("\n{% templatetag openblock %}\t \n\n\n some text\n")
      << dict << QStringLiteral("{%\t \n\n\n some text\n")
      << QStringLiteral("\n{%\t \n\n\n some text\n");

  // Consecutive trimmed lines with tags strips one newline each
  QTest::newRow("insignificant-whitespace20")
      << QStringLiteral("\n{% templatetag openblock %}\n{% templatetag openblock %}\n{% templatetag openblock %}\n some text\n")
      << dict << QStringLiteral("{%{%{%\n some text\n")
      << QStringLiteral("\n{%\n{%\n{%\n some text\n");

  // Consecutive trimmed lines with tags strips one newline each. Intermediate
  // newlines are preserved
  QTest::newRow("insignificant-whitespace21")
      << QStringLiteral("\n\n{% templatetag openblock %}\n\n{% templatetag openblock %}\n\n{% templatetag openblock %}\n\n some text\n")
      << dict << QStringLiteral("\n{%\n{%\n{%\n\n some text\n")
      << QStringLiteral("\n\n{%\n\n{%\n\n{%\n\n some text\n");

  // Consecutive trimmed lines with tags strips one newline each. Leading
  // whitespace is stripped but trailing is not
  QTest::newRow("insignificant-whitespace22")
      << QStringLiteral("\n\n\t {% templatetag openblock %}\t \n\n\t {% templatetag openblock %}\t \n\n\t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("\n{%\t \n{%\t \n{%\t \n some text\n")
      << QStringLiteral("\n\n\t {%\t \n\n\t {%\t \n\n\t {%\t \n some text\n");

  // Consecutive trimmed lines with tags strips one newline each. Intermediate
  // whitespace is stripped
  QTest::newRow("insignificant-whitespace23")
      << QStringLiteral("\n\t {% templatetag openblock %}\t \n\t {% templatetag openblock %}\t \n\t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("{%\t {%\t {%\t \n some text\n")
      << QStringLiteral("\n\t {%\t \n\t {%\t \n\t {%\t \n some text\n");

  // Intermediate whitespace on one line is preserved
  // Consecutive tags on one line do not have intermediate whitespace or
  // leading
  // whitespace stripped
  QTest::newRow("insignificant-whitespace24")
      << QStringLiteral("\n\t {% templatetag openblock %}\t \t {% templatetag openblock %}\t \t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("\n\t {%\t \t {%\t \t {%\t \n some text\n")
      << QStringLiteral("\n\t {%\t \t {%\t \t {%\t \n some text\n");

  // Still, only one leading newline is removed.
  QTest::newRow("insignificant-whitespace25")
      << QStringLiteral("\n\n {% templatetag openblock %}\n \t {% templatetag openblock %}\n \t {% templatetag openblock %}\n some text\n")
      << dict << QStringLiteral("\n{%{%{%\n some text\n")
      << QStringLiteral("\n\n {%\n \t {%\n \t {%\n some text\n");

  // Lines with {# comments #} have the same stripping behavior
  QTest::newRow("insignificant-whitespace26")
      << QStringLiteral("\n\n {% templatetag openblock %}\n \t {# some comment #}\n some text\n")
      << dict << QStringLiteral("\n{%\n some text\n")
      << QStringLiteral("\n\n {%\n \t \n some text\n");

  // Only {# comments #}
  QTest::newRow("insignificant-whitespace27")
      << QStringLiteral(
             "\n\n {# a comment #}\n \t {# some comment #}\n some text\n")
      << dict << QStringLiteral("\n\n some text\n")
      << QStringLiteral("\n\n \n \t \n some text\n");

  // Consecutive newlines with tags and comments
  QTest::newRow("insignificant-whitespace28")
      << QStringLiteral("\n\t {% templatetag openblock %}\t \n\t {# some comment #}\t \n\t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("{%\t \t {%\t \n some text\n")
      << QStringLiteral("\n\t {%\t \n\t \t \n\t {%\t \n some text\n");

  dict.insert(QStringLiteral("spam"), QStringLiteral("ham"));
  // Lines with only {{ values }} have the same stripping behavior
  QTest::newRow("insignificant-whitespace29")
      << QStringLiteral("\n {% templatetag openblock %}\t\n \t {{ spam }}\t \n \t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("{%\tham\t {%\t \n some text\n")
      << QStringLiteral("\n {%\t\n \t ham\t \n \t {%\t \n some text\n");
  QTest::newRow("insignificant-whitespace30")
      << QStringLiteral("\n\n {% templatetag openblock %}\t\n\n \t {{ spam }}\t \n\n \t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("\n{%\t\nham\t \n{%\t \n some text\n")
      << QStringLiteral("\n\n {%\t\n\n \t ham\t \n\n \t {%\t \n some text\n");

  // Leading whitespace not stripped when followed by anything. See
  // templatetag-whitespace24
  QTest::newRow("insignificant-whitespace31")
      << QStringLiteral("\n {% templatetag openblock %}\t \t {{ spam }}\t \t {% templatetag openblock %}\t \n some text\n")
      << dict << QStringLiteral("\n {%\t \t ham\t \t {%\t \n some text\n")
      << QStringLiteral("\n {%\t \t ham\t \t {%\t \n some text\n");

  // {{ value }} {% tag %} {{ value }} this time
  QTest::newRow("insignificant-whitespace32")
      << QStringLiteral("\n {{ spam }}\t\n \t {% templatetag openblock %}\t \n \t {{ spam }}\t \n some text\n")
      << dict << QStringLiteral("ham\t{%\t ham\t \n some text\n")
      << QStringLiteral("\n ham\t\n \t {%\t \n \t ham\t \n some text\n");

  // Invalid stuff is still invalid
  // Newlines inside begin-end tokens, even in {# comments #}, make it not a
  // tag.
  QTest::newRow("insignificant-whitespace33")
      << QStringLiteral(
             "\n\n {# \n{% templatetag openblock #}\t \n some text\n")
      << dict << QStringLiteral(
                     "\n\n {# \n{% templatetag openblock #}\t \n some text\n")
      << QStringLiteral(
             "\n\n {# \n{% templatetag openblock #}\t \n some text\n");

  // Complete comment matching tags on one line are processed
  QTest::newRow("insignificant-whitespace34")
      << QStringLiteral(
             "\n\n {# \n{# templatetag openblock #}\t \n some text\n")
      << dict << QStringLiteral("\n\n {# \t \n some text\n")
      << QStringLiteral("\n\n {# \n\t \n some text\n");
  QTest::newRow("insignificant-whitespace35")
      << QStringLiteral(
             "\n\n {# \n{# templatetag openblock\n #}\t \n some text\n")
      << dict << QStringLiteral(
                     "\n\n {# \n{# templatetag openblock\n #}\t \n some text\n")
      << QStringLiteral(
             "\n\n {# \n{# templatetag openblock\n #}\t \n some text\n");
  QTest::newRow("insignificant-whitespace36")
      << QStringLiteral("\n\n {# \n{{ some comment #}\t \n some text\n") << dict
      << QStringLiteral("\n\n {# \n{{ some comment #}\t \n some text\n")
      << QStringLiteral("\n\n {# \n{{ some comment #}\t \n some text\n");
  QTest::newRow("insignificant-whitespace37")
      << QStringLiteral(
             "\n\n {# \n \t {% templatetag openblock #}\t \n some text\n")
      << dict
      << QStringLiteral(
             "\n\n {# \n \t {% templatetag openblock #}\t \n some text\n")
      << QStringLiteral(
             "\n\n {# \n \t {% templatetag openblock #}\t \n some text\n");
  QTest::newRow("insignificant-whitespace38")
      << QStringLiteral("\n\n {# templatetag openblock #\n}\t \n some text\n")
      << dict
      << QStringLiteral("\n\n {# templatetag openblock #\n}\t \n some text\n")
      << QStringLiteral("\n\n {# templatetag openblock #\n}\t \n some text\n");
  QTest::newRow("insignificant-whitespace39")
      << QStringLiteral("\n\n {% templatetag openblock %\n}\t \n some text\n")
      << dict
      << QStringLiteral("\n\n {% templatetag openblock %\n}\t \n some text\n")
      << QStringLiteral("\n\n {% templatetag openblock %\n}\t \n some text\n");
  QTest::newRow("insignificant-whitespace40")
      << QStringLiteral("\n\n {{ templatetag openblock }\n}\t \n some text\n")
      << dict
      << QStringLiteral("\n\n {{ templatetag openblock }\n}\t \n some text\n")
      << QStringLiteral("\n\n {{ templatetag openblock }\n}\t \n some text\n");
  QTest::newRow("insignificant-whitespace41")
      << QStringLiteral(
             "\n\n {\n# {# templatetag openblock #}\t \n some text\n")
      << dict << QStringLiteral("\n\n {\n# \t \n some text\n")
      << QStringLiteral("\n\n {\n# \t \n some text\n");
  QTest::newRow("insignificant-whitespace42")
      << QStringLiteral("\n\n {\n {# templatetag openblock #}\t \n some text\n")
      << dict << QStringLiteral("\n\n {\t \n some text\n")
      << QStringLiteral("\n\n {\n \t \n some text\n");
  QTest::newRow("insignificant-whitespace43")
      << QStringLiteral("\n{{# foo #};{# bar #}\n") << dict
      << QStringLiteral("\n{;\n") << QStringLiteral("\n{;\n");

  QTest::newRow("insignificant-whitespace44") << QStringLiteral("\n{{ foo }} ")
                                              << dict << QString()
                                              << QStringLiteral("\n ");
}

QTEST_MAIN(TestBuiltinSyntax)
#include "testbuiltins.moc"

#endif
