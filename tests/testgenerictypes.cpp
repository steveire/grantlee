/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Michael Jansen <kde@michael-jansen.biz>
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

#include "metatype.h"
#include "engine.h"
#include "grantlee_paths.h"
#include "template.h"
#include "qtunorderedmap.h"

#include <QtTest/QtTest>
#include <QtCore/QVariant>
#include <QtCore/QVariantHash>
#include <QtCore/QMetaType>

#include <string>

#include <tr1/array>
#include <tr1/memory>

template <typename T>
struct ThreeArray : public std::tr1::array<T, 3>
{

};

GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER           (ThreeArray)
GRANTLEE_STL_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR  (ThreeArray)

Q_DECLARE_METATYPE(QVector<QVariant>)
Q_DECLARE_METATYPE(QStack<QVariant>)
Q_DECLARE_METATYPE(QQueue<QVariant>)
Q_DECLARE_METATYPE(QLinkedList<QVariant>)

GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER      (QtUnorderedMap)
GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR  (QtUnorderedMap)

GRANTLEE_SMART_PTR_ACCESSOR(std::tr1::shared_ptr)

namespace Grantlee {

template<typename T, typename U>
struct KeyGetter<QtUnorderedMap<T, U> > : public Getter<QtUnorderedMap<T, U> >
{
  static T get(typename QtUnorderedMap<T, U>::const_iterator it)
  {
    return it->first;
  }
};

template<typename T, typename U>
struct MappedValueGetter<QtUnorderedMap<T, U> > : public Getter<QtUnorderedMap<T, U> >
{
  static U get(typename QtUnorderedMap<T, U>::const_iterator it)
  {
    return it->second;
  }
};

}

class TestGenericTypes : public QObject
{
  Q_OBJECT

private Q_SLOTS:

  void initTestCase();

  void testGenericClassType();
  void testSequentialContainer_Variant();
  void testAssociativeContainer_Variant();
  void testSequentialContainer_Type();
  void testAssociativeContainer_Type();
  void testSharedPointer();
  void testThirdPartySharedPointer();

}; // class TestGenericTypes

class Person
{
public:
  Person() :age(0) {}
  Person(std::string _name, int _age)
    : name(_name), age(_age)
  {
    static int _uid = 0;
    uid = ++_uid;
  }

  bool operator==(const Person &other) const
  {
    return uid == other.uid;
  }

 std::string name;
 int age;
 int uid;
};

int qHash(const Person &p)
{
  return p.uid;
}

Q_DECLARE_METATYPE(Person)

GRANTLEE_BEGIN_LOOKUP(Person)
  if ( property == QLatin1String( "name" ) )
    return QString::fromStdString( object.name );
  else if ( property == QLatin1String( "age" ) )
    return object.age;
GRANTLEE_END_LOOKUP

class PersonObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(int age READ age)
public:
  PersonObject(const QString &name, int age, QObject* parent = 0)
    : QObject(parent), m_name(name), m_age(age)
  {

  }

  QString name() const { return m_name; }
  int age() const { return m_age; }

private:
  const QString m_name;
  const int m_age; // Yeah, you wish...
};


Q_DECLARE_METATYPE(QList<Person>)
Q_DECLARE_METATYPE(QVector<Person>)
Q_DECLARE_METATYPE(QStack<Person>)
Q_DECLARE_METATYPE(QQueue<Person>)
Q_DECLARE_METATYPE(QLinkedList<Person>)
Q_DECLARE_METATYPE(QSet<Person>)

typedef QHash<QString, Person> PersonHash;
typedef QMap<QString, Person> PersonMap;
Q_DECLARE_METATYPE(PersonHash)
Q_DECLARE_METATYPE(PersonMap)

typedef QHash<qint16, Person> PersonInt16Hash;
typedef QMap<qint16, Person> PersonInt16Map;
Q_DECLARE_METATYPE(PersonInt16Hash)
Q_DECLARE_METATYPE(PersonInt16Map)

typedef QHash<qint32, Person> PersonInt32Hash;
typedef QMap<qint32, Person> PersonInt32Map;
Q_DECLARE_METATYPE(PersonInt32Hash)
Q_DECLARE_METATYPE(PersonInt32Map)

typedef QHash<qint64, Person> PersonInt64Hash;
typedef QMap<qint64, Person> PersonInt64Map;
Q_DECLARE_METATYPE(PersonInt64Hash)
Q_DECLARE_METATYPE(PersonInt64Map)

typedef QHash<quint16, Person> PersonUInt16Hash;
typedef QMap<quint16, Person> PersonUInt16Map;
Q_DECLARE_METATYPE(PersonUInt16Hash)
Q_DECLARE_METATYPE(PersonUInt16Map)

typedef QHash<quint32, Person> PersonUInt32Hash;
typedef QMap<quint32, Person> PersonUInt32Map;
Q_DECLARE_METATYPE(PersonUInt32Hash)
Q_DECLARE_METATYPE(PersonUInt32Map)

typedef QHash<quint64, Person> PersonUInt64Hash;
typedef QMap<quint64, Person> PersonUInt64Map;
Q_DECLARE_METATYPE(PersonUInt64Hash)
Q_DECLARE_METATYPE(PersonUInt64Map)

Q_DECLARE_METATYPE(std::deque<Person>)
Q_DECLARE_METATYPE(std::vector<Person>)
Q_DECLARE_METATYPE(std::list<Person>)

typedef std::map<QString, Person> PersonStdMap;
Q_DECLARE_METATYPE(PersonStdMap)

typedef std::map<qint16, Person> PersonInt16StdMap;
Q_DECLARE_METATYPE(PersonInt16StdMap)

typedef std::map<qint32, Person> PersonInt32StdMap;
Q_DECLARE_METATYPE(PersonInt32StdMap)

typedef std::map<qint64, Person> PersonInt64StdMap;
Q_DECLARE_METATYPE(PersonInt64StdMap)

typedef std::map<quint16, Person> PersonUInt16StdMap;
Q_DECLARE_METATYPE(PersonUInt16StdMap)

typedef std::map<quint32, Person> PersonUInt32StdMap;
Q_DECLARE_METATYPE(PersonUInt32StdMap)

typedef std::map<quint64, Person> PersonUInt64StdMap;
Q_DECLARE_METATYPE(PersonUInt64StdMap)

typedef QtUnorderedMap<QString, Person> PersonHashMap;
Q_DECLARE_METATYPE(PersonHashMap)

typedef QtUnorderedMap<qint16, Person> PersonInt16HashMap;
Q_DECLARE_METATYPE(PersonInt16HashMap)

typedef QtUnorderedMap<qint32, Person> PersonInt32HashMap;
Q_DECLARE_METATYPE(PersonInt32HashMap)

typedef QtUnorderedMap<qint64, Person> PersonInt64HashMap;
Q_DECLARE_METATYPE(PersonInt64HashMap)

typedef QtUnorderedMap<quint16, Person> PersonUInt16HashMap;
Q_DECLARE_METATYPE(PersonUInt16HashMap)

typedef QtUnorderedMap<quint32, Person> PersonUInt32HashMap;
Q_DECLARE_METATYPE(PersonUInt32HashMap)

typedef QtUnorderedMap<quint64, Person> PersonUInt64HashMap;
Q_DECLARE_METATYPE(PersonUInt64HashMap)

Q_DECLARE_METATYPE(ThreeArray<Person>)

void TestGenericTypes::initTestCase()
{
  // Register the handler for our custom type
  Grantlee::registerMetaType<Person>();
  GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF(QtUnorderedMap, Person)
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF(ThreeArray, Person)
  Grantlee::registerMetaType<QSharedPointer<PersonObject> >();
  Grantlee::registerMetaType<std::tr1::shared_ptr<PersonObject> >();
}

void TestGenericTypes::testGenericClassType()
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Template t1 = engine.newTemplate(
      QLatin1String( "Person: \nName: {{p.name}}\nAge: {{p.age}}\nUnknown: {{p.unknown}}" ),
      QLatin1String( "template1" ) );

  // Check it
  QVariantHash h;
  Person p( "Grant Lee", 2);
  h.insert( QLatin1String( "p" ), QVariant::fromValue(p) );
  Grantlee::Context c( h );
  QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Person: \nName: Grant Lee\nAge: 2\nUnknown: " ));

}

static QMap<int, Person> getPeople()
{
  QMap<int, Person> people;
  people.insert( 23, Person( "Claire", 23 ) );
  people.insert( 32, Person( "Grant", 32) );
  people.insert( 50, Person( "Alan", 50 ) );
  return people;
}

template<typename SequentialContainer>
void insertPeopleVariants(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  SequentialContainer container;
  for ( ; it != end; ++it )
    container.push_back( QVariant::fromValue( it.value() ) );
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<typename AssociativeContainer>
void insertAssociatedPeopleVariants(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  AssociativeContainer container;
  for ( ; it != end; ++it )
    container.insert( QString::number( it.key() ), QVariant::fromValue( it.value() ) );
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<>
void insertPeopleVariants<QMap<QString, QVariant> >(Grantlee::Context &c)
{
  insertAssociatedPeopleVariants<QMap<QString, QVariant> >(c);
}

template<>
void insertPeopleVariants<QHash<QString, QVariant> >(Grantlee::Context &c)
{
  insertAssociatedPeopleVariants<QHash<QString, QVariant> >(c);
}

template<typename Container>
void testSequentialIteration(Grantlee::Context c)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for person in people %}{{ person.name }},{% endfor %}" ),
        QLatin1String( "people_template" ) );
    QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Claire,Grant,Alan," ));
  }
}

template<typename Container>
void testSequentialIndexing(Grantlee::Context c)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{{ people.0.name }},{{ people.1.name }},{{ people.2.name }}," ),
        QLatin1String( "people_template" ) );
    QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Claire,Grant,Alan," ));
  }
}

template<>
void testSequentialIndexing<QLinkedList<QVariant> >(Grantlee::Context)
{
  // No op
}

template<typename Container>
void doTestSequentialContainer_Variant()
{
  Grantlee::Context c;

  insertPeopleVariants<Container>(c);

  testSequentialIteration<Container>(c);
  testSequentialIndexing<Container>(c);
}

template<typename Container>
void testAssociativeValues(Grantlee::Context c, bool unordered = false)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for person in people.values %}({{ person.name }}:{{ person.age }}),{% endfor %}" ),
        QLatin1String( "people_template" ) );

    QString result = t1->render( &c );
    if (!unordered)
      QCOMPARE( result, QLatin1String( "(Claire:23),(Grant:32),(Alan:50)," ) );
    else {
      QVERIFY(result.size() == 33);
      QVERIFY(result.contains(QLatin1String( "(Claire:23),")));
      QVERIFY(result.contains(QLatin1String( "(Grant:32),")));
      QVERIFY(result.contains(QLatin1String( "(Alan:50),")));
    }
  }
}

template<typename Container>
void testAssociativeItems(Grantlee::Context c, bool unordered )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for item in people.items %}({{ item.1.name }}:{{ item.1.age }}),{% endfor %}" ),
        QLatin1String( "people_template" ) );
    QString result = t1->render( &c );
    if (!unordered)
      QCOMPARE( result, QLatin1String( "(Claire:23),(Grant:32),(Alan:50)," ) );
    else {
      QVERIFY(result.size() == 33);
      QVERIFY(result.contains(QLatin1String( "(Claire:23),")));
      QVERIFY(result.contains(QLatin1String( "(Grant:32),")));
      QVERIFY(result.contains(QLatin1String( "(Alan:50),")));
    }
  }
}

template<typename Container>
void doTestAssociativeContainer_Variant( bool unordered = false)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertPeopleVariants<Container>(c);
  testAssociativeValues<Container>(c, unordered);
  testAssociativeItems<Container>(c, unordered);
}

void TestGenericTypes::testSequentialContainer_Variant()
{
  doTestSequentialContainer_Variant<QVariantList>();
  doTestSequentialContainer_Variant<QVector<QVariant> >();
  doTestSequentialContainer_Variant<QStack<QVariant> >();
  doTestSequentialContainer_Variant<QQueue<QVariant> >();
  doTestSequentialContainer_Variant<QLinkedList<QVariant> >();
}

void TestGenericTypes::testAssociativeContainer_Variant()
{
  doTestAssociativeContainer_Variant<QVariantMap>();
  doTestAssociativeContainer_Variant<QVariantHash>(true);
}

template<typename SequentialContainer>
void insertPeople(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  SequentialContainer container;
  for ( ; it != end; ++it )
    container.insert( container.end(), it.value() );
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<>
void insertPeople<QSet<Person> >(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  QSet<Person> container;
  for ( ; it != end; ++it )
    container.insert( it.value() );
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<>
void insertPeople<ThreeArray<Person> >(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  ThreeArray<Person> container;
  for ( int i = 0; i < 3; ++i, ++it )
    container[i] = it.value();
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<typename AssociativeContainer>
void insertAssociatedPeople(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  AssociativeContainer container;
  for ( ; it != end; ++it )
    container[QString::number( it.key() )] = it.value();
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<typename AssociativeContainer>
void insertAssociatedPeople_Number(Grantlee::Context &c)
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  AssociativeContainer container;
  for ( ; it != end; ++it )
    container[it.key()] = it.value();
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

template<>
void testSequentialIndexing<QLinkedList<Person> >(Grantlee::Context)
{
  // No op
}

template<>
void testSequentialIndexing<QSet<Person> >(Grantlee::Context)
{
  // No op
}

template<>
void testSequentialIndexing<std::list<Person> >(Grantlee::Context)
{
  // No op
}

template<typename Container>
void doTestSequentialContainer_Type()
{
  Grantlee::Context c;

  insertPeople<Container>(c);

  testSequentialIteration<Container>(c);
  testSequentialIndexing<Container>(c);
}

template<typename Container>
void doTestAssociativeContainer_Type(bool unordered = false)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertAssociatedPeople<Container>(c);
  testAssociativeValues<Container>(c, unordered);
  testAssociativeItems<Container>(c, unordered);
}

template<typename Container>
void doTestAssociativeContainer_Type_Number(bool unordered = false)
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertAssociatedPeople_Number<Container>(c);
  testAssociativeValues<Container>(c, unordered);
  testAssociativeItems<Container>(c, unordered);
}

void TestGenericTypes::testSequentialContainer_Type()
{
  doTestSequentialContainer_Type<QList<Person> >();
  doTestSequentialContainer_Type<QVector<Person> >();
  doTestSequentialContainer_Type<QStack<Person> >();
  doTestSequentialContainer_Type<QQueue<Person> >();
  doTestSequentialContainer_Type<QLinkedList<Person> >();
  doTestSequentialContainer_Type<QSet<Person> >();
  doTestSequentialContainer_Type<std::deque<Person> >();
  doTestSequentialContainer_Type<std::vector<Person> >();
  doTestSequentialContainer_Type<std::list<Person> >();
  doTestSequentialContainer_Type<ThreeArray<Person> >();
}

void TestGenericTypes::testAssociativeContainer_Type()
{
  doTestAssociativeContainer_Type<QMap<QString, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<qint16, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<qint32, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<qint64, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<quint16, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<quint32, Person> >();
  doTestAssociativeContainer_Type_Number<QMap<quint64, Person> >();
  doTestAssociativeContainer_Type<QHash<QString, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<qint16, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<qint32, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<qint64, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<quint16, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<quint32, Person> >(true);
  doTestAssociativeContainer_Type_Number<QHash<quint64, Person> >(true);

  doTestAssociativeContainer_Type<std::map<QString, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint16, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint32, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint64, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint16, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint32, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint64, Person> >();

  doTestAssociativeContainer_Type<QtUnorderedMap<QString, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint16, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint32, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint64, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint16, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint32, Person> >(true);
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint64, Person> >(true);
}

Q_DECLARE_METATYPE(QSharedPointer<PersonObject>)
Q_DECLARE_METATYPE(std::tr1::shared_ptr<PersonObject>)

void TestGenericTypes::testSharedPointer()
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Template t1 = engine.newTemplate(
      QLatin1String( "{{ p.name }} {{ p.age }}" ),
      QLatin1String( "template1" ) );

  // Check it
  QVariantHash h;
  QSharedPointer<PersonObject> p( new PersonObject( QLatin1String( "Grant Lee" ), 2 ) );
  h.insert( QLatin1String( "p" ), QVariant::fromValue( p ) );
  Grantlee::Context c( h );
  QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Grant Lee 2" ));

}

void TestGenericTypes::testThirdPartySharedPointer()
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Template t1 = engine.newTemplate(
      QLatin1String( "{{ p.name }} {{ p.age }}" ),
      QLatin1String( "template1" ) );

  // Check it
  QVariantHash h;
  std::tr1::shared_ptr<PersonObject> p( new PersonObject( QLatin1String( "Grant Lee" ), 2 ) );
  h.insert( QLatin1String( "p" ), QVariant::fromValue( p ) );
  Grantlee::Context c( h );
  QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Grant Lee 2" ));

}

QTEST_MAIN( TestGenericTypes )
#include "testgenerictypes.moc"

