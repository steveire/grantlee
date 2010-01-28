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

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"
#include "exception.h"
#include <util_p.h>


const char * __loadedBlocks = "__loadedBlocks";

BlockNodeFactory::BlockNodeFactory( QObject *parent ) : AbstractNodeFactory( parent )
{

}

Node* BlockNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 2 ) {
    throw Grantlee::Exception( TagSyntaxError, "block tag takes one argument" );
  }

  QString blockName = expr.at( 1 );

  QVariant loadedBlocksVariant = p->property( __loadedBlocks );
  QVariantList blockVariantList;

  if ( loadedBlocksVariant.isValid() && loadedBlocksVariant.type() == QVariant::List ) {
    blockVariantList = loadedBlocksVariant.toList();
    QListIterator<QVariant> it( blockVariantList );
    while ( it.hasNext() ) {
      QString blockNodeName = it.next().toString();

      if ( blockNodeName == blockName ) {
        throw Grantlee::Exception( TagSyntaxError, QString( "%1 appears more than once." ).arg( blockName ) );
      }
    }
  }
  // Block not already in list.
  blockVariantList.append( blockName );
  loadedBlocksVariant = QVariant( blockVariantList );

  p->setProperty( __loadedBlocks, loadedBlocksVariant );

  BlockNode *n = new BlockNode( blockName, p );
  NodeList list = p->parse( n, QStringList() << "endblock" << "endblock " + blockName );

  n->setNodeList( list );
  p->deleteNextToken();

  return n;
}

BlockNode::BlockNode( const QString &name, QObject *parent )
    : Node( parent ), m_nodeParent( 0 )
{
  m_name = name;

  qRegisterMetaType<Grantlee::SafeString>( "Grantlee::SafeString" );
}

BlockNode::~BlockNode()
{
  delete m_nodeParent;
}

void BlockNode::setNodeList( NodeList list )
{
  m_list = list;
}

QString BlockNode::render( Context *c )
{
  c->push();
  m_context = c;
  c->insert( "block", QVariant::fromValue( static_cast<QObject *>( this ) ) );
  QString result = m_list.render( c );
  c->pop();
  return result;
}

SafeString BlockNode::getSuper() const
{
  // If blocks which are siblings in a parent are arranged nested in an extended template,
  // and {{ block.super }} is used, m_nodeParent will be 0 in the nested instance of
  // the block. See testExtendsTag:inheritance33
  if ( !m_nodeParent )
    return SafeString();

  return Util::markSafe( m_nodeParent->render( m_context ) );
}

QString BlockNode::blockName()
{
  return m_name;
}

BlockNode* BlockNode::takeNodeParent()
{
  BlockNode *n = m_nodeParent;
  // Make sure there's only one valid pointer to m_nodeParent
  // It's safe to delete 0.
  m_nodeParent = 0;
  return n;
}

void BlockNode::addParent( NodeList nodeList )
{
  if ( m_nodeParent )
    m_nodeParent->addParent( nodeList );
  else {
    // This new node is not really part of the template, so it can't be added as
    // a descendant of it or else it would show up in findChildren for the template.
    // So, we manage the resource manually.
    BlockNode *n = new BlockNode( m_name );
    n->setNodeList( nodeList );
    m_nodeParent = n;
  }
}

void BlockNode::setNodeParent( BlockNode* node )
{
  m_nodeParent = node;
}

NodeList BlockNode::nodeList()
{
  return m_list;
}

QString BlockNode::name()
{
  return m_name;
}

