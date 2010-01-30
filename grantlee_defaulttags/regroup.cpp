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

#include "regroup.h"

#include <QStringList>
#include "parser.h"
#include "exception.h"

#include "util_p.h"

RegroupNodeFactory::RegroupNodeFactory()
{

}

Node* RegroupNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( ' ' );

  if ( expr.size() != 6 ) {
    throw Grantlee::Exception( TagSyntaxError, "widthratio takes five arguments" );
  }
  FilterExpression target( expr.at( 1 ), p );
  if ( expr.at( 2 ) != "by" ) {
    throw Grantlee::Exception( TagSyntaxError, "second argument must be 'by'" );
  }

  if ( expr.at( 4 ) != "as" ) {
    throw Grantlee::Exception( TagSyntaxError, "fourth argument must be 'as'" );
  }

  FilterExpression expression( "\"" + expr.at( 3 ) + "\"", p );

  QString name = expr.at( 5 );

  return new RegroupNode( target, expression, name, p );
}

RegroupNode::RegroupNode( FilterExpression target, FilterExpression expression, const QString &varName, QObject *parent )
    : Node( parent ), m_target( target ), m_expression( expression ), m_varName( varName )
{

}

QString RegroupNode::render( Context *c )
{
  QVariantList objList = m_target.resolve( c ).toList();
  if ( objList.isEmpty() ) {
    c->insert( m_varName, QVariantHash() );
    return QString( "" );
  }

  // What's going on?
  //
  // objList is a flat list of objects with a common parameter. For example, Person objects with
  // a name parameter. The list is already sorted.
  // Say the objList contains ["David Beckham", "David Blain", "Keira Nightly"] etc.
  // We want to regroup the list into separate lists of people with the same first name.
  // ie objHash should be: {"David": ["David Beckham", "David Blain"], "Keira": ["Keira Nightly"]}
  //
  // We then insert the objHash into the Context ready for rendering later in a for loop.

  QVariantList list;
  QVariantHash objHash;
  QVariantList contextList;
  QString hashKey;
  QString lastKey;
  QString keyName = Util::getSafeString( m_expression.resolve( c ) );
  QListIterator<QVariant> i( objList );
  while ( i.hasNext() ) {
    QVariant var = i.next();
    c->push();
    c->insert( "var", var );
    QString key = Util::getSafeString( FilterExpression( "var." + keyName, 0 ).resolve( c ) );
    c->pop();
    QVariantHash hash;
    if ( contextList.size() > 0 ) {
      QVariant hashVar = contextList.last();
      hash = hashVar.toHash();
    }
    if ( !hash.contains( "grouper" ) || hash.value( "grouper" ) != key ) {
      QVariantHash newHash;
      hash.insert( "grouper", key );
      hash.insert( "list", QVariantList() );
      contextList.append( newHash );
    }

    QVariantList list = hash.value( "list" ).toList();
    list.append( var );
    hash.insert( "list", list );
    contextList[contextList.size() - 1] = hash;
  }
  c->insert( m_varName, contextList );
  return QString();
}

