/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#include "block.h"

#include <QtCore/QStringList>

#include "parser.h"
#include "template.h"
#include "exception.h"
#include <util.h>


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
  p->removeNextToken();

  return n;
}

BlockNode::BlockNode( const QString &name, QObject *parent )
    : Node( parent ), m_nodeParent( 0 ), m_stream( 0 )
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

void BlockNode::render( OutputStream *stream, Context *c )
{
  c->push();
  m_context = c;
  m_stream = stream;
  c->insert( "block", QVariant::fromValue( static_cast<QObject *>( this ) ) );
  m_list.render( stream, c );
  m_stream = 0;
  c->pop();
}

SafeString BlockNode::getSuper() const
{
  // If blocks which are siblings in a parent are arranged nested in an extended template,
  // and {{ block.super }} is used, m_nodeParent will be 0 in the nested instance of
  // the block. See testExtendsTag:inheritance33
  if ( !m_nodeParent )
    return SafeString();

  QString superContent;
  QTextStream superTextStream( &superContent );
  QSharedPointer<OutputStream> superStream = m_stream->clone( &superTextStream );
  m_nodeParent->render( superStream.data(), m_context );
  return markSafe( superContent );
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

