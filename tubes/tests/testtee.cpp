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

#include "tee.h"

using namespace Grantlee;

class TestTee : public QObject
{
  Q_OBJECT
public:
  explicit TestTee(QObject* parent = 0)
    : QObject(parent)
  {
  }

private slots:
  void testStrings();

private:
};

void TestTee::testStrings()
{
  Grantlee::Tee *tee = new Tee(this);
  QBuffer buffer1;
  buffer1.open(QIODevice::WriteOnly | QIODevice::Text );
  tee->appendTarget(&buffer1);
  QBuffer buffer2;
  buffer2.open(QIODevice::WriteOnly | QIODevice::Text );
  tee->appendTarget(&buffer2);

  QByteArray ba("one two");

  tee->open(QIODevice::WriteOnly);

  tee->write(ba);

  QCOMPARE(buffer1.data(), ba);
  QCOMPARE(buffer2.data(), ba);
}

QTEST_MAIN( TestTee )
#include "testtee.moc"
