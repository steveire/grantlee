/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef LOADERTAGSTEST_H
#define LOADERTAGSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "context.h"
#include "grantlee.h"

#include "grantlee.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Dict)
Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestLoaderTags : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testIncludeTag_data();
  void testIncludeTag() {  doTest();  }

  void testExtendsTag_data();
  void testExtendsTag() {  doTest();  }

private:

  void doTest();

  TemplateLoader *m_tl;

};

void TestLoaderTags::initTestCase()
{
  m_tl = TemplateLoader::instance();

  m_tl->setPluginDirs(QStringList() << "/home/kde-devel/kde/lib/");

}

void TestLoaderTags::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, errorNumber);

  Template* t = m_tl->getTemplate(this);
  
  QSignalSpy spy(t, SIGNAL(error(int, const QString &)));
  t->setContent(input);

  qDebug() << "ok" << spy.count();

  if (spy.count() > 0)
  {
    QVariantList l = spy.takeFirst();
    qDebug() << 1 << l;
    QCOMPARE(l.at(0).toInt(), (int)errorNumber );
    return;
  }

  Context context(dict);
  
  QString result = t->render(&context);

  if (spy.count() > 0)
  {
    QVariantList l = spy.takeFirst();
    qDebug() << 2 << l;
    QCOMPARE(l.at(0).toInt(), (int)errorNumber );
    QCOMPARE(result, QString());
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, errorNumber);

  QCOMPARE(result, output);
}

void TestLoaderTags::testIncludeTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("errorNumber");

  Dict dict;

  m_tl->injectTemplate("basic-syntax01", "Something cool");
  m_tl->injectTemplate("basic-syntax02", "{{ headline }}");

  QTest::newRow("include01") << "{% include \"basic-syntax01\" %}" << dict << "Something cool" << NoError;

  dict.insert("headline", "Included");

  QTest::newRow("include02") << "{% include \"basic-syntax02\" %}" << dict << "Included" << NoError;

  dict.insert("templateName", "basic-syntax02");

  QTest::newRow("include03") << "{% include templateName %}" << dict << "Included" << NoError;

  dict.clear();
  QTest::newRow("include04") << "a{% include \"nonexistent\" %}b" << dict << "ab" << NoError;
  
}


void TestLoaderTags::testExtendsTag_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("errorNumber");

  Dict dict;
  // Basic test
//   QTest::newRow("namedendblocks01") << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock first %}3" << dict << "1_2_3";
  // Unbalanced blocks
  // #C# {}
  // QTest::newRow("namedendblocks02") << "1{% block first %}_{% block second %}2{% endblock first %}_{% endblock second %}3" << dict << NoError;
  // #C# {}
  // QTest::newRow("namedendblocks03") << "1{% block first %}_{% block second %}2{% endblock %}_{% endblock second %}3" << dict << " template.TemplateSyntaxError),";
  // #C# {}
  // QTest::newRow("namedendblocks04") << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock third %}3" << dict << " template.TemplateSyntaxError),";
  // #C# {}
  // QTest::newRow("namedendblocks05") << "1{% block first %}_{% block second %}2{% endblock first %}" << dict << " template.TemplateSyntaxError),";
  // Mixed named and unnamed endblocks
  // #C# {}
  QTest::newRow("namedendblocks06") << "1{% block first %}_{% block second %}2{% endblock %}_{% endblock first %}3" << dict << "1_2_3" << NoError;
  // #C# {}
  QTest::newRow("namedendblocks07") << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock %}3" << dict << "1_2_3" << NoError;


  //## INHERITANCE ###########################################################
  // Standard template with no inheritance

  QString inh1("1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}");
  m_tl->injectTemplate("inheritance01", inh1);

  QTest::newRow("inheritance01") << inh1 << dict << "1&3_" << NoError;

  QString inh2("{% extends \"inheritance01\" %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}");
  m_tl->injectTemplate("inheritance02", inh2);

  // Standard two-level inheritance
  QTest::newRow("inheritance02") << inh2 << dict << "1234" << NoError;
  // Three-level with no redefinitions on third level
  QTest::newRow("inheritance03") << "{% extends 'inheritance02' %}" << dict << "1234" << NoError;
  // Two-level with no redefinitions on second level

  QString inh4("{% extends \"inheritance01\" %}");
  m_tl->injectTemplate("inheritance04", inh4);

  QTest::newRow("inheritance04") << inh4 << dict << "1&3_" << NoError;
  // Two-level with double quotes instead of single quotes
  QTest::newRow("inheritance05") << "{% extends \"inheritance02\" %}" << dict << "1234" << NoError;

  dict.insert("foo", "inheritance02");
  // Three-level with variable parent-template name
  QTest::newRow("inheritance06") << "{% extends foo %}" << dict << "1234" << NoError;

  QString inh7("{% extends 'inheritance01' %}{% block second %}5{% endblock %}");
  m_tl->injectTemplate("inheritance07", inh7);

  dict.clear();
  // Two-level with one block defined, one block not defined
  QTest::newRow("inheritance07") << inh7 << dict << "1&35" << NoError;
  // Three-level with one block defined on this level, two blocks defined next level
  QTest::newRow("inheritance08") << "{% extends 'inheritance02' %}{% block second %}5{% endblock %}" << dict << "1235" << NoError;

  // Three-level with second and third levels blank
  QTest::newRow("inheritance09") << "{% extends 'inheritance04' %}" << dict << "1&3_" << NoError;

  dict.clear();

  // Three-level with space NOT in a block -- should be ignored
  QTest::newRow("inheritance10") << "{% extends 'inheritance04' %}      " << dict << "1&3_" << NoError;
  // Three-level with both blocks defined on this level, but none on second level
  QTest::newRow("inheritance11") << "{% extends 'inheritance04' %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;
  // Three-level with this level providing one and second level providing the other
  QTest::newRow("inheritance12") << "{% extends 'inheritance07' %}{% block first %}2{% endblock %}" << dict << "1235" << NoError;
  // Three-level with this level overriding second level
  QTest::newRow("inheritance13") << "{% extends 'inheritance02' %}{% block first %}a{% endblock %}{% block second %}b{% endblock %}" << dict << "1a3b" << NoError;
  // A block defined only in a child template shouldn't be displayed
  QTest::newRow("inheritance14") << "{% extends 'inheritance01' %}{% block newblock %}NO DISPLAY{% endblock %}" << dict << "1&3_" << NoError;


  QString inh15("{% extends 'inheritance01' %}{% block first %}2{% block inner %}inner{% endblock %}{% endblock %}");
  m_tl->injectTemplate("inheritance15", inh15);

  // A block within another block
  QTest::newRow("inheritance15") << inh15 << dict << "12inner3_" << NoError;
  // A block within another block (level 2)

  QTest::newRow("inheritance16") << "{% extends 'inheritance15' %}{% block inner %}out{% endblock %}" << dict << "12out3" << NoError;
//   // {% load %} tag (parent -- setup for exception04)
//   // #C# {}
//   QTest::newRow("inheritance17") << "{% load testtags %}{% block first %}1234{% endblock %}" << dict << " '1234'),";
//   // {% load %} tag (standard usage, without inheritance)
//   // #C# {}
//   QTest::newRow("inheritance18") << "{% load testtags %}{% echo this that theother %}5678" << dict << " 'this that theother5678'),";
//   // {% load %} tag (within a child template)
//   // #C# {}
//   QTest::newRow("inheritance19") << "{% extends 'inheritance01' %}{% block first %}{% load testtags %}{% echo 400 %}5678{% endblock %}" << dict << " '140056783_'),";

  QString inh20("{% extends 'inheritance01' %}{% block first %}{{ block.super }}a{% endblock %}");
  m_tl->injectTemplate("inheritance20", inh20);

  // Two-level inheritance with {{ block.super }}
  QTest::newRow("inheritance20") << inh20 << dict << "1&a3_" << NoError;
  // Three-level inheritance with {{ block.super }} from parent
  QTest::newRow("inheritance21") << "{% extends 'inheritance02' %}{% block first %}{{ block.super }}a{% endblock %}" << dict << "12a34" << NoError;
  // Three-level inheritance with {{ block.super }} from grandparent
  QTest::newRow("inheritance22") << "{% extends 'inheritance04' %}{% block first %}{{ block.super }}a{% endblock %}" << dict << "1&a3_" << NoError;
  // Three-level inheritance with {{ block.super }} from parent and grandparent
//   QTest::newRow("inheritance23") << "{% extends 'inheritance20' %}{% block first %}{{ block.super }}b{% endblock %}" << dict << "1&ab3_" << NoError;

  // Inheritance from local context without use of template loader

  Template *t = m_tl->getTemplate();
  t->setContent("1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}");
  QObject *obj = t;
  dict.insert("context_template", QVariant::fromValue(obj));

  QTest::newRow("inheritance24") << "{% extends context_template %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;

  dict.clear();
  QVariantList list;

  Template *t1 = m_tl->getTemplate();
  t1->setContent("Wrong");
  QObject *obj1 = t1;
  Template *t2 = m_tl->getTemplate();
  t2->setContent("1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}");
  QObject *obj2 = t2;
  list << QVariant::fromValue(obj1);
  list << QVariant::fromValue(obj2);

  dict.insert("context_template", list);

  // Inheritance from local context with variable parent template
  QTest::newRow("inheritance25") << "{% extends context_template.1 %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;

  dict.clear();

  // Set up a base template to extend
  QString inh26 = QString("no tags");
  m_tl->injectTemplate("inheritance26", inh26);

  // Inheritance from a template that doesn't have any blocks
  QTest::newRow("inheritance27") << "{% extends 'inheritance26' %}" << dict << "no tags" << NoError;

  dict.clear();
  // Raise exception for invalid template name
  QTest::newRow("exception01") << "{% extends 'nonexistent' %}" << dict << "" << TagSyntaxError;
  // Raise exception for invalid template name (in variable)
  QTest::newRow("exception02") << "{% extends nonexistent %}" << dict << "" << TagSyntaxError;
  // Raise exception for extra {% extends %} tags
  QTest::newRow("exception03") << "{% extends 'inheritance01' %}{% block first %}2{% endblock %}{% extends 'inheritance16' %}" << dict << "" << TagSyntaxError;
  // Raise exception for custom tags used in child with {% load %} tag in parent, not in child
  QTest::newRow("exception04") << "{% extends 'inheritance17' %}{% block first %}{% echo 400 %}5678{% endblock %}" << dict << "" << TagSyntaxError;
}


QTEST_MAIN(TestLoaderTags)
#include "testloadertags.moc"

#endif
