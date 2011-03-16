/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010,2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_PUMP_H
#define GRANTLEE_PUMP_H

#include <QtCore/QIODevice>

#include "grantlee_tubes_export.h"

namespace Grantlee
{

class PumpPrivate;

/// @headerfile pump.h grantlee/pump.h

/**
  @brief Pump reads from a source and writes to a target

  Pump reads all data available from a source QIODevice and writes it
  to a target QIODevice, working asynchronously and listening to
  the QIODevice::readyRead signal of the source.

  @code
    QTcpSocket *socket = new QTcpSocket(...);
    socket->open(QIODevice::ReadOnly);

    QFile *dumpFile = new DumpFile("dump.txt", this);
    dumpFile->open(QIODevice::Append);

    // Write all available data from the socket to the file.
    Grantlee::Pump *pump = new Grantlee::Pump(this);
    pump->setSource(socket);
    pump->setTarget(dumpFile);
  @endcode

  As with all Grantlee::Tubes APIs, the responsibility for opening the source and
  target devices lies with the caller.

  @author Stephen Kelly <steveire@gmail.com>
 */
class GRANTLEE_TUBES_EXPORT Pump : public QObject
{
  Q_OBJECT
public:
  Pump(QObject *parent = 0);

  /**
   * Sets the device to read from to @p sourceDevice.
   */
  void setSource(QIODevice *sourceDevice);

  /**
   * Returns the device being read from.
   */
  QIODevice *source() const;

  /**
   * Sets the device to write to to @p targetDevice.
   */
  void setTarget(QIODevice *targetDevice);

  /**
   * Returns the device being written to.
   */
  QIODevice *target() const;

private:
  Q_DECLARE_PRIVATE(Pump)
  PumpPrivate * const d_ptr;

  Q_PRIVATE_SLOT(d_func(), void pumpFromSource())
};

}

#endif
