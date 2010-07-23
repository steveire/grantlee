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

#include "ifequal.h"

#include <QtCore/QStringList>

#include "parser.h"
#include "exception.h"

#include "util.h"


IfEqualNodeFactory::IfEqualNodeFactory()
{

}

Node* IfEqualNodeFactory::do_getNode( const QString &tagContent, Parser *p, bool negate ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 3 ) {
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "%1 tag takes two arguments." ).arg( expr.first() ) );
  }

  QStringList vars;

  FilterExpression val1( expr.at( 1 ), p );
  FilterExpression val2( expr.at( 2 ), p );

  IfEqualNode *n = new IfEqualNode( val1 , val2, negate, p );

  const QString endTag( QLatin1String( "end" ) + expr.first() );
  NodeList trueList = p->parse( n, QStringList() << QLatin1String( "else" ) << endTag );
  n->setTrueList( trueList );
  NodeList falseList;
  if ( p->takeNextToken().content.trimmed() == QLatin1String( "else" ) ) {
    falseList = p->parse( n, QStringList() << endTag );
    n->setFalseList( falseList );
    p->removeNextToken();
  } // else empty falseList.

  return n;
}

Node* IfEqualNodeFactory::getNode( const QString &tagContent, Parser *p ) const

{
  return do_getNode( tagContent, p, false );
}


IfNotEqualNodeFactory::IfNotEqualNodeFactory()
{

}

Node* IfNotEqualNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  return do_getNode( tagContent, p, true );
}

IfEqualNode::IfEqualNode( FilterExpression val1, FilterExpression val2, bool negate, QObject *parent )
    : Node( parent )
{
  m_var1 = val1;
  m_var2 = val2;
  m_negate = negate;
}

void IfEqualNode::setTrueList( NodeList trueList )
{
  m_trueList = trueList;
}

void IfEqualNode::setFalseList( NodeList falseList )
{
  m_falseList = falseList;
}

void IfEqualNode::render( OutputStream *stream, Context *c )
{
  QVariant var1 = m_var1.resolve( c );
  QVariant var2 = m_var2.resolve( c );

  bool equal = equals( var1, var2 );

  if ((( m_negate && !equal ) || ( !m_negate && equal ) ) )
    m_trueList.render( stream, c );
  else
    m_falseList.render( stream, c );

}

