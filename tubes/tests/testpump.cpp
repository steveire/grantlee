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

#include <QtTest/QtTest>

#include "pump.h"

using namespace Grantlee;

class TestPump : public QObject
{
  Q_OBJECT
public:
  explicit TestPump(QObject* parent = 0)
    : QObject(parent)
  {
  }

private slots:
  void testStrings();

private:
};

void TestPump::testStrings()
{
  Grantlee::Pump *pump = new Pump(this);
  QBuffer outputBuffer;
  outputBuffer.open(QIODevice::ReadWrite | QIODevice::Text );

  pump->setTarget(&outputBuffer);

  QByteArray ba("one two");
  QBuffer inputBuffer(&ba);
  inputBuffer.open(QIODevice::ReadOnly | QIODevice::Text );

  pump->setSource(&inputBuffer);

  QCOMPARE(outputBuffer.data(), inputBuffer.data());
}

QTEST_MAIN( TestPump )
#include "testpump.moc"
