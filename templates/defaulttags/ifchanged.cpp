/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#include "ifchanged.h"

#include "parser.h"

#include <QtCore/QDateTime>


IfChangedNodeFactory::IfChangedNodeFactory()
{

}

Node* IfChangedNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );

  expr.takeAt( 0 );
  IfChangedNode *n =  new IfChangedNode( getFilterExpressionList( expr, p ), p );

  NodeList trueList = p->parse( n, QStringList() << QStringLiteral( "else" ) << QStringLiteral( "endifchanged" ) );
  n->setTrueList( trueList );
  NodeList falseList;

  if ( p->takeNextToken().content == QStringLiteral( "else" ) ) {
    falseList = p->parse( n, QStringLiteral( "endifchanged" ) );
    n->setFalseList( falseList );
    p->removeNextToken();
  }

  return n;
}

IfChangedNode::IfChangedNode(const QList<FilterExpression>& feList, QObject *parent )
    : Node( parent ), m_filterExpressions( feList )
{
  m_lastSeen = QVariant();
  m_id = QString::number( reinterpret_cast<qint64>( this ) );
}

void IfChangedNode::setTrueList( const NodeList &trueList )
{
  m_trueList = trueList;
}

void IfChangedNode::setFalseList(const NodeList& falseList )
{
  m_falseList = falseList;
}

void IfChangedNode::render( OutputStream *stream, Context *c ) const
{
  if ( c->lookup( QStringLiteral( "forloop" ) ).isValid() && ( !c->lookup( QStringLiteral( "forloop" ) ).value<QVariantHash>().contains( m_id ) ) ) {
    m_lastSeen = QVariant();
    QVariantHash hash = c->lookup( QStringLiteral( "forloop" ) ).value<QVariantHash>();
    hash.insert( m_id, 1 );
    c->insert( QStringLiteral( "forloop" ), hash );
  }

  QString watchedString;
  QTextStream watchedTextStream( &watchedString );
  QSharedPointer<OutputStream> watchedStream = stream->clone( &watchedTextStream );
  if ( m_filterExpressions.size() == 0 ) {
    m_trueList.render( watchedStream.data(), c );
  }
  QListIterator<FilterExpression> i( m_filterExpressions );
  QVariantList watchedVars;
  while ( i.hasNext() ) {
    QVariant var = i.next().resolve( c );
    if ( !var.isValid() ) {
      // silent error
      return;
    }
    watchedVars.append( var );
  }

  // At first glance it looks like m_last_seen will always be invalid,
  // But it will change because render is called multiple times by the parent
  // {% for %} loop in the template.
  if (( watchedVars != m_lastSeen.value<QVariantList>() )
      || ( !watchedString.isEmpty() && ( watchedString != m_lastSeen.value<QString>() ) ) ) {
    bool firstLoop = !m_lastSeen.isValid();
    if ( !watchedString.isNull() )
      m_lastSeen = watchedString;
    else
      m_lastSeen = watchedVars;
    c->push();
    QVariantHash hash;
    // TODO: Document this.
    hash.insert( QStringLiteral( "firstloop" ), firstLoop );
    c->insert( QStringLiteral( "ifchanged" ), hash );
    m_trueList.render( stream, c );
    c->pop();
  } else if ( !m_falseList.isEmpty() ) {
    m_falseList.render( stream, c );
  }
}
