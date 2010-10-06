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

#include "blockcontext.h"

#include "block.h"

void BlockContext::addBlocks( const QHash<QString, BlockNode*> &blocks)
{
  QHash<QString, BlockNode*>::const_iterator it = blocks.constBegin();
  const QHash<QString, BlockNode*>::const_iterator end = blocks.constEnd();

  for ( ; it != end; ++it ) {
    m_blocks[ it.key() ].prepend( it.value() );
  }
}

BlockNode* BlockContext::getBlock( const QString& name ) const
{
  QList<BlockNode*> list = m_blocks[ name ];
  if ( list.isEmpty() )
    return 0;

  return list.last();
}

BlockNode* BlockContext::pop( const QString& name )
{
  QList<BlockNode*> &list = m_blocks[ name ];
  if ( list.isEmpty() )
    return 0;
  return list.takeLast();
}

void BlockContext::push( const QString& name, BlockNode* blockNode )
{
  m_blocks[ name ].append( blockNode );
}

bool BlockContext::isEmpty()
{
  return m_blocks.isEmpty();
}

