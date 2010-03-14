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

#include "scriptableparser.h"
#include "scriptablenode.h"

#include "parser.h"


ScriptableParser::ScriptableParser( Grantlee::Parser* p, QObject* parent )
    : QObject( parent ), m_p( p )
{
}

void ScriptableParser::deleteNextToken()
{
  m_p->deleteNextToken();
}

bool ScriptableParser::hasNextToken() const
{
  return m_p->hasNextToken();
}

void ScriptableParser::loadLib( const QString& name )
{
  m_p->loadLib( name );
}

Token ScriptableParser::nextToken()
{
  return m_p->nextToken();
}

void ScriptableParser::skipPast( const QString& tag )
{
  m_p->skipPast( tag );
}

QObjectList ScriptableParser::parse( QObject *parent, const QString& stopAt )
{
  return parse( parent, QStringList() << stopAt );
}

QObjectList ScriptableParser::parse( QObject *parent, const QStringList& stopAt )
{
  Node *node = qobject_cast<Node*>( parent );
  Q_ASSERT( node );

  NodeList nodeList = m_p->parse( node, stopAt );
  QObjectList objList;
  QListIterator<Node*> it( nodeList );
  while ( it.hasNext() ) {
    objList << it.next();
  }
  return objList;
}

