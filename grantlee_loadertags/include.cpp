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

#include "include.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"
#include "template.h"
#include "engine.h"

#include <QDebug>
#include <util_p.h>

IncludeNodeFactory::IncludeNodeFactory()
{

}

Node* IncludeNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );

  QString includeName = expr.at( 1 );
  int size = includeName.size();

  if (( includeName.startsWith( "\"" ) && includeName.endsWith( "\"" ) )
      || ( includeName.startsWith( "'" ) && includeName.endsWith( "'" ) ) ) {
    return new ConstantIncludeNode( includeName.mid( 1, size - 2 ) );
  }
  return new IncludeNode( FilterExpression( includeName, p ) );
}

IncludeNode::IncludeNode( const FilterExpression &fe, QObject *parent )
    : Node( parent ), m_filterExpression( fe )
{
}

QString IncludeNode::render( Context *c )
{
  QString filename = Util::getSafeString( m_filterExpression.resolve( c ) ).rawString();

  Engine *engine = Engine::instance();
  qint64 settingsToken = parent()->property("settingsToken").toULongLong();

  Template *t = engine->loadByName( filename, this, settingsToken );

  if ( !t )
    return QString();

  return t->render( c );
}

ConstantIncludeNode::ConstantIncludeNode( const QString &name, QObject *parent )
    : Node( parent )
{
  m_name = name;
}

QString ConstantIncludeNode::render( Context *c )
{
  Engine *engine = Engine::instance();
  qint64 settingsToken = parent()->property("settingsToken").toULongLong();
  Template *t = engine->loadByName( m_name, this, settingsToken );

  if ( !t )
    return QString();

  return t->render( c );
}

