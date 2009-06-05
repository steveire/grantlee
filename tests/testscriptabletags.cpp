/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef BUILTINSTEST_H
#define BUILTINSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "template.h"
#include "templateloader.h"
#include "context.h"
#include "grantlee.h"
#include "filterexpression.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE(Dict)
Q_DECLARE_METATYPE(Grantlee::Error)

using namespace Grantlee;

class TestScriptableTagsSyntax : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testBasicSyntax_data();
  void testBasicSyntax() { doTest(); }

  void cleanupTestCase();

private:

  void doTest();

  Engine *m_tl;

};

void TestScriptableTagsSyntax::initTestCase()
{
  m_tl = Engine::instance();

  QString appDirPath = QFileInfo(QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_tl->setPluginDirs(QStringList() << appDirPath + "/grantlee_defaulttags/"
                                    << appDirPath + "/grantlee_scriptabletags/" );
}

void TestScriptableTagsSyntax::cleanupTestCase()
{
  delete m_tl;
}

void TestScriptableTagsSyntax::doTest()
{
  QFETCH(QString, input);
  QFETCH(Dict, dict);
  QFETCH(QString, output);
  QFETCH(Grantlee::Error, error);

  Template* t = new Template(this);
  t->setContent(input);

  Context context(dict);

  QString result = t->render(&context);

  if (t->error() != NoError)
  {
    QCOMPARE(t->error(), error );
    return;
  }


  QCOMPARE(t->error(), NoError);

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE(NoError, error);

  QCOMPARE(t->error(), NoError);


  QCOMPARE(result, output);
}

void TestScriptableTagsSyntax::testBasicSyntax_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<Dict>("dict");
  QTest::addColumn<QString>("output");
  QTest::addColumn<Grantlee::Error>("error");

  Dict dict;

  dict.insert("boo", "Far");
  dict.insert("booList", QVariantList() << "Tom" << "Dick" << "Harry");

  QTest::newRow("scriptable-tags01") << "{% load scripteddefaults %}{% if2 \"something\\\" stupid\" %}{{ boo }}{% endif2 %}" << dict << "Far" << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow("scriptable-tags02") << "{% load scripteddefaults %}{% if2 \"something\\\" stupid\" %}{% for letter in boo %}:{{ letter }};{% endfor %}{% endif2 %}" << dict << ":F;:a;:r;" << NoError;

  // Nest c++ tags inside scripted tags.
  QTest::newRow("scriptable-tags03") << "{% load scripteddefaults %}{% if2 boo %}yes{% else %}no{% endif2 %}" << dict << "yes" << NoError;
  QTest::newRow("scriptable-tags04") << "{% load scripteddefaults %}{% if2 foo %}yes{% else %}no{% endif2 %}" << dict << "no" << NoError;

  QTest::newRow("scriptable-tags05") << "{% load scripteddefaults %}{{ boo|upper }}" << dict << "FAR" << NoError;


  dict.insert("boo", "Far & away");
  // Variables are escaped ...
  QTest::newRow("scriptable-tags06") << "{% load scripteddefaults %}{{ boo }}" << dict << "Far &amp; away" << NoError;
  // and scripted filters can mark output as 'safe'.
  QTest::newRow("scriptable-tags07") << "{% load scripteddefaults %}{{ boo|safe2 }}" << dict << "Far & away" << NoError;

  // Filters can take arguments
  QTest::newRow("scriptable-tags08") << "{% load scripteddefaults %}{{ booList|join2:\" \" }}" << dict << "Tom Dick Harry" << NoError;

  // Literals in tags are compared un-escaped.
  QTest::newRow("scriptable-tags09") << "{% load scripteddefaults %}{% ifequal2 boo \"Far & away\" %}yes{% endifequal2 %}" << dict << "yes" << NoError;

  // Literals in arguments to filters are not escaped.
  QTest::newRow("scriptable-tags10") << "{% load scripteddefaults %}{{ booList|join2:\" & \" }}" << dict << "Tom & Dick & Harry" << NoError;

  // Nor are variables.
  dict.insert("amp", " & ");
  QTest::newRow("scriptable-tags11") << "{% load scripteddefaults %}{{ booList|join2:amp }}" << dict << "Tom & Dick & Harry" << NoError;



  dict.clear();
}

QTEST_MAIN(TestScriptableTagsSyntax)
#include "testscriptabletags.moc"

#endif
