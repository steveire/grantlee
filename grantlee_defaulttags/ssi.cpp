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

#include "ssi.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "parser.h"
#include "template.h"
#include "engine.h"



SsiNodeFactory::SsiNodeFactory()
{

}

Node* SsiNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = tagContent.split( " ", QString::SkipEmptyParts );

  int exprSize = expr.size();

  bool parse = false;

  if ( ( exprSize != 3 ) && ( exprSize != 2 ) ) {
    throw Grantlee::Exception( TagSyntaxError, "ssi tag takes one argument" );
  }

  if ( exprSize == 3 ) {
    if ( expr.at( 2 ) == "parsed" ) {
      parse = true;
    }
  }

  QString filename = expr.at( 1 );

  return new SsiNode( filename, parse );
}

SsiNode::SsiNode( const QString &filename, bool parse, QObject *parent )
    : Node( parent ), m_filename( filename ), m_parse( parse )
{
}

QString SsiNode::render( Context *c )
{
  QFile file( m_filename );

  if ( !file.exists() || !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    return QString();

  QTextStream in( &file );
  QString content;
  while ( !in.atEnd() ) {
    content += in.readLine();
  }

  if ( m_parse ) {
    qint64 settingsToken = parent()->property( "settingsToken" ).toULongLong();
    Template t = Engine::instance()->newTemplate( content, m_filename, settingsToken );
    return t->render( c );
  }
  return content;

}

