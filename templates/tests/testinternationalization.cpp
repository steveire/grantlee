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

#include "qtlocalizer.h"
#include "nulllocalizer_p.h"

#include <QtCore/QScopedPointer>
#include <QtTest/QtTest>

using namespace Grantlee;

class TestInternationalization : public QObject
{
  Q_OBJECT
public:
  explicit TestInternationalization(QObject* parent = 0)
    : QObject(parent),
      cLocalizer(new QtLocalizer(QLocale::c())),
      nullLocalizer(new NullLocalizer()),
      deLocalizer(new QtLocalizer(QLocale(QLocale::German, QLocale::Germany))),
      frLocalizer(new QtLocalizer(QLocale(QLocale::French, QLocale::France))),
      en_GBLocalizer(new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedKingdom))),
      en_USLocalizer(new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedStates)))
  {

  }

private slots:
  void testStrings();
  void testIntegers();
  void testFloats();
  void testDates();
  void testTimes();
  void testDateTimes();

  void testStrings_data();
  void testIntegers_data();
  void testFloats_data();
  void testDates_data();
  void testTimes_data();
  void testDateTimes_data();

private:
  const QSharedPointer<AbstractLocalizer> cLocalizer;
  const QSharedPointer<AbstractLocalizer> nullLocalizer;
  const QSharedPointer<AbstractLocalizer> deLocalizer;
  const QSharedPointer<AbstractLocalizer> frLocalizer;
  const QSharedPointer<AbstractLocalizer> en_GBLocalizer;
  const QSharedPointer<AbstractLocalizer> en_USLocalizer;
};

void TestInternationalization::testStrings()
{

}

void TestInternationalization::testStrings_data()
{
}

void TestInternationalization::testDates()
{
  QFETCH(QDate, date);
  QFETCH(QString, nullDate);
  QFETCH(QString, cDate);
  QFETCH(QString, en_USDate);
  QFETCH(QString, en_GBDate);
  QFETCH(QString, deDate);
  QFETCH(QString, frDate);

  QCOMPARE(nullLocalizer->localizeDate(date), nullDate);
  QCOMPARE(cLocalizer->localizeDate(date), cDate);
  QCOMPARE(en_USLocalizer->localizeDate(date), en_USDate);
  QCOMPARE(en_GBLocalizer->localizeDate(date), en_GBDate);
  QCOMPARE(deLocalizer->localizeDate(date), deDate);
  QCOMPARE(frLocalizer->localizeDate(date), frDate);
}

void TestInternationalization::testDates_data()
{
  QTest::addColumn<QDate>("date");
  QTest::addColumn<QString>("nullDate");
  QTest::addColumn<QString>("cDate");
  QTest::addColumn<QString>("en_USDate");
  QTest::addColumn<QString>("en_GBDate");
  QTest::addColumn<QString>("deDate");
  QTest::addColumn<QString>("frDate");

  QTest::newRow("date-01")
    << QDate(2010, 5, 9)
    << QString::fromLatin1("Sun May 9 2010")
    << QString::fromLatin1("9 May 2010")
    << QString::fromLatin1("5/9/10")
    << QString::fromLatin1("09/05/2010")
    << QString::fromLatin1("09.05.10")
    << QString::fromLatin1("09/05/10");

  QTest::newRow("date-02")
    << QDate(2010, 10, 11)
    << QString::fromLatin1("Mon Oct 11 2010")
    << QString::fromLatin1("11 Oct 2010")
    << QString::fromLatin1("10/11/10")
    << QString::fromLatin1("11/10/2010")
    << QString::fromLatin1("11.10.10")
    << QString::fromLatin1("11/10/10");
}

void TestInternationalization::testIntegers()
{
  QFETCH(int, integer);
  QFETCH(QString, nullInteger);
  QFETCH(QString, cInteger);
  QFETCH(QString, en_USInteger);
  QFETCH(QString, en_GBInteger);
  QFETCH(QString, deInteger);
  QFETCH(QString, frInteger);

  QCOMPARE(nullLocalizer->localizeNumber(integer), nullInteger);
  QCOMPARE(cLocalizer->localizeNumber(integer), cInteger);
  QCOMPARE(en_USLocalizer->localizeNumber(integer), en_USInteger);
  QCOMPARE(en_GBLocalizer->localizeNumber(integer), en_GBInteger);
  QCOMPARE(deLocalizer->localizeNumber(integer), deInteger);
  QCOMPARE(frLocalizer->localizeNumber(integer), frInteger);
}

void TestInternationalization::testIntegers_data()
{
  QTest::addColumn<int>("integer");
  QTest::addColumn<QString>("nullInteger");
  QTest::addColumn<QString>("cInteger");
  QTest::addColumn<QString>("en_USInteger");
  QTest::addColumn<QString>("en_GBInteger");
  QTest::addColumn<QString>("deInteger");
  QTest::addColumn<QString>("frInteger");

  QTest::newRow("integer-01")
    << 7
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7");

  QTest::newRow("integer-02")
    << 7000
    << QString::fromLatin1("7000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7.000")
    << QString::fromUtf8("7 000");
}

void TestInternationalization::testFloats()
{
  QFETCH(double, floatingPoint);
  QFETCH(QString, nullFloatingPoint);
  QFETCH(QString, cFloatingPoint);
  QFETCH(QString, en_USFloatingPoint);
  QFETCH(QString, en_GBFloatingPoint);
  QFETCH(QString, deFloatingPoint);
  QFETCH(QString, frFloatingPoint);

  QCOMPARE(nullLocalizer->localizeNumber(floatingPoint), nullFloatingPoint);
  QCOMPARE(cLocalizer->localizeNumber(floatingPoint), cFloatingPoint);
  QCOMPARE(en_USLocalizer->localizeNumber(floatingPoint), en_USFloatingPoint);
  QCOMPARE(en_GBLocalizer->localizeNumber(floatingPoint), en_GBFloatingPoint);
  QCOMPARE(deLocalizer->localizeNumber(floatingPoint), deFloatingPoint);
  QCOMPARE(frLocalizer->localizeNumber(floatingPoint), frFloatingPoint);
}

void TestInternationalization::testFloats_data()
{
  QTest::addColumn<double>("floatingPoint");
  QTest::addColumn<QString>("nullFloatingPoint");
  QTest::addColumn<QString>("cFloatingPoint");
  QTest::addColumn<QString>("en_USFloatingPoint");
  QTest::addColumn<QString>("en_GBFloatingPoint");
  QTest::addColumn<QString>("deFloatingPoint");
  QTest::addColumn<QString>("frFloatingPoint");

  QTest::newRow("float-01")
    << 7.4
    << QString::fromLatin1("7.4")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7,40")
    << QString::fromLatin1("7,40");
}

void TestInternationalization::testTimes()
{
  QFETCH(QTime, time);
  QFETCH(QString, nullTime);
  QFETCH(QString, cTime);
  QFETCH(QString, en_USTime);
  QFETCH(QString, en_GBTime);
  QFETCH(QString, deTime);
  QFETCH(QString, frTime);

  QCOMPARE(nullLocalizer->localizeTime(time), nullTime);
  QCOMPARE(cLocalizer->localizeTime(time), cTime);
  QCOMPARE(en_USLocalizer->localizeTime(time), en_USTime);
  QCOMPARE(en_GBLocalizer->localizeTime(time), en_GBTime);
  QCOMPARE(deLocalizer->localizeTime(time), deTime);
  QCOMPARE(frLocalizer->localizeTime(time), frTime);
}

void TestInternationalization::testTimes_data()
{
  QTest::addColumn<QTime>("time");
  QTest::addColumn<QString>("nullTime");
  QTest::addColumn<QString>("cTime");
  QTest::addColumn<QString>("en_USTime");
  QTest::addColumn<QString>("en_GBTime");
  QTest::addColumn<QString>("deTime");
  QTest::addColumn<QString>("frTime");

  QTest::newRow("time-01")
    << QTime(5, 6, 7)
    << QString::fromLatin1("05:06:07")
    << QString::fromLatin1("05:06:07")
    << QString::fromLatin1("5:06 AM")
    << QString::fromLatin1("05:06")
    << QString::fromLatin1("05:06")
    << QString::fromLatin1("05:06");

  QTest::newRow("time-02")
    << QTime(11, 12, 13)
    << QString::fromLatin1("11:12:13")
    << QString::fromLatin1("11:12:13")
    << QString::fromLatin1("11:12 AM")
    << QString::fromLatin1("11:12")
    << QString::fromLatin1("11:12")
    << QString::fromLatin1("11:12");

  QTest::newRow("time-03")
    << QTime(15, 12, 13)
    << QString::fromLatin1("15:12:13")
    << QString::fromLatin1("15:12:13")
    << QString::fromLatin1("3:12 PM")
    << QString::fromLatin1("15:12")
    << QString::fromLatin1("15:12")
    << QString::fromLatin1("15:12");

}

void TestInternationalization::testDateTimes()
{
  QFETCH(QDateTime, dateTime);
  QFETCH(QString, nullDateTime);
  QFETCH(QString, cDateTime);
  QFETCH(QString, en_USDateTime);
  QFETCH(QString, en_GBDateTime);
  QFETCH(QString, deDateTime);
  QFETCH(QString, frDateTime);

  QCOMPARE(nullLocalizer->localizeDateTime(dateTime), nullDateTime);
  QCOMPARE(cLocalizer->localizeDateTime(dateTime), cDateTime);
  QCOMPARE(en_USLocalizer->localizeDateTime(dateTime), en_USDateTime);
  QCOMPARE(en_GBLocalizer->localizeDateTime(dateTime), en_GBDateTime);
  QCOMPARE(deLocalizer->localizeDateTime(dateTime), deDateTime);
  QCOMPARE(frLocalizer->localizeDateTime(dateTime), frDateTime);
}

void TestInternationalization::testDateTimes_data()
{
  QTest::addColumn<QDateTime>("dateTime");
  QTest::addColumn<QString>("nullDateTime");
  QTest::addColumn<QString>("cDateTime");
  QTest::addColumn<QString>("en_USDateTime");
  QTest::addColumn<QString>("en_GBDateTime");
  QTest::addColumn<QString>("deDateTime");
  QTest::addColumn<QString>("frDateTime");

  QTest::newRow("datetime-01")
    << QDateTime(QDate(2005, 6, 7), QTime(5, 6, 7))
    << QString::fromLatin1("Tue Jun 7 05:06:07 2005")
    << QString::fromLatin1("7 Jun 2005 05:06:07")
    << QString::fromLatin1("6/7/05 5:06 AM")
    << QString::fromLatin1("07/06/2005 05:06")
    << QString::fromLatin1("07.06.05 05:06")
    << QString::fromLatin1("07/06/05 05:06");

  QTest::newRow("datetime-02")
    << QDateTime(QDate(2005, 6, 7), QTime(11, 12, 13))
    << QString::fromLatin1("Tue Jun 7 11:12:13 2005")
    << QString::fromLatin1("7 Jun 2005 11:12:13")
    << QString::fromLatin1("6/7/05 11:12 AM")
    << QString::fromLatin1("07/06/2005 11:12")
    << QString::fromLatin1("07.06.05 11:12")
    << QString::fromLatin1("07/06/05 11:12");

  QTest::newRow("datetime-03")
    << QDateTime(QDate(2005, 6, 7), QTime(15, 12, 13))
    << QString::fromLatin1("Tue Jun 7 15:12:13 2005")
    << QString::fromLatin1("7 Jun 2005 15:12:13")
    << QString::fromLatin1("6/7/05 3:12 PM")
    << QString::fromLatin1("07/06/2005 15:12")
    << QString::fromLatin1("07.06.05 15:12")
    << QString::fromLatin1("07/06/05 15:12");

  QTest::newRow("datetime-04")
    << QDateTime(QDate(2005, 10, 11), QTime(5, 6, 7))
    << QString::fromLatin1("Tue Oct 11 05:06:07 2005")
    << QString::fromLatin1("11 Oct 2005 05:06:07")
    << QString::fromLatin1("10/11/05 5:06 AM")
    << QString::fromLatin1("11/10/2005 05:06")
    << QString::fromLatin1("11.10.05 05:06")
    << QString::fromLatin1("11/10/05 05:06");

  QTest::newRow("datetime-05")
    << QDateTime(QDate(2005, 10, 11), QTime(11, 12, 13))
    << QString::fromLatin1("Tue Oct 11 11:12:13 2005")
    << QString::fromLatin1("11 Oct 2005 11:12:13")
    << QString::fromLatin1("10/11/05 11:12 AM")
    << QString::fromLatin1("11/10/2005 11:12")
    << QString::fromLatin1("11.10.05 11:12")
    << QString::fromLatin1("11/10/05 11:12");

  QTest::newRow("datetime-06")
    << QDateTime(QDate(2005, 10, 11), QTime(15, 12, 13))
    << QString::fromLatin1("Tue Oct 11 15:12:13 2005")
    << QString::fromLatin1("11 Oct 2005 15:12:13")
    << QString::fromLatin1("10/11/05 3:12 PM")
    << QString::fromLatin1("11/10/2005 15:12")
    << QString::fromLatin1("11.10.05 15:12")
    << QString::fromLatin1("11/10/05 15:12");

}


QTEST_MAIN( TestInternationalization )
#include "testinternationalization.moc"
