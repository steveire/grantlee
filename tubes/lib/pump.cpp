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

#include "pump.h"

#include <QtCore/QWeakPointer>

namespace Grantlee
{

class PumpPrivate
{
  PumpPrivate( Pump *pump )
    : q_ptr( pump )
  {

  }

  Q_DECLARE_PUBLIC( Pump )
  Pump * const q_ptr;

  void pumpFromSource();

  QWeakPointer<QIODevice> source;
  QWeakPointer<QIODevice> target;
};

}

using namespace Grantlee;

void PumpPrivate::pumpFromSource()
{
  Q_Q( Pump );
  if( !source || !target ) {
    return;
  }
  target.data()->write( source.data()->readAll() );
}


Pump::Pump( QObject* parent )
  : QObject( parent ), d_ptr( new PumpPrivate( this ) )
{

}

void Pump::setSource( QIODevice* sourceDevice )
{
  Q_D( Pump );
  if( d->source )
    d->source.data()->disconnect( this );
  d->source = sourceDevice;
  if( sourceDevice && d->target.data() ) {
    d->target.data()->write( sourceDevice->readAll() );
    connect( d->source.data(), SIGNAL( readyRead() ), SLOT( pumpFromSource() ) );
  }
}

void Pump::setTarget( QIODevice* targetDevice )
{
  Q_D( Pump );
  d->target = targetDevice;
  if( d->source && d->target ) {
    d->target.data()->write( d->source.data()->readAll() );
    connect( d->source.data(), SIGNAL( readyRead() ), SLOT( pumpFromSource() ) );
  }
}

QIODevice* Pump::source() const
{
  Q_D( const Pump );
  return d->source.data();
}

QIODevice* Pump::target() const
{
  Q_D( const Pump );
  return d->target.data();
}

#include "pump.moc"
