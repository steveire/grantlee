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

#include "extends.h"

#include "block.h"

#include <QtCore/QStringList>

#include "nodebuiltins_p.h"
#include "parser.h"
#include "template.h"
#include "engine.h"
#include "exception.h"

#include <QtCore/QListIterator>
#include <util.h>
#include <rendercontext.h>
#include "blockcontext.h"

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory( QObject *parent )
    : AbstractNodeFactory( parent )
{

}

Node* ExtendsNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  const QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 2 )
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: Include tag takes only one argument" ) );

  QString parentName = expr.at( 1 );
  FilterExpression fe;
  const int size = parentName.size();

  if (( parentName.startsWith( QLatin1Char( '"' ) ) && parentName.endsWith( QLatin1Char( '\"' ) ) )
      || ( parentName.startsWith( QLatin1Char( '\'' ) ) && parentName.endsWith( QLatin1Char( '\'' ) ) ) ) {
    parentName = parentName.mid( 1, size - 2 );
  } else {
    fe = FilterExpression( parentName, p );
    parentName.clear();
  }

  ExtendsNode *n = new ExtendsNode( parentName, fe, p );

  TemplateImpl *t = qobject_cast<TemplateImpl *>( p->parent() );

  if ( !t )
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Extends tag is not in a template." ) );

  const NodeList nodeList = p->parse( t );
  n->setNodeList( nodeList );

  if ( t->findChildren<ExtendsNode *>().size() > 1 ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Extends tag may only appear once in a template." ) );
  }

  return n;
}

ExtendsNode::ExtendsNode( const QString &name, FilterExpression fe, QObject *parent )
    : Node( parent ),
    m_filterExpression( fe ),
    m_name( name )
{
}


ExtendsNode::~ExtendsNode()
{
}

static QHash<QString, BlockNode*> createNodeMap( QList<BlockNode*> list )
{
  QHash<QString, BlockNode*> map;

  QList<BlockNode*>::const_iterator it = list.constBegin();
  const QList<BlockNode*>::const_iterator end = list.constEnd();

  for ( ; it != end; ++it )
  {
    map.insert( ( *it )->name(), *it );
  }

  return map;
}

void ExtendsNode::setNodeList( const NodeList &list )
{
  m_list = list;

  const QList<BlockNode*> blockList = m_list.findChildren<BlockNode*>();
  m_blocks = createNodeMap( blockList );
}

Template ExtendsNode::getParent( Context *c )
{
  QString parentName;
  if ( m_name.isEmpty() ) {
    const QVariant parentVar = m_filterExpression.resolve( c );
    if ( parentVar.userType() == qMetaTypeId<Grantlee::Template>() ) {
      return parentVar.value<Template>();
    }

    if ( parentVar.userType() == qMetaTypeId<Grantlee::MutableTemplate>() ) {
      Template parentTemplate = parentVar.value<Template>();
      return parentTemplate;
    }
    parentName = getSafeString( parentVar );
  } else {
    parentName = m_name;
  }

  TemplateImpl *ti = containerTemplate();

  const Template t = ti->engine()->loadByName( parentName );

  if ( !t )
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "Template not found %1" ).arg( parentName ) );

  if ( t->error() )
    throw Grantlee::Exception( t->error(), t->errorString() );

  return t;
}

void ExtendsNode::render( OutputStream *stream, Context *c )
{
  m_parentTemplate = getParent( c );

  if ( !m_parentTemplate ) {
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "Cannot load template '%1'" ).arg( m_name ) );
  }

  QVariant &variant = c->renderContext()->data( 0 );
  BlockContext blockContext = variant.value<BlockContext>();
  blockContext.addBlocks( m_blocks );
  variant.setValue( blockContext );

  const NodeList nodeList = m_parentTemplate->nodeList();

  const QHash<QString, BlockNode*> parentBlocks = createNodeMap( m_parentTemplate->findChildren<BlockNode*>() );
  QListIterator<Node*> i( nodeList );

  while ( i.hasNext() ) {
    Node* n = i.next();
    TextNode *tn = qobject_cast<TextNode*>( n );
    if ( !tn ) {
      ExtendsNode *en = qobject_cast<ExtendsNode*>( n );
      if ( !en ) {
        blockContext.addBlocks( parentBlocks );
        variant.setValue( blockContext );
      }
      break;
    }
  }
  variant.setValue( blockContext );
  m_parentTemplate->nodeList().render( stream, c );
}

void ExtendsNode::appendNode( Node *node )
{
  m_list.append( node );
  node->setParent( parent() );
}

