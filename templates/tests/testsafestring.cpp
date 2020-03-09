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

#ifndef SAFESTRINGTEST_H
#define SAFESTRINGTEST_H

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "coverageobject.h"
#include "outputstream.h"
#include "safestring.h"

using namespace Grantlee;

class TestSafeString : public CoverageObject
{
  Q_OBJECT
private Q_SLOTS:
  void testCombining();
  void testAppending();
  void testChopping();
  void testReplacing();
};

void TestSafeString::testCombining()
{
  auto string1 = QStringLiteral("this & that");
  auto string2 = QStringLiteral(" over & under");
  SafeString safeString1unsafe(string1);
  SafeString safeString2unsafe(string2);
  SafeString safeString1safe(string1, SafeString::IsSafe);
  SafeString safeString2safe(string2, SafeString::IsSafe);

  SafeString combined;
  combined = safeString1safe + safeString2safe;
  QVERIFY(combined.isSafe());
  combined = safeString1safe + safeString1unsafe;
  QVERIFY(!combined.isSafe());
  combined = safeString1safe + string1;
  QVERIFY(!combined.isSafe());
  combined = safeString1unsafe + safeString1safe;
  QVERIFY(!combined.isSafe());
  combined = safeString1unsafe + safeString2unsafe;
  QVERIFY(!combined.isSafe());
  combined = safeString1unsafe + string1;
  QVERIFY(!combined.isSafe());
}

void TestSafeString::testAppending()
{
  auto string1 = QStringLiteral("this & that");
  SafeString safeString1unsafe(string1);
  SafeString safeString1safe(string1, SafeString::IsSafe);
  SafeString safeString2safe(string1, SafeString::IsSafe);

  SafeString result;
  result = safeString1safe.get().append(safeString1safe);
  QVERIFY(safeString1safe.isSafe());
  QVERIFY(result.isSafe());
  QCOMPARE(result.get(), safeString1safe.get());
  result = safeString1safe.get().append(string1);
  QVERIFY(!safeString1safe.isSafe());
  QVERIFY(!result.isSafe());
  QCOMPARE(result.get(), safeString1safe.get());
  result = safeString1unsafe.get().append(string1);
  QVERIFY(!safeString1unsafe.isSafe());
  QVERIFY(!result.isSafe());
  QCOMPARE(result.get(), safeString1unsafe.get());
}

void TestSafeString::testChopping()
{
  auto string1 = QStringLiteral("this & that");
  SafeString safeString1unsafe(string1);
  SafeString safeString1safe(string1, SafeString::IsSafe);

  safeString1safe.get().chop(4);
  QVERIFY(!safeString1safe.isSafe());
  QVERIFY(QStringLiteral("this & ") == safeString1safe.get());
  safeString1unsafe.get().chop(4);
  QVERIFY(!safeString1unsafe.isSafe());
  QVERIFY(QStringLiteral("this & ") == safeString1unsafe.get());
}

void TestSafeString::testReplacing()
{
  auto string1 = QStringLiteral("x&\ny");
  SafeString safeString1safe(string1, SafeString::IsSafe);

  auto result = safeString1safe.get().replace(QLatin1Char('\n'),
                                              QStringLiteral("<br />"));
  QVERIFY(!result.isSafe());
  QVERIFY(!safeString1safe.isSafe());
  QCOMPARE(result.get(), safeString1safe.get());
}

QTEST_MAIN(TestSafeString)
#include "testsafestring.moc"

#endif
