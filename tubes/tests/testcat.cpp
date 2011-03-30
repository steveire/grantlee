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

#include "cat.h"

using namespace Grantlee;

class TestCat : public QObject
{
  Q_OBJECT
public:
  explicit TestCat(QObject* parent = 0)
    : QObject(parent)
  {
  }

private slots:
  void testStrings();

private:
};

void TestCat::testStrings()
{

  Grantlee::Cat *cat = new Cat(this);

  QBuffer buffer1;
  buffer1.open(QIODevice::ReadWrite | QIODevice::Text );
  buffer1.write("one two ");
  buffer1.seek(0);

  QBuffer buffer2;
  buffer2.open(QIODevice::ReadWrite | QIODevice::Text );
  buffer2.write("three four ");
  buffer2.seek(0);

  cat->appendSource(&buffer1);
  cat->appendSource(&buffer2);
  QBuffer resultBuffer;
  resultBuffer.open(QIODevice::ReadWrite | QIODevice::Text );

  cat->setTarget(&resultBuffer);
  buffer1.close();
  buffer2.close();

  QCOMPARE(resultBuffer.data(), QByteArray("one two three four "));

}

QTEST_MAIN( TestCat )
#include "testcat.moc"

