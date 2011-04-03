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

#include "channel.h"

#include <QDebug>

using namespace Grantlee;

namespace Grantlee
{

class ChannelPrivate
{
  ChannelPrivate( Channel *qq )
    : q_ptr( qq ) {

  }
  Channel * const q_ptr;
  Q_DECLARE_PUBLIC( Channel )

  QByteArray data;
};

}

Channel::Channel( QObject* parent )
  : QIODevice( parent ), d_ptr( new ChannelPrivate( this ) )
{

}

qint64 Channel::size() const
{
  Q_D( const Channel );
  return QIODevice::size() + d->data.size();
}

bool Channel::isSequential() const
{
  return true;
}

qint64 Channel::readData( char* data, qint64 maxlen )
{
  Q_D( Channel );
  memcpy( data, d->data.constData(), maxlen );
  const qint64 prevSize = d->data.size();
  d->data.remove( 0, maxlen );
  return qMin(prevSize, maxlen);
}

qint64 Channel::writeData( const char* data, qint64 len )
{
  Q_D( Channel );
  d->data.append( data, len );
  emit bytesWritten( len );
  emit readyRead();
  return len;
}

#include "channel.moc"
