/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class BlockNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  BlockNodeFactory( QObject *parent = 0 );

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};

class BlockNode : public Node
{
  Q_OBJECT
  Q_PROPERTY( Grantlee::SafeString super READ getSuper )
public:
  BlockNode( const QString &blockName, const NodeList &list, QObject *parent );

  QString blockName();

  QString render( Context *c );

  NodeList getNodesByType( const char * className );

  BlockNode* nodeParent() const;

  QString name();

  void setNodeParent( BlockNode * );

  void addParent( NodeList );

  void setNodeList( NodeList );
  NodeList nodeList();

// slots
  /**
  Returns the block overriden by this one rendered in context.
  */
  SafeString getSuper() const;

private:
//   FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;
  Context *m_context;
  BlockNode *m_parent;

};

#endif

