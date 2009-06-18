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

#include "templatetag.h"

#include <QStringList>

#include "parser.h"
#include "grantlee.h"


static QHash<QString, QString> s_map;


TemplateTagNodeFactory::TemplateTagNodeFactory()
{
  s_map.insert( "openblock", BLOCK_TAG_START );
  s_map.insert( "closeblock", BLOCK_TAG_END );
  s_map.insert( "openvariable", VARIABLE_TAG_START );
  s_map.insert( "closevariable", VARIABLE_TAG_END );
  s_map.insert( "openbrace", SINGLE_BRACE_START );
  s_map.insert( "closebrace", SINGLE_BRACE_END );
  s_map.insert( "opencomment", COMMENT_TAG_START );
  s_map.insert( "closecomment", COMMENT_TAG_END );
}


Node* TemplateTagNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );
  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    setError( TagSyntaxError, "'templatetag' statement takes one argument" );
    return 0;
  }

  QString name = expr.at( 0 );

  if ( !TemplateTagNode::isKeyword( name ) ) {
    setError( TagSyntaxError, "Not a template tag" );
    return 0;
  }

  return new TemplateTagNode( name );
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
  return s_map.value( m_name );
}

