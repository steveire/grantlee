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

#include "templatetag.h"

#include <QStringList>

#include "parser.h"
#include "grantlee_tags_p.h"
#include "exception.h"


static QHash<QString, QString> s_map;


TemplateTagNodeFactory::TemplateTagNodeFactory()
{
  s_map.insert( "openblock", BLOCK_TAG_START );
  s_map.insert( "closeblock", BLOCK_TAG_END );
  s_map.insert( "openvariable", VARIABLE_TAG_START );
  s_map.insert( "closevariable", VARIABLE_TAG_END );
  s_map.insert( "openbrace", "{" );
  s_map.insert( "closebrace", "}" );
  s_map.insert( "opencomment", COMMENT_TAG_START );
  s_map.insert( "closecomment", COMMENT_TAG_END );
}


Node* TemplateTagNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );
  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, "'templatetag' statement takes one argument" );
  }

  QString name = expr.at( 0 );

  if ( !TemplateTagNode::isKeyword( name ) ) {
    throw Grantlee::Exception( TagSyntaxError, "Not a template tag" );
  }

  return new TemplateTagNode( name, p );
}


TemplateTagNode::TemplateTagNode( const QString &name, QObject *parent )
    : Node( parent )
{
  m_name = name;
}

bool TemplateTagNode::isKeyword( const QString &name )
{
  return s_map.contains( name );
}

QString TemplateTagNode::render( Context *c )
{
  Q_UNUSED( c )
  return s_map.value( m_name );
}

