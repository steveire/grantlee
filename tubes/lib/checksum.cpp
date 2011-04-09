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

#include "checksum.h"

#include <QtCore/QCryptographicHash>

namespace Grantlee
{

static QCryptographicHash::Algorithm mapToCryptoHash( Checksum::ChecksumType cryptoHash )
{
  return ( QCryptographicHash::Algorithm )cryptoHash;
}

class ChecksumPrivate
{
  ChecksumPrivate( Checksum::ChecksumType checkSumType, Checksum * qq )
    : q_ptr( qq ),
      m_checkSumType( checkSumType ),
      m_cryptoHash( mapToCryptoHash( checkSumType ) ) {

  }

  Q_DECLARE_PUBLIC( Checksum )
  Checksum * const q_ptr;

  const Checksum::ChecksumType m_checkSumType;
  QCryptographicHash m_cryptoHash;
  QIODevice *m_target;
};

}

using namespace Grantlee;

Checksum::Checksum( ChecksumType checkSumType, QObject* parent )
  : QIODevice( parent ), d_ptr( new ChecksumPrivate( checkSumType, this ) )
{

}

void Checksum::setTarget( QIODevice* target )
{
  Q_D( Checksum );
  d->m_target = target;
}

QIODevice* Checksum::target() const
{
  Q_D( const Checksum );
  return d->m_target;
}

qint64 Checksum::readData( char* data, qint64 maxlen )
{
  return -1;
}

qint64 Checksum::writeData( const char* data, qint64 len )
{
  Q_D( Checksum );
  d->m_cryptoHash.addData( data, len );
  return len;
}

void Checksum::close()
{
  Q_D( Checksum );
  QIODevice::close();
  d->m_target->write( d->m_cryptoHash.result().toHex() );
}

#include "checksum.moc"
