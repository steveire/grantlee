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

#include "if.h"

#include <QStringList>

#include "parser.h"
#include "grantlee.h"


IfNodeFactory::IfNodeFactory()
{

}


Node* IfNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );
  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, "'if' statement requires at least one argument" );
  }

  int linkType = IfNode::OrLink;

  QString exprString = expr.join( " " );

  QStringList boolPairs = exprString.split( " and " );

  if ( boolPairs.size() == 1 ) {
    boolPairs = exprString.split( " or " );
  } else {
    linkType = IfNode::AndLink;
    if ( exprString.contains( " or " ) ) {
      throw Grantlee::Exception( TagSyntaxError, "'if' tags can't mix 'and' and 'or'" );
    }
  }

  QList<QPair<bool, FilterExpression > > boolVars;
  foreach( const QString &boolStr, boolPairs ) {
    QPair<bool, FilterExpression> pair;
    if ( boolStr.contains( " " ) ) {
      QStringList bits = boolStr.split( " " );
      if ( bits.size() != 2 ) {
        throw Grantlee::Exception( TagSyntaxError, "'if' statement improperly formatted" );
      }
      if ( bits.at( 0 ) != "not" ) {
        throw Grantlee::Exception( TagSyntaxError, "Expected 'not' in if statement" );
      }
      pair.first = true;
      pair.second = FilterExpression( bits.at( 1 ).trimmed(), p );
    } else {
      pair.first = false;
      pair.second = FilterExpression( boolStr.trimmed(), p );
    }
    boolVars.append( pair );
  }

  IfNode *n = new IfNode( boolVars, linkType, p );

  NodeList trueList = p->parse( n, QStringList() << "else" << "endif" );
  n->setTrueList( trueList );
  NodeList falseList;
  if ( p->nextToken().content.trimmed() == "else" ) {
    falseList = p->parse( n, QStringList() << "endif" );
    n->setFalseList( falseList );
    // skip past the endif tag
    p->deleteNextToken();
  } // else empty falseList.

  return n;
}


IfNode::IfNode( QList<QPair<bool, FilterExpression > > boolVars, int linkType, QObject *parent )
    : Node( parent ),
    m_boolVars( boolVars ),
    m_linkType( linkType )
{

}

void IfNode::setTrueList( NodeList trueList )
{
  m_trueList = trueList;
}

void IfNode::setFalseList( NodeList falseList )
{
  m_falseList = falseList;
}

QString IfNode::render( Context *c )
{
  // Evaluate the expression. rendering variables with the context as needed. and processing nodes recursively
  // in either trueList or falseList as determined by booleanExpression.

  if ( m_linkType == OrLink ) {
    for ( int i = 0; i < m_boolVars.size(); i++ ) {
      QPair<bool, FilterExpression> pair = m_boolVars.at( i );
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue( c );

      if ( isTrue != negate ) {
        return renderTrueList( c );
      }
    }
//     return renderFalseList(c);
  } else {
    bool renderTrue = true;
    for ( int i = 0; i < m_boolVars.size(); i++ ) {
      QPair<bool, FilterExpression> pair = m_boolVars.at( i );
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue( c );

      // Karnaugh map:
      //          VariantIsTrue
      //          \ 0   1
      //         0| 0 | 1 |
      // negate  1| 1 | 0 |

      if (( !isTrue && !negate )
          || ( isTrue && negate ) ) {
        renderTrue = false;
        break;
      }
    }
    if ( renderTrue )
      return renderTrueList( c );
  }

  return renderFalseList( c );
}

QString IfNode::renderTrueList( Context *c )
{
  return m_trueList.render( c );
}

QString IfNode::renderFalseList( Context *c )
{
  return m_falseList.render( c );
}

