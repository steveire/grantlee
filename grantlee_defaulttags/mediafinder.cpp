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

#include "mediafinder.h"


#include "parser.h"

#include "grantlee.h"

#include "util_p.h"
#include "engine.h"


MediaFinderNodeFactory::MediaFinderNodeFactory()
{

}

Grantlee::Node* MediaFinderNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, "'media_finder' tag requires at least one argument" );
  }

  return new MediaFinderNode( getFilterExpressionList( expr, p ) );
}

MediaFinderNode::MediaFinderNode( QList<FilterExpression> mediaExpressionList, QObject* parent )
    : Node( parent ), m_mediaExpressionList( mediaExpressionList )
{
}

QString MediaFinderNode::render( Context* c )
{
  Grantlee::Engine *engine = Grantlee::Engine::instance();
  foreach( FilterExpression fe, m_mediaExpressionList ) {
    if ( fe.isTrue( c ) ) {
      return engine->mediaUri( Util::getSafeString( fe.resolve( c ) ), parent()->property( "settingsToken" ).toULongLong() );
    }
  }
  return QString();
}

