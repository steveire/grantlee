/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Michael Jansen <kde@michael-jansen.biz>

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

#include <QtTest/QtTest>
#include <QtCore/QVariant>
#include <QtCore/QVariantHash>
#include <QtCore/QMetaType>

#include <string>


class TestGenericTypes : public QObject
{
  Q_OBJECT

private Q_SLOTS:

  void initTestCase();

  void testGenericClassType();

}; // class TestGenericTypes

class Person
{
public:
  Person() :age(0) {}
  Person(std::string _name, int _age)
    : name(_name), age(_age)
  {

  }

 std::string name;
 int age;
};

Q_DECLARE_METATYPE(Person)

GRANTLEE_BEGIN_LOOKUP(Person)
  if ( property == QLatin1String( "name" ) )
    return QString::fromStdString( object.name );
  else if ( property == QLatin1String( "age" ) )
    return object.age;
GRANTLEE_END_LOOKUP

void TestGenericTypes::initTestCase()
{
  // Register the handler for our custom type
  Grantlee::registerMetaType<Person>();
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

QTEST_MAIN( TestGenericTypes )
#include "testgenerictypes.moc"

