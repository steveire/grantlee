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

#include "ifequal.h"

#include <QStringList>

#include "parser.h"
#include "exception.h"

#include "util_p.h"


IfEqualNodeFactory::IfEqualNodeFactory()
{

}

Node* IfEqualNodeFactory::do_getNode( const QString &tagContent, Parser *p, bool negate ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() != 3 ) {
    throw Grantlee::Exception( TagSyntaxError, QString( "%1 tag takes two arguments." ).arg( expr.at( 0 ) ) );
  }

  QStringList vars;

  FilterExpression val1( expr.at( 1 ), p );
  FilterExpression val2( expr.at( 2 ), p );

  IfEqualNode *n = new IfEqualNode( val1 , val2, negate, p );

  const QString endTag( "end" + expr.at( 0 ) );
  NodeList trueList = p->parse( n, QStringList() << "else" << endTag );
  n->setTrueList( trueList );
  NodeList falseList;
  if ( p->nextToken().content.trimmed() == "else" ) {
    falseList = p->parse( n, QStringList() << endTag );
    n->setFalseList( falseList );
    p->deleteNextToken();
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

QString IfEqualNode::render( Context *c )
{
  QVariant var1 = m_var1.resolve( c );
  QVariant var2 = m_var2.resolve( c );

  bool equal = Util::equals( var1, var2 );

  if ((( m_negate && !equal ) || ( !m_negate && equal ) ) )
    return m_trueList.render( c );
  return m_falseList.render( c );

}
