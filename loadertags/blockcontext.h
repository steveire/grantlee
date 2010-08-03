/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef BLOCKCONTEXT_H
#define BLOCKCONTEXT_H

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QVariant>

class BlockNode;

class BlockContext
{
public:
  void addBlocks( const QHash<QString, BlockNode*> &blocks );

  BlockNode* pop( const QString &name );

  void push( const QString &name, BlockNode *blockNode );

  BlockNode* getBlock( const QString &name ) const;

  bool isEmpty();

private:
  QHash<QString, QList<BlockNode*> > m_blocks;

};

Q_DECLARE_METATYPE( BlockContext )

#endif
