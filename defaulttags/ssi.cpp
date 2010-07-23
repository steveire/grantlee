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

#include "ssi.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "parser.h"
#include "template.h"
#include "engine.h"

#include "exception.h"


SsiNodeFactory::SsiNodeFactory()
{

}

Node* SsiNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );

  int exprSize = expr.size();

  bool parse = false;

  if ( ( exprSize != 3 ) && ( exprSize != 2 ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "ssi tag takes one argument" ) );
  }

  if ( exprSize == 3 ) {
    if ( expr.at( 2 ) == QLatin1String( "parsed" ) ) {
      parse = true;
    }
  }

  QString filename = expr.at( 1 );

  return new SsiNode( filename, parse, p );
}

SsiNode::SsiNode( const QString &filename, bool parse, QObject *parent )
    : Node( parent ), m_filename( filename ), m_parse( parse )
{
}

void SsiNode::render( OutputStream *stream, Context *c )
{
  QFile file( m_filename );

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  QTextStream in( &file );
  if ( m_parse ) {
    QString content;
    while ( !in.atEnd() ) {
      content += in.readLine();
    }
    TemplateImpl *ti = containerTemplate();
    Template t = ti->engine()->newTemplate( content, m_filename );
    t->render( stream, c );
  }

  ( *stream ) << &in;
}

