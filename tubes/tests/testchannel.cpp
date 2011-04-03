/*
  This file is part of the Grantlee template system.

  Copyright (c) 2011 Stephen Kelly <steveire@gmail.com>

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

#include "channel.h"
#include <qtcpsocket.h>

using namespace Grantlee;

class TestChannel : public QObject
{
  Q_OBJECT
public:
  explicit TestChannel(QObject* parent = 0)
    : QObject(parent)
  {
  }

private slots:
  void testStrings();

private:
};

void TestChannel::testStrings()
{
  Grantlee::Channel *channel = new Channel(this);
  channel->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

  QByteArray ba("one two");

  QSignalSpy spy(channel, SIGNAL(readyRead()));
  channel->write(ba);
  QVERIFY(spy.size() == 1);

  QVERIFY(channel->size() == 7);
  QVERIFY(channel->peek(3) == "one");
  QVERIFY(channel->size() == 7);
  QVERIFY(channel->read(2) == "on");
  QVERIFY(channel->write(" three") == 6);
  QVERIFY(channel->readAll() == "e two three");
}

QTEST_MAIN( TestChannel )
#include "testchannel.moc"
