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

#ifndef GRANTLEE_CHECKSUMTUBE_H
#define GRANTLEE_CHECKSUMTUBE_H

#include <QtCore/QIODevice>

#include "grantlee_tubes_export.h"

namespace Grantlee
{

class ChecksumPrivate;

/// @headerfile checksum.h grantlee/checksum.h

/**
  @brief Checksum writes a checksum of the data written to it to a target

  Checksum is a writable QIODevice that writes a checksum of data to a target when closed.

  @code
    QByteArray data = getData();

    QBuffer *checksumBuffer = new QBuffer(this);

    Grantlee::Checksum *checksum = new Grantlee::Checksum(Grantlee::Checksum::Sha1Sum, this);
    checksum->setTarget(checksumBuffer);

    checksum->write(data);
    checksum->close();

    qDebug() << "The checksum is" << checksumBuffer->buffer();
  @endcode

  This may be used together with Grantlee::Tee to checksum data while writing it.

  @code
    QIODevice *source = getSource();
    QIODevice *target = getTarget();

    QBuffer *checksumBuffer = new QBuffer(this);

    Grantlee::Tee *tee = new Grantlee::Tee(this);
    Grantlee::Pump *pump = new Grantlee::Pump(this);
    Grantlee::Checksum *checksum = new Grantlee::Checksum(Grantlee::Checksum::Sha1Sum, this);

    checksum->setTarget(checksumBuffer);
    tee->appendTarget(checksum);
    tee->appendTarget(target);

    pump->setSource(source);
    pump->setTarget(tee);
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
 */
class GRANTLEE_TUBES_EXPORT Checksum : public QIODevice
{
  Q_OBJECT
  Q_ENUMS( ChecksumType )
public:
  enum ChecksumType {
    Md4Sum,   ///< Create an Md4 checksum of the written data
    Md5Sum,   ///< Create an Md5 checksum of the written data
    Sha1Sum   ///< Create an Sha1 checksum of the written data
  };

  /**
    Constructor
   */
  Checksum( ChecksumType type, QObject *parent = 0 );

  /**
    Set the target device to write the checksum to.
   */
  void setTarget( QIODevice *target );

  /**
    The target device to write the checksum to.
   */
  QIODevice *target() const;

  /**
    Close the device and write the checksum to the target.
   */
  virtual void close();

protected:
  /**
    Write implementation
   */
  virtual qint64 writeData( const char* data, qint64 len );

  /**
    Read implementation
   */
  virtual qint64 readData( char* data, qint64 maxlen );

private:
  Q_DECLARE_PRIVATE( Checksum )
  ChecksumPrivate * const d_ptr;
};

}

#endif
