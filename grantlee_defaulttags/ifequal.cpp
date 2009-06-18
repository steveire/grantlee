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

#include "util_p.h"


IfEqualNodeFactory::IfEqualNodeFactory()
{

}

Node* IfEqualNodeFactory::do_getNode( const QString &tagContent, Parser *p, bool negate ) const
{
  QStringList expr = smartSplit( tagContent );

  QList<QString> vars;

  FilterExpression val1( expr.at( 1 ), p );
  FilterExpression val2( expr.at( 2 ), p );

  const QString endTag( "end" + expr.at( 0 ) );
  NodeList trueList = p->parse( QStringList() << "else" << endTag );
  NodeList falseList;
  if ( p->nextToken().content.trimmed() == "else" ) {
    falseList = p->parse( QStringList() << endTag );
    p->deleteNextToken();
  } // else empty falseList.

  return new IfEqualNode( val1 , val2, trueList, falseList, negate );
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

IfEqualNode::IfEqualNode( FilterExpression val1, FilterExpression val2, NodeList trueList, NodeList falseList, bool negate, QObject *parent )
    : Node( parent )
{
  m_var1 = val1;
  m_var2 = val2;
  m_trueList = trueList;
  m_falseList = falseList;
  m_negate = negate;
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

NodeList IfEqualNode::getNodesByType( const char* className )
{
  NodeList list;
  list << m_trueList.getNodesByType( className );
  list << m_falseList.getNodesByType( className );
  return list;
}

