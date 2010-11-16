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

#include "rendercontext.h"

#include "node.h"

using namespace Grantlee;

namespace Grantlee
{

class RenderContextPrivate
{
  RenderContextPrivate( RenderContext * qq )
    : q_ptr( qq )
  {

  }

  Q_DECLARE_PUBLIC( RenderContext )
  RenderContext * const q_ptr;

  QList<QHash<const Node *, QVariant> > m_variantHashStack;
};

}

RenderContext::RenderContext()
  : d_ptr( new RenderContextPrivate( this ) )
{

}

RenderContext::~RenderContext()
{
  delete d_ptr;
}

void RenderContext::push()
{
  Q_D( RenderContext );
  const QHash<const Node *, QVariant> hash;
  d->m_variantHashStack.prepend( hash );
}

bool RenderContext::contains( Node * const scopeNode ) const
{
  Q_D( const RenderContext );
  Q_ASSERT( !d->m_variantHashStack.isEmpty() );
  return d->m_variantHashStack.last().contains( scopeNode );
}

QVariant& RenderContext::data( const Node * const scopeNode )
{
  Q_D( RenderContext );
  Q_ASSERT( !d->m_variantHashStack.isEmpty() );
  return d->m_variantHashStack.last()[ scopeNode ];
}

void RenderContext::pop()
{
  Q_D( RenderContext );
  d->m_variantHashStack.removeFirst();
}
