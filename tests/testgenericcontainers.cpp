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

#include "engine.h"
#include "grantlee_paths.h"
#include "metatype.h"
#include "test_macros.h"

#include <QtTest/QtTest>
#include <QtCore/QVariant>

DECLARE_TYPE_CONTAINERS(QVariant)

class TestGenericContainers : public QObject
{
  Q_OBJECT
public:
  explicit TestGenericContainers(QObject* parent = 0);

private slots:
  void testContainer_Builtins();
};

TestGenericContainers::TestGenericContainers(QObject* parent)
  : QObject(parent)
{

}

template<typename T>
QVector<T> getItems()
{
  QVector<T> items;
  items.push_back(9);
  items.push_back(7);
  items.push_back(5);
  return items;
}

template<>
QVector<QVariant> getItems<QVariant>()
{
  QVector<QVariant> items;
  foreach(const int item, getItems<int>())
    items.push_back(item);
  return items;
}

template <typename Container>
struct ContainerPopulator
{
  static void populateSequential( Container &container )
  {
    foreach(const typename Container::value_type item, getItems<typename Container::value_type>())
      container.push_back(item);
  }
  static void populateAssociative( Container &container )
  {
    int i = 0;
    foreach(const typename Container::mapped_type item, getItems<typename Container::mapped_type>())
      container[i++] = item;
  }
};

template<typename T>
struct ContainerPopulator<QSet<T> >
{
  static void populateSequential( QSet<T> &container )
  {
    foreach(const T item, getItems<T>())
      container.insert(item);
  }
};

template<typename T>
struct ContainerPopulator<QMap<QString, T> >
{
  static void populateAssociative( QMap<QString, T> &container )
  {
    int i = 0;
    foreach(const T item, getItems<T>())
      container.insert(QString::number(i++), item);
  }
};

template<typename T>
struct ContainerPopulator<QHash<QString, T> >
{
  static void populateAssociative( QHash<QString, T> &container )
  {
    int i = 0;
    foreach(const T item, getItems<T>())
      container.insert(QString::number(i++), item);
  }
};

template<typename T>
struct ContainerPopulator<std::map<QString, T> >
{
  static void populateAssociative( std::map<QString, T> &container )
  {
    int i = 0;
    foreach(const T item, getItems<T>())
      container[QString::number(i++)] = item;
  }
};

template<typename T>
QString getTemplate()
{
  return QLatin1String( "{% for item in container %}{{ item }},{% endfor %}" );
}

template<typename T>
QString getAssociativeTemplate()
{
  return QLatin1String( "{% for item in container.values %}{{ item }},{% endfor %}" );
}

template <typename T>
QStringList getResults()
{
  return QStringList() << QLatin1String( "9," ) << QLatin1String( "7," ) << QLatin1String( "5," );
}

void testContainer(const QString &stringTemplate, const QVariant &containerVariant, const QStringList &expectedResults, bool unordered)
{
  Grantlee::MetaType::init();
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;
  c.insert( QLatin1String( "container" ), containerVariant );

  Grantlee::Template t1 = engine.newTemplate( stringTemplate, QLatin1String( "template1" ) );

  QString result = t1->render( &c );
  if (!unordered)
    QCOMPARE(result, expectedResults.join(QString()));
  else {
    QVERIFY(result.size() == expectedResults.join(QString()).size());
    foreach (const QString &expectedResult, expectedResults)
      QVERIFY(result.contains(expectedResult));
  }
}

template<typename Container>
void doTestSequentialContainer(bool unordered = false)
{
  Container container;
  ContainerPopulator<Container>::populateSequential( container );

  testContainer(
      getTemplate<typename Container::value_type>(),
      QVariant::fromValue(container),
      getResults<typename Container::value_type>(),
      unordered);
}

template<typename Container>
void doTestAssociativeContainer(bool unordered = false)
{
  Container container;
  ContainerPopulator<Container>::populateAssociative( container );

  testContainer(
      getAssociativeTemplate<typename Container::mapped_type>(),
      QVariant::fromValue(container),
      getResults<typename Container::mapped_type>(),
      unordered);
}

template<typename T>
void doTestNonHashableContainers()
{
  doTestSequentialContainer<QVector<T> >();
  doTestSequentialContainer<QList<T> >();
  doTestSequentialContainer<QLinkedList<T> >();
  doTestSequentialContainer<QQueue<T> >();
  doTestSequentialContainer<QStack<T> >();
  doTestAssociativeContainer<QMap<qint16, T> >();
  doTestAssociativeContainer<QMap<qint32, T> >();
  doTestAssociativeContainer<QMap<qint64, T> >();
  doTestAssociativeContainer<QMap<quint16, T> >();
  doTestAssociativeContainer<QMap<quint32, T> >();
  doTestAssociativeContainer<QMap<quint64, T> >();
  doTestAssociativeContainer<QMap<QString, T> >();
  doTestAssociativeContainer<std::map<qint16, T> >();
  doTestAssociativeContainer<std::map<qint32, T> >();
  doTestAssociativeContainer<std::map<qint64, T> >();
  doTestAssociativeContainer<std::map<quint16, T> >();
  doTestAssociativeContainer<std::map<quint32, T> >();
  doTestAssociativeContainer<std::map<quint64, T> >();
  doTestAssociativeContainer<std::map<QString, T> >();
  doTestAssociativeContainer<QHash<qint16, T> >(true);
  doTestAssociativeContainer<QHash<qint32, T> >(true);
  doTestAssociativeContainer<QHash<qint64, T> >(true);
  doTestAssociativeContainer<QHash<quint16, T> >(true);
  doTestAssociativeContainer<QHash<quint32, T> >(true);
  doTestAssociativeContainer<QHash<quint64, T> >(true);
  doTestAssociativeContainer<QHash<QString, T> >(true);
}

template<typename T>
void doTestContainers()
{
  doTestNonHashableContainers<T>();
}

void TestGenericContainers::testContainer_Builtins()
{
  doTestNonHashableContainers<QVariant>();
}


QTEST_MAIN( TestGenericContainers )
#include "testgenericcontainers.moc"
