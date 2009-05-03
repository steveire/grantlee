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

class TestBuiltins : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void testVariables();
  void testInclude();
  void testExtends();

private:
  TemplateLoader *m_tl;

};

void TestBuiltins::initTestCase()
{
  m_tl = TemplateLoader::instance();

  m_tl->setPluginDirs(QStringList() << "/home/kde-devel/kde/lib/");

  m_tl->injectTemplate("include.html", "This is a {{var}}.");

  m_tl->injectTemplate("extend.html", "This is a test.{% block test %}One{% endblock %}");

}

void TestBuiltins::testVariables()
{
  Template t = m_tl->loadFromString("This is {{foo}}.");
  QHash<QString, QVariant> h;
  h.insert("foo", "bar");
  Context c(h);

  QString result = t.render(&c);

  QVERIFY(result == "This is bar.");

  h.clear();

  h.insert("foo", "bat");
  Context c2(h);

  QString result2 = t.render(&c2);

  QVERIFY(result2 == "This is bat.");

}

void TestBuiltins::testInclude()
{
  Template t = m_tl->loadFromString("foo {% include \"include.html\" %} bar");

  QHash<QString, QVariant> h;
  h.insert("var", "test");
  h.insert("includevar", "include.html");
  Context c(h);

  QString result = t.render(&c);

  QVERIFY(result == "foo This is a test. bar");

  t = m_tl->loadFromString("foo {% include includevar %} bar");

  result = t.render(&c);

  QVERIFY(result == "foo This is a test. bar");

}

void TestBuiltins::testExtends()
{
  Template t = m_tl->loadFromString("{% extends \"extend.html\" %} {% block test %} {{ block.super }} Two {% endblock %}");

  QHash<QString, QVariant> h;
  Context c(h);

  QString result = t.render(&c);

  QVERIFY(result == "This is a test. One Two ");

}

QTEST_MAIN(TestBuiltins)
#include "testbuiltins.moc"

#endif
