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

#ifndef GRANTLEE_CHANNEL_H
#define GRANTLEE_CHANNEL_H

#include <QtCore/QIODevice>

#include "grantlee_tubes_export.h"

namespace Grantlee
{

class ChannelPrivate;

/// @headerfile channel.h grantlee/channel.h

/**
  @brief Channel provides a FIFO data buffer

  In situations where the API of one class is to write
  to a QIODevice, and the API of another class is to read from
  a QIODevice, a Channel can be used to funnel data from the
  source device to the target device.

  @code
    QTextDocument *document = getDocument();

    Grantlee::Channel *channel = new Grantlee::Channel(this);
    channel->open(QIODevice::ReadWrite);

    // Write to the channel
    QTextDocumentWriter *writer = new QTextDocumentWriter(channel, this);

    // Read from the channel
    QXmlStreamReader reader(channel);

    // Use the reader.
  @endcode

  Channel emits readyRead and bytesAvailable whenever data is written to it.

  @author Stephen Kelly <steveire@gmail.com>
 */
class GRANTLEE_TUBES_EXPORT Channel : public QIODevice
{
  Q_OBJECT
public:
  /**
    Constructor
   */
  Channel( QObject *parent = 0 );

  /**
    Returns true. Channel is sequential
  */
  virtual bool isSequential() const;

  /**
    Returns the amount of data available in the channel.
  */
  virtual qint64 size() const;

protected:
  virtual qint64 readData( char* data, qint64 maxlen );
  virtual qint64 writeData( const char* data, qint64 len );

private:
  Q_DECLARE_PRIVATE( Channel )
  ChannelPrivate * const d_ptr;
};

}

#endif
