/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#define MINIMAL_CONTAINER_TESTS

#include "engine.h"
#include "grantlee_paths.h"
#include "metatype.h"
#include "test_macros.h"

#include "coverageobject.h"
#include <QtCore/QLinkedList>
#include <QtCore/QQueue>
#include <QtCore/QStack>
#include <QtCore/QVariant>
#include <QtTest/QTest>

class TestGenericContainers : public CoverageObject
{
  Q_OBJECT
public:
  explicit TestGenericContainers(QObject *parent = {});

private Q_SLOTS:
  void testContainer_Builtins();
};

TestGenericContainers::TestGenericContainers(QObject *parent)
    : CoverageObject(parent)
{
}

template <typename T> QVector<T> getItems()
{
  QVector<T> items;
  items.push_back(9);
  items.push_back(7);
  items.push_back(5);
  return items;
}

template <> QVector<QString> getItems<QString>()
{
  QVector<QString> items;
  Q_FOREACH (const int item, getItems<int>())
    items.push_back(QString::number(item));
  return items;
}

template <> QVector<QVariant> getItems<QVariant>()
{
  QVector<QVariant> items;
  Q_FOREACH (const int item, getItems<int>())
    items.push_back(item);
  return items;
}

template <> QVector<QDateTime> getItems<QDateTime>()
{
  QVector<QDateTime> items;
  items.reserve(3);
  for (auto i = 0; i < 3; ++i) {
    QDateTime d;
    d.setTime_t(0);
    d = d.addDays(i);
    items.push_back(d);
  }
  return items;
}

template <> QVector<QObject *> getItems<QObject *>()
{
  QVector<QObject *> items;
  items.reserve(3);
  for (auto i = 9; i > 4; i -= 2) {
    auto obj = new QObject;

    obj->setObjectName(QString::number(i));
    items.push_back(obj);
  }
  return items;
}

template <typename Container> struct ContainerPopulator {
  static void populateSequential(Container &container)
  {
    Q_FOREACH (const typename Container::value_type item,
               getItems<typename Container::value_type>())
      container.push_back(item);
  }
  static void populateAssociative(Container &container)
  {
    auto i = 0;
    Q_FOREACH (const typename Container::mapped_type item,
               getItems<typename Container::mapped_type>())
      container[i++] = item;
  }
};

template <typename T> struct ContainerPopulator<QSet<T>> {
  static void populateSequential(QSet<T> &container)
  {
    Q_FOREACH (const T item, getItems<T>())
      container.insert(item);
  }
};

template <typename T> struct ContainerPopulator<QMap<QString, T>> {
  static void populateAssociative(QMap<QString, T> &container)
  {
    auto i = 0;
    Q_FOREACH (const T item, getItems<T>())
      container.insert(QString::number(i++), item);
  }
};

template <typename T> struct ContainerPopulator<QHash<QString, T>> {
  static void populateAssociative(QHash<QString, T> &container)
  {
    auto i = 0;
    Q_FOREACH (const T item, getItems<T>())
      container.insert(QString::number(i++), item);
  }
};

template <typename T> struct ContainerPopulator<std::map<QString, T>> {
  static void populateAssociative(std::map<QString, T> &container)
  {
    auto i = 0;
    Q_FOREACH (const T item, getItems<T>())
      container[QString::number(i++)] = item;
  }
};

template <typename T> QString getTemplate()
{
  return QStringLiteral("{{ container.size }};{{ container.count }};{% for "
                        "item in container %}{{ item }},{% endfor %}");
}

template <> QString getTemplate<QDateTime>()
{
  return QStringLiteral("{{ container.size }};{{ container.count }};{% for "
                        "item in container %}{{ item|date }},{% endfor %}");
}

template <> QString getTemplate<QObject *>()
{
  return QStringLiteral(
      "{{ container.size }};{{ container.count }};{% for item in container "
      "%}{{ item.objectName }},{% endfor %}");
}

template <typename T> QString getAssociativeTemplate()
{
  return QStringLiteral("{{ container.size }};{{ container.count }};{% for "
                        "item in container.values %}{{ item }},{% endfor %}");
}

template <> QString getAssociativeTemplate<QDateTime>()
{
  return QStringLiteral(
      "{{ container.size }};{{ container.count }};{% for item in "
      "container.values %}{{ item|date }},{% endfor %}");
}

template <> QString getAssociativeTemplate<QObject *>()
{
  return QStringLiteral(
      "{{ container.size }};{{ container.count }};{% for item in "
      "container.values %}{{ item.objectName }},{% endfor %}");
}

template <typename T> QStringList getResults()
{
  return {QStringLiteral("3;3;"), QStringLiteral("9,"), QStringLiteral("7,"),
          QStringLiteral("5,")};
}

template <> QStringList getResults<QDateTime>()
{
  return {QStringLiteral("3;3;"), QStringLiteral("Jan. 1, 1970,"),
          QStringLiteral("Jan. 2, 1970,"), QStringLiteral("Jan. 3, 1970,")};
}

template <typename Container, typename T = typename Container::value_type>
struct CleanupSequentialContainer {
  static void clean(Container &) {}
};

template <typename Container, typename T = typename Container::mapped_type>
struct CleanupAssociativeContainer {
  static void clean(Container &) {}
};

template <typename Container>
struct CleanupSequentialContainer<Container, QObject *> {
  static void clean(Container &c) { qDeleteAll(c); }
};

template <typename Container>
struct CleanupAssociativeContainer<Container, QObject *> {
  static void clean(Container &c) { qDeleteAll(c); }
};

template <typename T>
struct CleanupAssociativeContainer<std::map<T, QObject *>, QObject *> {
  static void clean(std::map<T, QObject *> &c)
  {
    typename std::map<T, QObject *>::iterator it = c.begin();
    const typename std::map<T, QObject *>::iterator end = c.end();
    for (; it != end; ++it) {
      delete it->second;
      it->second = 0;
    }
  }
};

template <typename Container> void cleanupSequential(Container c)
{
  CleanupSequentialContainer<Container>::clean(c);
}

template <typename Container> void cleanupAssociative(Container c)
{
  CleanupAssociativeContainer<Container>::clean(c);
}

void testContainer(const QString &stringTemplate,
                   const QVariant &containerVariant,
                   const QStringList &expectedResults, bool unordered)
{
  Grantlee::Engine engine;

  engine.setPluginPaths({QStringLiteral(GRANTLEE_PLUGIN_PATH)});

  Grantlee::Context c;
  c.insert(QStringLiteral("container"), containerVariant);

  auto t1 = engine.newTemplate(stringTemplate, QStringLiteral("template1"));

  auto result = t1->render(&c);
  if (!unordered)
    QCOMPARE(result, expectedResults.join(QString()));
  else {
    QVERIFY(result.size() == expectedResults.join(QString()).size());
    Q_FOREACH (const QString &expectedResult, expectedResults)
      QVERIFY(result.contains(expectedResult));
  }

  auto t2 = engine.newTemplate(QStringLiteral("-{{ container.doesnotexist }}-"),
                               QStringLiteral("template2"));

  auto result2 = t2->render(&c);

  QCOMPARE(result2, QStringLiteral("--"));
}

template <typename Container>
void doTestSequentialContainer(bool unordered = {})
{
  Container container;
  ContainerPopulator<Container>::populateSequential(container);

  testContainer(getTemplate<typename Container::value_type>(),
                QVariant::fromValue(container),
                getResults<typename Container::value_type>(), unordered);
  cleanupSequential(container);
}

template <typename Container>
void doTestAssociativeContainer(bool unordered = {})
{
  Container container;
  ContainerPopulator<Container>::populateAssociative(container);

  testContainer(getAssociativeTemplate<typename Container::mapped_type>(),
                QVariant::fromValue(container),
                getResults<typename Container::mapped_type>(), unordered);
  cleanupAssociative(container);
}

template <typename T> void doTestNonHashableContainers()
{
  doTestSequentialContainer<QVector<T>>();
  doTestSequentialContainer<QList<T>>();
  doTestSequentialContainer<QLinkedList<T>>();
  doTestSequentialContainer<QQueue<T>>();
  doTestSequentialContainer<QStack<T>>();
  doTestSequentialContainer<std::list<T>>();
  doTestAssociativeContainer<QMap<qint32, T>>();
  doTestAssociativeContainer<std::map<qint32, T>>();
  doTestAssociativeContainer<QHash<qint32, T>>(true);
#ifndef MINIMAL_CONTAINER_TESTS
  doTestAssociativeContainer<QMap<qint16, T>>();
  doTestAssociativeContainer<QMap<qint64, T>>();
  doTestAssociativeContainer<QMap<quint16, T>>();
  doTestAssociativeContainer<QMap<quint32, T>>();
  doTestAssociativeContainer<QMap<quint64, T>>();
  doTestAssociativeContainer<QMap<QString, T>>();
  doTestAssociativeContainer<std::map<qint16, T>>();
  doTestAssociativeContainer<std::map<qint64, T>>();
  doTestAssociativeContainer<std::map<quint16, T>>();
  doTestAssociativeContainer<std::map<quint32, T>>();
  doTestAssociativeContainer<std::map<quint64, T>>();
  doTestAssociativeContainer<std::map<QString, T>>();
  doTestAssociativeContainer<QHash<qint16, T>>(true);
  doTestAssociativeContainer<QHash<qint64, T>>(true);
  doTestAssociativeContainer<QHash<quint16, T>>(true);
  doTestAssociativeContainer<QHash<quint32, T>>(true);
  doTestAssociativeContainer<QHash<quint64, T>>(true);
  doTestAssociativeContainer<QHash<QString, T>>(true);
#endif
}

template <typename T> void doTestContainers()
{
  doTestNonHashableContainers<T>();
  doTestSequentialContainer<QSet<T>>(true);
}

void TestGenericContainers::testContainer_Builtins()
{
  doTestContainers<qint32>();
#ifndef MINIMAL_CONTAINER_TESTS
  doTestContainers<qint16>();
  doTestContainers<qint64>();
  doTestContainers<quint16>();
  doTestContainers<quint32>();
  doTestContainers<quint64>();
  doTestNonHashableContainers<float>();
  doTestNonHashableContainers<double>();
  doTestContainers<QString>();
  doTestNonHashableContainers<QVariant>();
  doTestNonHashableContainers<QDateTime>();
  doTestContainers<QObject *>();
#endif
}

QTEST_MAIN(TestGenericContainers)
#include "testgenericcontainers.moc"
