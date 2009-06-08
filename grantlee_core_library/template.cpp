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

#include "template.h"

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "grantlee.h"

using namespace Grantlee;

NodeList Template::compileString( const QString &str )
{
  Lexer l( str );
  Parser p( l.tokenize(), this );
  NodeList nodeList = p.parse( this );

  if ( NoError != p.error() ) {
    setError( p.error(), p.errorString() );
  }
  return nodeList;
}

Template::Template( QObject *parent )
    : QObject( parent ),
    m_error( NoError )
{
}

void Template::setContent( const QString &templateString )
{
  if ( !templateString.isEmpty() )
    m_nodeList = compileString( templateString );
}

NodeList Template::getNodesByType( const char* className )
{
  return m_nodeList.getNodesByType( className );
}

QString Template::render( Context *c )
{
  QString result = m_nodeList.render( c );

  if ( m_nodeList.error() != NoError ) {
    setError( m_nodeList.error(), m_nodeList.errorString() );
  }
  return result;
}

NodeList Template::nodeList() const
{
  return m_nodeList;
}

void Template::setNodeList( const NodeList &list )
{
  m_nodeList = list;
}

void Template::setError( Error type, const QString &message )
{
  m_error = type;
  m_errorString = message;
}

Error Template::error()
{
  return m_error;
}

QString Template::errorString()
{
  return m_errorString;
}

