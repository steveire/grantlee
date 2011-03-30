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

#include "cat.h"

#include <QtCore/QDebug>
#include <QtCore/QWeakPointer>

#include "pump.h"

namespace Grantlee
{

class CatPrivate
{
  CatPrivate( Cat *qq )
    : q_ptr( qq ),
      m_pump( new Pump( qq ) )
  {
  }

  Q_DECLARE_PUBLIC( Cat )
  Cat * const q_ptr;

  void setActiveSource( QList<QWeakPointer<QIODevice> >::const_iterator activeSource ) {
    Q_Q( Cat );
    Q_ASSERT( activeSource != m_sources.constEnd() );
    if( m_activeSource != m_sources.constEnd() )
      q->disconnect( m_activeSource->data(), SIGNAL( aboutToClose() ), q, SLOT( sourceReadFinished() ) );
    m_activeSource = activeSource;
    m_pump->setSource( activeSource->data() );
    if( m_activeSource->data() )
      q->connect( activeSource->data(), SIGNAL( aboutToClose() ), q, SLOT( sourceReadFinished() ) );
  }

  void sourceReadFinished() {
    ++m_activeSource;
    if( m_activeSource != m_sources.constEnd() )
      setActiveSource( m_activeSource );
  }

  QList<QWeakPointer<QIODevice> > m_sources;
  QList<QWeakPointer<QIODevice> >::const_iterator m_activeSource;

  Pump * const m_pump;
};

}

using namespace Grantlee;

Cat::Cat( QObject* parent )
  : QObject( parent ), d_ptr( new CatPrivate( this ) )
{

}

void Cat::setTarget( QIODevice* targetDevice )
{
  Q_D( Cat );
  d->m_pump->setTarget( targetDevice );
}

QIODevice* Cat::target() const
{
  Q_D( const Cat );
  return d->m_pump->target();
}

void Cat::appendSource( QIODevice* source )
{
  Q_D( Cat );
  d->m_sources.append( source );
  if( d->m_sources.size() == 1 ) {
    d->m_activeSource = d->m_sources.constEnd();
    d->setActiveSource( d->m_sources.constBegin() );
  }
}

void Cat::setSources( const QList<QIODevice*>& sources )
{
  Q_D( Cat );
  d->m_sources.clear();
  QList<QIODevice*>::const_iterator it = sources.constBegin();
  const QList<QIODevice*>::const_iterator end = sources.constEnd();

  while( it != end )
    d->m_sources.append( *it );

  if( !d->m_sources.isEmpty() ) {
    d->m_activeSource = d->m_sources.constEnd();
    d->setActiveSource( d->m_sources.constBegin() );
  }
}

QList<QIODevice*> Cat::sources() const
{
  Q_D( const Cat );
  QList<QIODevice *> sources;
  foreach( const QWeakPointer<QIODevice> &source, d->m_sources ) {
    if( source )
      sources.append( source.data() );
  }
  return sources;
}

#include "cat.moc"
