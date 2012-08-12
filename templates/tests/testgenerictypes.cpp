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
#include "test_macros.h"

#include <QtTest/QTest>
#include <QtCore/QVariant>
#include <QtCore/QVariantHash>
#include <QtCore/QMetaType>

#include <string>
#include "coverageobject.h"

#ifndef GRANTLEE_NO_TR1

#ifdef Q_CC_MSVC
#include <memory>
#else
#include <tr1/memory>
#endif

GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER        (ThreeArray)
GRANTLEE_SEQUENTIAL_TYPE_CONTAINER_ACCESSOR   (ThreeArray)

GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER       (QtUnorderedMap)
GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR  (QtUnorderedMap)

GRANTLEE_SMART_PTR_ACCESSOR(std::tr1::shared_ptr)

#endif

Q_DECLARE_METATYPE( QVector<QVariant> )
Q_DECLARE_METATYPE( QStack<QVariant> )
Q_DECLARE_METATYPE( QQueue<QVariant> )
Q_DECLARE_METATYPE( QLinkedList<QVariant> )

class TestGenericTypes : public CoverageObject
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
  void testNestedContainers();

  void testCustomQObjectDerived();

  void testUnregistered();
  void testPointerNonQObject();
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

int qHash( const Person &p )
{
  return p.uid;
}

Q_DECLARE_METATYPE( Person )

GRANTLEE_BEGIN_LOOKUP(Person)
  if ( property == QLatin1String( "name" ) )
    return QString::fromStdString( object.name );
  else if ( property == QLatin1String( "age" ) )
    return object.age;
GRANTLEE_END_LOOKUP

class PersonObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY( QString name READ name )
  Q_PROPERTY( int age READ age )
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

DECLARE_TYPE_CONTAINERS( Person )

void TestGenericTypes::initTestCase()
{
  // Register the handler for our custom type
  Grantlee::registerMetaType<Person>();
#ifndef GRANTLEE_NO_TR1
  GRANTLEE_REGISTER_ASSOCIATIVE_CONTAINER_IF( QtUnorderedMap, Person )
  GRANTLEE_REGISTER_SEQUENTIAL_CONTAINER_IF( ThreeArray, Person )
  Grantlee::registerMetaType<std::tr1::shared_ptr<PersonObject> >();
#endif
  Grantlee::registerMetaType<QSharedPointer<PersonObject> >();
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
  Person p( "Grant Lee", 2 );
  h.insert( QLatin1String( "p" ), QVariant::fromValue( p ) );
  Grantlee::Context c( h );
  QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Person: \nName: Grant Lee\nAge: 2\nUnknown: " ));

}

static QMap<int, Person> getPeople()
{
  QMap<int, Person> people;
  people.insert( 23, Person( "Claire", 23 ) );
  people.insert( 32, Person( "Grant", 32 ) );
  people.insert( 50, Person( "Alan", 50 ) );
  return people;
}

template<typename SequentialContainer>
void insertPeopleVariants( Grantlee::Context &c )
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
void insertAssociatedPeopleVariants( Grantlee::Context &c )
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
void insertPeopleVariants<QMap<QString, QVariant> >( Grantlee::Context &c )
{
  insertAssociatedPeopleVariants<QMap<QString, QVariant> >( c );
}

template<>
void insertPeopleVariants<QHash<QString, QVariant> >( Grantlee::Context &c )
{
  insertAssociatedPeopleVariants<QHash<QString, QVariant> >( c );
}

template<typename Container>
void testSequentialIteration( Grantlee::Context c )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for person in people %}{{ person.name }},{% endfor %}" ),
        QLatin1String( "people_template" ) );
    QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Claire,Grant,Alan," ) );
  }
}

template<typename Container>
void testSequentialIndexing( Grantlee::Context c )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{{ people.0.name }},{{ people.1.name }},{{ people.2.name }}," ),
        QLatin1String( "people_template" ) );
    QCOMPARE(
      t1->render( &c ),
      QLatin1String( "Claire,Grant,Alan," ) );
  }
}

template<>
void testSequentialIndexing<QLinkedList<QVariant> >( Grantlee::Context )
{
  // No op
}

template<typename Container>
void doTestSequentialContainer_Variant()
{
  Grantlee::Context c;

  insertPeopleVariants<Container>( c );

  testSequentialIteration<Container>( c );
  testSequentialIndexing<Container>( c );
}

template<typename Container>
void testAssociativeValues( Grantlee::Context c, bool unordered = false )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for person in people.values %}({{ person.name }}:{{ person.age }}),{% endfor %}" ),
        QLatin1String( "people_template" ) );

    QString result = t1->render( &c );
    if ( !unordered )
      QCOMPARE( result, QLatin1String( "(Claire:23),(Grant:32),(Alan:50)," ) );
    else {
      QVERIFY( result.size() == 33 );
      QVERIFY( result.contains( QLatin1String( "(Claire:23)," ) ) );
      QVERIFY( result.contains( QLatin1String( "(Grant:32)," ) ) );
      QVERIFY( result.contains( QLatin1String( "(Alan:50)," ) ) );
    }
  }
}

template<typename Container>
void testAssociativeItems( Grantlee::Context c, bool unordered )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{% for item in people.items %}({{ item.1.name }}:{{ item.1.age }}),{% endfor %}" ),
        QLatin1String( "people_template" ) );
    QString result = t1->render( &c );
    if ( !unordered )
      QCOMPARE( result, QLatin1String( "(Claire:23),(Grant:32),(Alan:50)," ) );
    else {
      QVERIFY( result.size() == 33 );
      QVERIFY( result.contains( QLatin1String( "(Claire:23)," ) ) );
      QVERIFY( result.contains( QLatin1String( "(Grant:32)," ) ) );
      QVERIFY( result.contains( QLatin1String( "(Alan:50)," ) ) );
    }
  }
}

template<typename Container>
void doTestAssociativeContainer_Variant( bool unordered = false )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertPeopleVariants<Container>( c );
  testAssociativeValues<Container>( c, unordered );
  testAssociativeItems<Container>( c, unordered );
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
  doTestAssociativeContainer_Variant<QVariantHash>( true );
}

template<typename SequentialContainer>
void insertPeople( Grantlee::Context &c )
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
void insertPeople<QSet<Person> >( Grantlee::Context &c )
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  const QMap<int, Person>::const_iterator end = people.constEnd();
  QSet<Person> container;
  for ( ; it != end; ++it )
    container.insert( it.value() );
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}

#ifndef GRANTLEE_NO_TR1
template<>
void insertPeople<ThreeArray<Person> >( Grantlee::Context &c )
{
  QMap<int, Person> people = getPeople();
  QMap<int, Person>::const_iterator it = people.constBegin();
  ThreeArray<Person> container;
  for ( int i = 0; i < 3; ++i, ++it ) {
    Q_ASSERT( it != people.constEnd() );
    container[i] = it.value();
  }
  c.insert( QLatin1String( "people" ), QVariant::fromValue( container ) );
}
#endif

template<typename AssociativeContainer>
void insertAssociatedPeople( Grantlee::Context &c )
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
void insertAssociatedPeople_Number( Grantlee::Context &c )
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
void testSequentialIndexing<QLinkedList<Person> >( Grantlee::Context )
{
  // No op
}

template<>
void testSequentialIndexing<QSet<Person> >( Grantlee::Context )
{
  // No op
}

template<>
void testSequentialIndexing<std::list<Person> >( Grantlee::Context )
{
  // No op
}

template<typename Container>
void doTestSequentialContainer_Type()
{
  Grantlee::Context c;

  insertPeople<Container>( c );

  testSequentialIteration<Container>( c );
  testSequentialIndexing<Container>( c );
}

template<typename Container>
void doTestAssociativeContainer_Type( bool unordered = false )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertAssociatedPeople<Container>( c );
  testAssociativeValues<Container>( c, unordered );
  testAssociativeItems<Container>( c, unordered );
}

template<typename Container>
void doTestAssociativeContainer_Type_Number( bool unordered = false )
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;

  insertAssociatedPeople_Number<Container>( c );
  testAssociativeValues<Container>( c, unordered );
  testAssociativeItems<Container>( c, unordered );

  {
    Grantlee::Template t1 = engine.newTemplate(
        QLatin1String( "{{ people.23.name }}" ),
        QLatin1String( "claire_template" ) );
    QString result = t1->render( &c );
    QCOMPARE( result, QLatin1String( "Claire" ) );
  }

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
#ifndef GRANTLEE_NO_TR1
  doTestSequentialContainer_Type<ThreeArray<Person> >();
#endif
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
  doTestAssociativeContainer_Type<QHash<QString, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<qint16, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<qint32, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<qint64, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<quint16, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<quint32, Person> >( true );
  doTestAssociativeContainer_Type_Number<QHash<quint64, Person> >( true );

  doTestAssociativeContainer_Type<std::map<QString, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint16, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint32, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<qint64, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint16, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint32, Person> >();
  doTestAssociativeContainer_Type_Number<std::map<quint64, Person> >();

#ifndef GRANTLEE_NO_TR1
  doTestAssociativeContainer_Type<QtUnorderedMap<QString, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint16, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint32, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<qint64, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint16, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint32, Person> >( true );
  doTestAssociativeContainer_Type_Number<QtUnorderedMap<quint64, Person> >( true );
#endif
}

Q_DECLARE_METATYPE( QSharedPointer<PersonObject> )
#ifndef GRANTLEE_NO_TR1
Q_DECLARE_METATYPE( std::tr1::shared_ptr<PersonObject> )
#endif

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
#ifndef GRANTLEE_NO_TR1
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
#endif
}

Q_DECLARE_METATYPE( QVector<qint16> )
typedef QList<QVector<qint16> > ListVectorInt;
Q_DECLARE_METATYPE( ListVectorInt )
typedef QMap<int, QList<QVector<qint16> > > MapListVectorInt;
Q_DECLARE_METATYPE( MapListVectorInt )
typedef QStack<QMap<int, QList<QVector<qint16> > > > StackMapListVectorInt;
Q_DECLARE_METATYPE( StackMapListVectorInt )

static QVector<qint16> getNumbers()
{
  static int n = 0;
  QVector<qint16> nums;
  nums.push_back( ++n );
  nums.push_back( ++n );
  return nums;
}

static ListVectorInt getNumberLists()
{
  ListVectorInt list;
  for ( int i = 0; i < 2; ++i ) {
    list.append( getNumbers() );
  }
  return list;
}

static MapListVectorInt getNumberListMap()
{
  MapListVectorInt map;
  for ( int i = 0; i < 2; ++i ) {
    map.insert( i, getNumberLists() );
  }
  return map;
}

static StackMapListVectorInt getMapStack()
{
  StackMapListVectorInt stack;
  for ( int i = 0; i < 2; ++i ) {
    stack.push( getNumberListMap() );
  }
  return stack;
}

void TestGenericTypes::testNestedContainers()
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  Grantlee::Context c;
  c.insert( QLatin1String( "stack" ), QVariant::fromValue( getMapStack() ) );

  Grantlee::Template t1 = engine.newTemplate(
    QLatin1String(  "{% for map in stack %}"
                      "(M {% for key, list in map.items %}"
                        "({{ key }} : (L {% for vector in list %}"
                          "(V {% for number in vector %}"
                            "{{ number }},"
                          "{% endfor %}),"
                        "{% endfor %}),"
                      "{% endfor %}),"
                    "{% endfor %}" )
    , QLatin1String( "template1" ) );

  QString result = t1->render( &c );

  QString expectedResult = QLatin1String(
    "(M (0 : (L (V 1,2,),(V 3,4,),),(1 : (L (V 5,6,),(V 7,8,),),),(M (0 : (L (V 9,10,),(V 11,12,),),(1 : (L (V 13,14,),(V 15,16,),),),"
  );

  QCOMPARE(result, expectedResult);
}

class CustomObject : public QObject
{
  Q_OBJECT
public:
  explicit CustomObject(QObject* parent = 0)
    : QObject(parent)
  {

  }
};

Q_DECLARE_METATYPE(CustomObject*)

class OtherObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(CustomObject* custom READ custom)
public:
  explicit OtherObject(QObject* parent = 0)
    : QObject(parent), m_custom(new CustomObject(this))
  {
    m_custom->setProperty("nestedProp", QLatin1String("nestedValue"));
  }

  CustomObject* custom()
  {
    return m_custom;
  }

private:
  CustomObject *m_custom;
};

void TestGenericTypes::testCustomQObjectDerived()
{
  Grantlee::Engine engine;

  engine.setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH ) );

  CustomObject *customObject = new CustomObject(this);
  customObject->setProperty("someProp", QLatin1String("propValue"));

  Grantlee::registerMetaType<CustomObject*>();

  Grantlee::Context c;
  c.insert( QLatin1String( "custom" ), QVariant::fromValue( customObject ) );

  {
    Grantlee::Template t1 = engine.newTemplate(
      QLatin1String( "{{ custom.someProp }}"), QLatin1String( "template1" ) );

    QString result = t1->render( &c );
    QString expectedResult = QLatin1String("propValue");

    QCOMPARE(result, expectedResult);
  }

  QObject *other = new OtherObject(this);

  c.insert(QLatin1String("other"), other);

  {
    Grantlee::Template t1 = engine.newTemplate(
      QLatin1String( "{{ other.custom.nestedProp }}"), QLatin1String( "template1" ) );

    QString result = t1->render( &c );
    QString expectedResult = QLatin1String("nestedValue");

    QCOMPARE(result, expectedResult);
  }
}

struct UnregisteredType
{

};

Q_DECLARE_METATYPE(UnregisteredType)

struct RegisteredNotListType
{

};

Q_DECLARE_METATYPE(RegisteredNotListType)

GRANTLEE_BEGIN_LOOKUP(RegisteredNotListType)
  Q_UNUSED( object )
  if ( property == QLatin1String( "property" ) )
    return 42;
GRANTLEE_END_LOOKUP

static QVariantList dummy( const QVariant& )
{
  return QVariantList() << 42;
}

QVariant dummyLookup( const QVariant &, const QString & )
{
  return 42;
}

void TestGenericTypes::testUnregistered()
{

  {
    UnregisteredType unregType;
    QVariant v = QVariant::fromValue( unregType );

    QVariant result = Grantlee::MetaType::lookup( v, QLatin1String( "property" ) );
    QVERIFY( !result.isValid() );

    QVariantList resultList = Grantlee::MetaType::toVariantList( v );
    QVERIFY( resultList.isEmpty() );
  }

  Grantlee::registerMetaType<RegisteredNotListType>();

  {
    RegisteredNotListType nonListType;
    QVariant v = QVariant::fromValue( nonListType );
    QVariant result = Grantlee::MetaType::lookup( v, QLatin1String( "property" ) );
    QVERIFY( result.isValid() );
    QVariantList resultList = Grantlee::MetaType::toVariantList( v );
    QVERIFY( resultList.isEmpty() );
  }

  {
    Grantlee::MetaType::registerToVariantListOperator(qMetaTypeId<UnregisteredType>(), dummy);
    UnregisteredType unregType;
    QVariant v = QVariant::fromValue( unregType );
    QVariant result = Grantlee::MetaType::lookup( v, QLatin1String( "property" ) );
    QVERIFY( !result.isValid() );
  }

  // Only do this in release mode?
//   Grantlee::MetaType::registerLookUpOperator(0, dummyLookup);
//   Grantlee::MetaType::registerToVariantListOperator(0, dummy);
}


Q_DECLARE_METATYPE( Person* )

GRANTLEE_BEGIN_LOOKUP_PTR(Person)
  if ( property == QLatin1String( "name" ) )
    return QString::fromStdString( object->name );
  else if ( property == QLatin1String( "age" ) )
    return object->age;
GRANTLEE_END_LOOKUP

void TestGenericTypes::testPointerNonQObject()
{
  Person *p = new Person( "Adele", 21 );
  QVariant v = QVariant::fromValue(p);

  Grantlee::registerMetaType<Person*>();

  QVariant result = Grantlee::MetaType::lookup( v, QLatin1String( "name" ) );

  QCOMPARE( result.toString(), QLatin1String( "Adele" ) );

  delete p;
}

QTEST_MAIN( TestGenericTypes )
#include "testgenerictypes.moc"
