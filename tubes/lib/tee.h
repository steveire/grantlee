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

#ifndef GRANTLEE_TEE_H
#define GRANTLEE_TEE_H

#include "grantlee_tubes_export.h"

#include <QtCore/QIODevice>

namespace Grantlee
{

class TeePrivate;

/// @headerfile tee.h grantlee/tee.h

/**
  @brief Tee provides sequential write access to multiple devices at once.

  Tee implements QIODevice as a sequential write only proxy to many QIODevices.

  Any data written to an instance of Tee is written to all targets() set on the instance.

  @code
    QIODevice *sourceDevice = getSourceData();
    QFile *file1 = new File("file1");
    QFile *file2 = new File("file2");
    QFile *file3 = new File("file3");

    file1->open(QIODevice::WriteOnly | QIODevice::Text);
    file2->open(QIODevice::WriteOnly);
    file3->open(QIODevice::Append);

    Grantlee::Tee *tee = new Grantlee::Tee(this);
    tee->open(QIODevice::WriteOnly);

    tee->appendTarget(file1);
    tee->appendTarget(file2);
    tee->appendTarget(file3);

    tee->write(sourceDevice->readAll());
  @endcode

  Reading from Tee never returns any data.

  It only makes sense to open Tee in WriteOnly mode. Other flags such as QIODevice::Text should
  be specified on the target devices directly.

  Tee does not open its targets when it is opened itself. They must be opened separately using appropriate flags.
  Attempting to open Tee with any other mode will not work. It can only be open with WriteOpen, or NotOpen.

  Tee is sequential because there is no guarantee that all targets have the
  same size, and can be seek()ed to the same position. Additionally,
  the pos() returned by Tee could not be meaningful because of the multiple targets.

  Tee expects that all writes of data of all sizes is successful. If it is not possible to write all data to the target
  Tee considers this an error. It is the responsibilty of the targets to ensure that any write to them is successful,
  and the responsibility of the caller to ensure that only data which can be written to all targets is written to Tee.

  In summary, Tee does not provide a complete abstraction for the target devices, it only provides a write abstraction.

  @author Stephen Kelly <steveire@gmail.com>
*/
class GRANTLEE_TUBES_EXPORT Tee : public QIODevice
{
  Q_OBJECT
public:
  explicit Tee( QObject* parent = 0 );
  virtual ~Tee();

  /**
   * Appends @p target to the list of targets written to by Tee.
   */
  void appendTarget( QIODevice *target );

  /**
   * Returns the list of targets written to by Tee.
   */
  QList<QIODevice*> targets() const;

  /**
   * Set the targets written to by Tee to @p targets.
   */
  void setTargets( const QList<QIODevice*> &targets );

  /**
   * Returns true. Tee is always sequential.
   */
  bool isSequential() const;

  /**
   * Opens the Tee device.
   */
  virtual bool open(OpenMode mode);

protected:
#ifndef Q_QDOC
  /**
   * @internal
   */
  virtual qint64 readData(char* data, qint64 maxlen);

  /**
   * @internal Writes @p data to each device.
   */
  virtual qint64 writeData(const char* data, qint64 maxSize);
#endif
private:
  Q_DECLARE_PRIVATE(Tee)
  TeePrivate * const d_ptr;
};

}

#endif

