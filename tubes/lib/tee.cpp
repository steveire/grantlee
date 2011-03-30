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

#include "tee.h"

#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <QtCore/QWeakPointer>
#include <QtCore/QTime>

namespace Grantlee
{

class TeePrivate
{
  TeePrivate( Tee *qq )
    : q_ptr( qq ) {

  }
  Q_DECLARE_PUBLIC( Tee )
  Tee * const q_ptr;

  QVector<QWeakPointer<QIODevice> > m_targets;
};

}

using namespace Grantlee;

Tee::Tee( QObject* parent )
  : QIODevice( parent ), d_ptr( new TeePrivate( this ) )
{
}

Tee::~Tee()
{

}

void Tee::appendTarget( QIODevice* target )
{
  Q_D( Tee );
  d->m_targets.append( target );
}

void Tee::setTargets( const QList<QIODevice*>& targets )
{
  Q_D( Tee );
  d->m_targets.clear();
  foreach( QIODevice * target, targets )
  d->m_targets.append( target );
}

QList<QIODevice*> Tee::targets() const
{
  Q_D( const Tee );
  QList<QIODevice *> targets;
  foreach( const QWeakPointer<QIODevice> &target, d->m_targets ) {
    if( target )
      targets.append( target.data() );
  }
  return targets;
}

bool Tee::isSequential() const
{
  return true;
}

qint64 Tee::readData( char* data, qint64 maxlen )
{
  return -1;
}

qint64 Tee::writeData( const char* data, qint64 maxSize )
{
  Q_D( Tee );
  QVector<QWeakPointer<QIODevice> >::iterator it = d->m_targets.begin();

  while( it != d->m_targets.end() ) {
    if( *it ) {
      const int bytesWritten = it->data()->write( data, maxSize );
      if( bytesWritten != maxSize ) {
        // TODO: Define error strings somewhere.
        // TODO: Add QIODevice API for error codes.
        // TODO: Close the device?
        setErrorString( QLatin1String( "Write Error" ) );
        return -1;
      }
      ++it;
    } else {
      it = d->m_targets.erase( it );
      // TODO: Close the device?
      setErrorString( QLatin1String( "Write Error" ) );
      return -1;
    }
  }
  emit bytesWritten( maxSize );
  return maxSize;
}

bool Tee::open( QIODevice::OpenMode mode )
{
  if( mode == QIODevice::WriteOnly || mode == QIODevice::NotOpen )
    return QIODevice::open( mode );
  return false;
}


#include "tee.moc"
