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

#include "ifchanged.h"

#include <QStringList>
#include <QDateTime>

#include "parser.h"



IfChangedNodeFactory::IfChangedNodeFactory()
{

}

Node* IfChangedNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( " ", QString::SkipEmptyParts );

  expr.takeAt( 0 );
  IfChangedNode *n =  new IfChangedNode( getFilterExpressionList( expr, p ) );

  NodeList trueList = p->parse( n, QStringList() << "else" << "endifchanged" );
  n->setTrueList( trueList );
  NodeList falseList;

  if ( p->nextToken().content.trimmed() == "else" ) {
    falseList = p->parse( n, QStringList() << "endifchanged" );
    n->setFalseList( falseList );
    p->deleteNextToken();
  }

  return n;
}

IfChangedNode::IfChangedNode( QList<FilterExpression> feList, QObject *parent )
    : Node( parent ), m_filterExpressions( feList )
{
  m_lastSeen = QVariant();
  m_id = QString::number( reinterpret_cast<qint64>( this ) );
}

void IfChangedNode::setTrueList( NodeList trueList )
{
  m_trueList = trueList;
}

void IfChangedNode::setFalseList( NodeList falseList )
{
  m_falseList = falseList;
}

QString IfChangedNode::render( Context *c )
{
  if ( c->lookup( "forloop" ).isValid() && ( !c->lookup( "forloop" ).toHash().contains( m_id ) ) ) {
    m_lastSeen = QVariant();
    QVariantHash hash = c->lookup( "forloop" ).toHash();
    hash.insert( m_id, 1 );
    c->insert( "forloop", hash );
  }

  QString watchedString;
  if ( m_filterExpressions.size() == 0 ) {
    watchedString = m_trueList.render( c );
  }
  QListIterator<FilterExpression> i( m_filterExpressions );
  QVariantList watchedVars;
  while ( i.hasNext() ) {
    QVariant var = i.next().resolve( c );
    if ( !var.isValid() ) {
      // silent error
      return QString();
    }
    watchedVars << var;
  }

  // At first glance it looks like m_last_seen will always be invalid,
  // But it will change because render is called multiple times by the parent
  // for loop.
  if (( watchedVars != m_lastSeen.toList() )
      || ( !watchedString.isNull() && ( watchedString != m_lastSeen.toString() ) ) ) {
    bool firstLoop = !m_lastSeen.isValid();
    if ( !watchedString.isNull() )
      m_lastSeen = watchedString;
    else
      m_lastSeen = watchedVars;
    c->push();
    QVariantHash hash;
    // TODO: Document this.
    hash.insert( "firstloop", firstLoop );
    c->insert( "ifchanged", hash );
    QString content = m_trueList.render( c );
    c->pop();
    return content;
  } else if ( !m_falseList.isEmpty() ) {
    return m_falseList.render( c );
  }


  return QString();
}
