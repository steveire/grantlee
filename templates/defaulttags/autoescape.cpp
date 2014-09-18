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

#include "autoescape.h"

#include "exception.h"
#include "parser.h"
#include "template.h"
#include "util.h"

#include <QtCore/QFile>

AutoescapeNodeFactory::AutoescapeNodeFactory()
{

}

Node* AutoescapeNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );

  if ( expr.size() != 2 ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "autoescape takes two arguments." ) );
  }

  QString strState = expr.at( 1 );
  int state;
  if ( strState == QLatin1String( "on" ) )
    state = AutoescapeNode::On;
  else if ( strState == QLatin1String( "off" ) )
    state = AutoescapeNode::Off;
  else {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "argument must be 'on' or 'off'" ) );
  }

  AutoescapeNode *n = new AutoescapeNode( state, p );

  NodeList list = p->parse( n, QLatin1String( "endautoescape" ) );
  p->removeNextToken();

  n->setList( list );

  return n;
}

AutoescapeNode::AutoescapeNode( int state, QObject *parent )
    : Node( parent ), m_state( state )
{
}


void AutoescapeNode::setList( NodeList list )
{
  m_list = list;
}

void AutoescapeNode::render( OutputStream *stream, Context *c )
{
  const bool old_setting = c->autoEscape();
  c->setAutoEscape( m_state == On );
  m_list.render( stream, c );
  c->setAutoEscape( old_setting );
}
