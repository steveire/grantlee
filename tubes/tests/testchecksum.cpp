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

#include "checksum.h"

using namespace Grantlee;

class TestChecksum : public QObject
{
  Q_OBJECT
public:
  explicit TestChecksum(QObject* parent = 0)
    : QObject(parent)
  {
  }

private slots:
  void testStrings();

private:
};

void TestChecksum::testStrings()
{

  Grantlee::Checksum *checksum = new Checksum(Checksum::Sha1Sum, this);
  QBuffer buffer1;
  buffer1.open(QIODevice::WriteOnly | QIODevice::Text );
  checksum->setTarget(&buffer1);

  QByteArray ba("one two");

  checksum->open(QIODevice::WriteOnly | QIODevice::Text );

  checksum->write(ba);
  checksum->close();

  QCOMPARE(buffer1.data(), QByteArray("a6151e9585c47b1d3e4c4eb25e8200a81de14b7c"));

}

QTEST_MAIN( TestChecksum )
#include "testchecksum.moc"

