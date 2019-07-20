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

#ifndef LOADERTAGSTEST_H
#define LOADERTAGSTEST_H

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtTest/QTest>

#include "context.h"
#include "coverageobject.h"
#include "engine.h"
#include "grantlee_paths.h"
#include "template.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestLoaderTags : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void testIncludeTag_data();
  void testIncludeTag() { doTest(); }

  void testExtendsTag_data();
  void testExtendsTag() { doTest(); }

  void testBlockTagErrors_data();
  void testBlockTagErrors() { doTest(); }

private:
  void doTest();

  QSharedPointer<InMemoryTemplateLoader> loader;
  Engine *m_engine;
};

void TestLoaderTags::initTestCase()
{
  Q_INIT_RESOURCE(testresource);

  m_engine = new Engine(this);

  loader = QSharedPointer<InMemoryTemplateLoader>(new InMemoryTemplateLoader());
  m_engine->addTemplateLoader(loader);

  auto appDirPath
      = QFileInfo(QCoreApplication::applicationDirPath()).absoluteDir().path();
  m_engine->setPluginPaths({
      QStringLiteral(GRANTLEE_PLUGIN_PATH),
      QStringLiteral(":/plugins/") // For testtags.qs
  });
#ifdef HAVE_QTQML_LIB
  m_engine->addDefaultLibrary(QStringLiteral("grantlee_scriptabletags"));
#endif
}

void TestLoaderTags::cleanupTestCase() { delete m_engine; }

void TestLoaderTags::doTest()
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

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  QCOMPARE(t->error(), NoError);

  QCOMPARE(result, output);
}

void TestLoaderTags::testIncludeTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  loader->setTemplate(QStringLiteral("basic-syntax01"),
                      QStringLiteral("Something cool"));
  loader->setTemplate(QStringLiteral("basic-syntax02"),
                      QStringLiteral("{{ headline }}"));

  QTest::newRow("include01") << "{% include \"basic-syntax01\" %}" << dict
                             << QStringLiteral("Something cool") << NoError;

  dict.insert(QStringLiteral("headline"), QStringLiteral("Included"));

  QTest::newRow("include02") << "{% include \"basic-syntax02\" %}" << dict
                             << QStringLiteral("Included") << NoError;

  dict.insert(QStringLiteral("templateName"), QStringLiteral("basic-syntax02"));

  QTest::newRow("include03") << QStringLiteral("{% include templateName %}")
                             << dict << QStringLiteral("Included") << NoError;

  dict.clear();
  QTest::newRow("include04") << "a{% include \"nonexistent\" %}b" << dict
                             << QStringLiteral("ab") << TagSyntaxError;

  auto incl05 = QStringLiteral("template with a space");
  loader->setTemplate(QStringLiteral("include 05"), incl05);

  QTest::newRow("include 05")
      << incl05 << dict << QStringLiteral("template with a space") << NoError;

  QTest::newRow("include06")
      << "{% include \"include 05\" %}" << dict
      << QStringLiteral("template with a space") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("list"), QVariantList()
                                          << QVariant() << QVariant());
  QTest::newRow("include07")
      << "{% for i in list %}{% include \"include 05\" %}{% endfor %}" << dict
      << QStringLiteral("template with a spacetemplate with a space")
      << NoError;
}

void TestLoaderTags::testExtendsTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;
  // Basic test
  QTest::newRow("namedendblocks01")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "second %}_{% endblock first %}3")
      << dict << QStringLiteral("1_2_3") << NoError;
  // Unbalanced blocks
  QTest::newRow("namedendblocks02")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "first %}_{% endblock second %}3")
      << dict << QString() << InvalidBlockTagError;
  QTest::newRow("namedendblocks03")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "%}_{% endblock second %}3")
      << dict << QString() << InvalidBlockTagError;
  QTest::newRow("namedendblocks04")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "second %}_{% endblock third %}3")
      << dict << QString() << InvalidBlockTagError;
  QTest::newRow("namedendblocks05")
      << QStringLiteral(
             "1{% block first %}_{% block second %}2{% endblock first %}")
      << dict << QString() << InvalidBlockTagError;
  // Mixed named and unnamed endblocks
  QTest::newRow("namedendblocks06")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "%}_{% endblock first %}3")
      << dict << QStringLiteral("1_2_3") << NoError;
  QTest::newRow("namedendblocks07")
      << QStringLiteral("1{% block first %}_{% block second %}2{% endblock "
                        "second %}_{% endblock %}3")
      << dict << QStringLiteral("1_2_3") << NoError;

  //## INHERITANCE ###########################################################
  // Standard template with no inheritance

  auto inh1 = QStringLiteral(
      "1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance01"), inh1);

  QTest::newRow("inheritance01")
      << inh1 << dict << QStringLiteral("1&3_") << NoError;

  auto inh2
      = QStringLiteral("{% extends \"inheritance01\" %}{% block first %}2{% "
                       "endblock %}{% block second %}4{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance02"), inh2);

  // Standard two-level inheritance
  QTest::newRow("inheritance02")
      << inh2 << dict << QStringLiteral("1234") << NoError;
  // Three-level with no redefinitions on third level
  QTest::newRow("inheritance03")
      << QStringLiteral("{% extends 'inheritance02' %}") << dict
      << QStringLiteral("1234") << NoError;
  // Two-level with no redefinitions on second level

  auto inh4 = QStringLiteral("{% extends \"inheritance01\" %}");
  loader->setTemplate(QStringLiteral("inheritance04"), inh4);

  QTest::newRow("inheritance04")
      << inh4 << dict << QStringLiteral("1&3_") << NoError;
  // Two-level with double quotes instead of single quotes
  QTest::newRow("inheritance05") << "{% extends \"inheritance02\" %}" << dict
                                 << QStringLiteral("1234") << NoError;

  dict.insert(QStringLiteral("foo"), QStringLiteral("inheritance02"));
  // Three-level with variable parent-template name
  QTest::newRow("inheritance06") << QStringLiteral("{% extends foo %}") << dict
                                 << QStringLiteral("1234") << NoError;

  auto inh7 = QStringLiteral(
      "{% extends 'inheritance01' %}{% block second %}5{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance07"), inh7);

  dict.clear();
  // Two-level with one block defined, one block not defined
  QTest::newRow("inheritance07")
      << inh7 << dict << QStringLiteral("1&35") << NoError;
  // Three-level with one block defined on this level, two blocks defined next
  // level
  QTest::newRow("inheritance08") << QStringLiteral(
      "{% extends 'inheritance02' %}{% block second %}5{% endblock %}")
                                 << dict << QStringLiteral("1235") << NoError;

  // Three-level with second and third levels blank
  QTest::newRow("inheritance09")
      << QStringLiteral("{% extends 'inheritance04' %}") << dict
      << QStringLiteral("1&3_") << NoError;

  dict.clear();

  // Three-level with space NOT in a block -- should be ignored
  QTest::newRow("inheritance10")
      << QStringLiteral("{% extends 'inheritance04' %}      ") << dict
      << QStringLiteral("1&3_") << NoError;
  // Three-level with both blocks defined on this level, but none on second
  // level
  QTest::newRow("inheritance11")
      << QStringLiteral("{% extends 'inheritance04' %}{% block first %}2{% "
                        "endblock %}{% block second %}4{% endblock %}")
      << dict << QStringLiteral("1234") << NoError;
  // Three-level with this level providing one and second level providing the
  // other
  QTest::newRow("inheritance12") << QStringLiteral(
      "{% extends 'inheritance07' %}{% block first %}2{% endblock %}")
                                 << dict << QStringLiteral("1235") << NoError;
  // Three-level with this level overriding second level
  QTest::newRow("inheritance13")
      << QStringLiteral("{% extends 'inheritance02' %}{% block first %}a{% "
                        "endblock %}{% block second %}b{% endblock %}")
      << dict << QStringLiteral("1a3b") << NoError;
  // A block defined only in a child template shouldn't be displayed
  QTest::newRow("inheritance14")
      << QStringLiteral("{% extends 'inheritance01' %}{% block newblock %}NO "
                        "DISPLAY{% endblock %}")
      << dict << QStringLiteral("1&3_") << NoError;

  auto inh15
      = QStringLiteral("{% extends 'inheritance01' %}{% block first %}2{% "
                       "block inner %}inner{% endblock %}{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance15"), inh15);

  // A block within another block
  QTest::newRow("inheritance15")
      << inh15 << dict << QStringLiteral("12inner3_") << NoError;
  // A block within another block (level 2)

  QTest::newRow("inheritance16")
      << QStringLiteral(
             "{% extends 'inheritance15' %}{% block inner %}out{% endblock %}")
      << dict << QStringLiteral("12out3_") << NoError;

#ifdef HAVE_QTQML_LIB
  // {% load %} tag (parent -- setup for exception04)
  auto inh17 = QStringLiteral(
      "{% load testtags %}{% block first %}1234{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance17"), inh17);

  dict.clear();
  QTest::newRow("inheritance17")
      << inh17 << dict << QStringLiteral("1234") << NoError;

  // {% load %} tag (standard usage, without inheritance)
  QTest::newRow("inheritance18")
      << QStringLiteral("{% load testtags %}{% echo this that theother %}5678")
      << dict << QStringLiteral("this that theother5678") << NoError;

  // {% load %} tag (within a child template)
  QTest::newRow("inheritance19")
      << QStringLiteral("{% extends 'inheritance01' %}{% block first %}{% load "
                        "testtags %}{% echo 400 %}5678{% endblock %}")
      << dict << QStringLiteral("140056783_") << NoError;
#endif

  auto inh20 = QStringLiteral("{% extends 'inheritance01' %}{% block first "
                              "%}{{ block.super }}a{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance20"), inh20);

  // Two-level inheritance with {{ block.super }}
  QTest::newRow("inheritance20")
      << inh20 << dict << QStringLiteral("1&a3_") << NoError;
  // Three-level inheritance with {{ block.super }} from parent
  QTest::newRow("inheritance21")
      << QStringLiteral("{% extends 'inheritance02' %}{% block first %}{{ "
                        "block.super }}a{% endblock %}")
      << dict << QStringLiteral("12a34") << NoError;
  // Three-level inheritance with {{ block.super }} from grandparent
  QTest::newRow("inheritance22")
      << QStringLiteral("{% extends 'inheritance04' %}{% block first %}{{ "
                        "block.super }}a{% endblock %}")
      << dict << QStringLiteral("1&a3_") << NoError;
  // Three-level inheritance with {{ block.super }} from parent and
  // grandparent
  QTest::newRow("inheritance23")
      << QStringLiteral("{% extends 'inheritance20' %}{% block first %}{{ "
                        "block.super }}b{% endblock %}")
      << dict << QStringLiteral("1&ab3_") << NoError;

  // Inheritance from local context without use of template loader

  auto t = m_engine->newTemplate(
      QStringLiteral("1{% block first %}_{% endblock %}3{% block second %}_{% "
                     "endblock %}"),
      QStringLiteral("context_template"));
  dict.insert(QStringLiteral("context_template"), QVariant::fromValue(t));

  QTest::newRow("inheritance24")
      << QStringLiteral("{% extends context_template %}{% block first %}2{% "
                        "endblock %}{% block second %}4{% endblock %}")
      << dict << QStringLiteral("1234") << NoError;

  dict.clear();
  QVariantList list;

  auto t1 = m_engine->newTemplate(QStringLiteral("Wrong"),
                                  QStringLiteral("context_template_1"));
  auto t2 = m_engine->newTemplate(
      QStringLiteral("1{% block first %}_{% endblock %}3{% block second %}_{% "
                     "endblock %}"),
      QStringLiteral("context_template_2"));
  list << QVariant::fromValue(t1);
  list << QVariant::fromValue(t2);

  dict.insert(QStringLiteral("context_template"), list);

  // Inheritance from local context with variable parent template
  QTest::newRow("inheritance25")
      << QStringLiteral("{% extends context_template.1 %}{% block first %}2{% "
                        "endblock %}{% block second %}4{% endblock %}")
      << dict << QStringLiteral("1234") << NoError;

  dict.clear();

  // Set up a base template to extend
  auto inh26 = QStringLiteral("no tags");
  loader->setTemplate(QStringLiteral("inheritance26"), inh26);

  // Inheritance from a template that doesn't have any blocks
  QTest::newRow("inheritance27")
      << QStringLiteral("{% extends 'inheritance26' %}") << dict
      << QStringLiteral("no tags") << NoError;

  auto inh28 = QStringLiteral("{% block first %}!{% endblock %}");
  loader->setTemplate(QStringLiteral("inheritance 28"), inh28);

  QTest::newRow("inheritance 28")
      << inh28 << dict << QStringLiteral("!") << NoError;

  // Inheritance from a template with a space in its name should work.
  QTest::newRow("inheritance29")
      << QStringLiteral("{% extends 'inheritance 28' %}") << dict
      << QStringLiteral("!") << NoError;

  dict.insert(QStringLiteral("optional"), QStringLiteral("True"));

  auto inh30 = QStringLiteral(
      "1{% if optional %}{% block opt %}2{% endblock %}{% endif %}3");
  loader->setTemplate(QStringLiteral("inheritance30"), inh30);

  QTest::newRow("inheritance30")
      << inh30 << dict << QStringLiteral("123") << NoError;
  QTest::newRow("inheritance31") << QStringLiteral(
      "{% extends 'inheritance30' %}{% block opt %}two{% endblock %}")
                                 << dict << QStringLiteral("1two3") << NoError;
  dict.clear();
  QTest::newRow("inheritance32") << QStringLiteral(
      "{% extends 'inheritance30' %}{% block opt %}two{% endblock %}")
                                 << dict << QStringLiteral("13") << NoError;

  dict.insert(QStringLiteral("optional"), 1);
  auto inh33 = QStringLiteral("1{% ifequal optional 1 %}{% block opt %}2{% "
                              "endblock %}{% endifequal %}3");
  loader->setTemplate(QStringLiteral("inheritance33"), inh33);

  QTest::newRow("inheritance33")
      << inh33 << dict << QStringLiteral("123") << NoError;

  QTest::newRow("inheritance34") << QStringLiteral(
      "{% extends 'inheritance33' %}{% block opt %}two{% endblock %}")
                                 << dict << QStringLiteral("1two3") << NoError;
  dict.clear();
  QTest::newRow("inheritance35") << QStringLiteral(
      "{% extends 'inheritance33' %}{% block opt %}two{% endblock %}")
                                 << dict << QStringLiteral("13") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("numbers"), QVariantList() << 1 << 2 << 3);

  auto inh36 = QStringLiteral("{% for n in numbers %}_{% block opt %}{{ n }}{% "
                              "endblock %}{% endfor %}_");
  loader->setTemplate(QStringLiteral("inheritance36"), inh36);

  QTest::newRow("inheritance36")
      << inh36 << dict << QStringLiteral("_1_2_3_") << NoError;
  QTest::newRow("inheritance37")
      << QStringLiteral(
             "{% extends 'inheritance36' %}{% block opt %}X{% endblock %}")
      << dict << QStringLiteral("_X_X_X_") << NoError;
  dict.clear();
  QTest::newRow("inheritance38") << QStringLiteral(
      "{% extends 'inheritance36' %}{% block opt %}X{% endblock %}")
                                 << dict << QStringLiteral("_") << NoError;

  dict.insert(QStringLiteral("optional"), QStringLiteral("True"));

  QTest::newRow("inheritance39")
      << QStringLiteral("{% extends 'inheritance30' %}{% block opt %}new{{ "
                        "block.super }}{% endblock %}")
      << dict << QStringLiteral("1new23") << NoError;

  dict.insert(QStringLiteral("optional"), 1);

  QTest::newRow("inheritance40")
      << QStringLiteral("{% extends 'inheritance33' %}{% block opt %}new{{ "
                        "block.super }}{% endblock %}")
      << dict << QStringLiteral("1new23") << NoError;

  dict.clear();
  dict.insert(QStringLiteral("numbers"), QVariantList() << 1 << 2 << 3);

  QTest::newRow("inheritance41")
      << QStringLiteral("{% extends 'inheritance36' %}{% block opt %}new{{ "
                        "block.super }}{% endblock %}")
      << dict << QStringLiteral("_new1_new2_new3_") << NoError;

  QTest::newRow("inheritance42")
      << QStringLiteral("{% extends 'inheritance02'|cut:' ' %}") << dict
      << QStringLiteral("1234") << NoError;

  dict.clear();
  // Raise exception for invalid template name
  QTest::newRow("exception01") << QStringLiteral("{% extends 'nonexistent' %}")
                               << dict << QString() << TagSyntaxError;
  // Raise exception for invalid template name (in variable)
  QTest::newRow("exception02") << QStringLiteral("{% extends nonexistent %}")
                               << dict << QString() << TagSyntaxError;
  // Raise exception for extra {% extends %} tags
  QTest::newRow("exception03")
      << QStringLiteral("{% extends 'inheritance01' %}{% block first %}2{% "
                        "endblock %}{% extends 'inheritance16' %}")
      << dict << QString() << TagSyntaxError;
  // Raise exception for custom tags used in child with {% load %} tag in
  // parent, not in child
  QTest::newRow("exception04")
      << QStringLiteral("{% extends 'inheritance17' %}{% block first %}{% echo "
                        "400 %}5678{% endblock %}")
      << dict << QString() << InvalidBlockTagError;
}

void TestLoaderTags::testBlockTagErrors_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  QTest::newRow("block-error-01") << QStringLiteral(
      "{% block repeat %}{% endblock %}{% block repeat %}{% endblock %}")
                                  << dict << QString() << TagSyntaxError;
  QTest::newRow("block-error-02") << QStringLiteral("{% block %}{% endblock %}")
                                  << dict << QString() << TagSyntaxError;
  QTest::newRow("block-error-03")
      << QStringLiteral("{% block foo bar %}{% endblock %}") << dict
      << QString() << TagSyntaxError;
}

QTEST_MAIN(TestLoaderTags)
#include "testloadertags.moc"

#endif
