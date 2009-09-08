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

#include "extends.h"

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"
#include "engine.h"
#include "grantlee.h"

#include <QListIterator>
#include <util_p.h>

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory( QObject *parent )
    : AbstractNodeFactory( parent )
{

}

Node* ExtendsNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 2 )
    throw Grantlee::Exception( TagSyntaxError, "Error: Include tag takes only one argument" );

  QString parentName = expr.at( 1 );
  FilterExpression fe;
  int size = parentName.size();

  if (( parentName.startsWith( "\"" ) && parentName.endsWith( "\"" ) )
      || ( parentName.startsWith( "'" ) && parentName.endsWith( "'" ) ) ) {
    parentName = parentName.mid( 1, size - 2 );
  } else {
    fe = FilterExpression( parentName, p );
    parentName = QString();
  }

  ExtendsNode *n = new ExtendsNode( parentName, fe, p );

  TemplateImpl *t = qobject_cast<TemplateImpl *>( p->parent() );

  Q_ASSERT( t );

  NodeList nodeList = p->parse( t );
  n->setNodeList( nodeList );

  if ( t->findChildren<ExtendsNode *>().size() > 1 ) {
    throw Grantlee::Exception( TagSyntaxError, "Extends tag may only appear once in a template." );
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


void ExtendsNode::setNodeList( NodeList list )
{
  m_list = list;
}

Template ExtendsNode::getParent( Context *c )
{
  QString parentName;
  if ( m_name.isEmpty() ) {
    QVariant parentVar = m_filterExpression.resolve( c );
    if ( parentVar.userType() == qMetaTypeId<Grantlee::Template>() )
    {
      Template parentTemplate = parentVar.value<Template>();
      return parentTemplate;
    }

    if ( parentVar.userType() == qMetaTypeId<Grantlee::MutableTemplate>() )
    {
      Template parentTemplate = parentVar.value<Template>();
      return parentTemplate;
    }
    parentName = Util::getSafeString( parentVar );
  } else {
    parentName = m_name;
  }
  Engine *engine = Engine::instance();
  qint64 settingsToken = parent()->property( "settingsToken" ).toULongLong();

  Template t = engine->loadByName( parentName, settingsToken );

  return t;
}

QString ExtendsNode::render( Context *c )
{
  m_parentTemplate = getParent( c );

  if ( !m_parentTemplate ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "Cannot load template '%1'" ).arg( m_name ) );
  }

  QList<BlockNode*> nodeList = m_parentTemplate->findChildren<BlockNode *>();

  QHash<QString, BlockNode *> parentBlocks;

  QListIterator<BlockNode *> i( nodeList );

  while ( i.hasNext() ) {
    BlockNode* bn = i.next();
    parentBlocks.insert( bn->name(), bn );
  }

  QList<BlockNode*> l = parent()->findChildren<BlockNode *>();
  QListIterator<BlockNode *> j( l );

  while ( j.hasNext() ) {
    BlockNode *bn = j.next();
    if ( parentBlocks.contains( bn->name() ) ) {
      BlockNode *pbn = parentBlocks.value( bn->name() );
      pbn->setNodeParent( bn->takeNodeParent() );
      pbn->addParent( pbn->nodeList() );
      pbn->setNodeList( bn->nodeList() );
    } else {
      foreach( Node *node, m_parentTemplate->nodeList() ) {
        TextNode *tn = qobject_cast<TextNode*>( node );
        if ( !tn ) {
          ExtendsNode *en = qobject_cast<ExtendsNode*>( node );
          if ( en ) {
            en->appendNode( bn );
          }
          break;
        }
      }
    }
  }

  QString res = m_parentTemplate->render( c );
  return res;
}

void ExtendsNode::appendNode( Node *node )
{
  m_list.append( node );
  node->setParent( parent() );
}

