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

#include "mediafinder.h"

#include <QUrl>

#include "parser.h"

#include "exception.h"

#include "util_p.h"
#include "engine.h"


MediaFinderNodeFactory::MediaFinderNodeFactory()
{

}

Grantlee::Node* MediaFinderNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() <= 1 ) {
    throw Grantlee::Exception( TagSyntaxError, "'media_finder' tag requires at least one argument" );
  }
  expr.takeAt( 0 );

  return new MediaFinderNode( getFilterExpressionList( expr, p ), p );
}

MediaFinderNode::MediaFinderNode( QList<FilterExpression> mediaExpressionList, QObject* parent )
    : Node( parent ), m_mediaExpressionList( mediaExpressionList )
{
}

void MediaFinderNode::render( OutputStream *stream, Context* c )
{
  TemplateImpl *t = containerTemplate();
  Engine const *engine = t->engine();

  foreach( const FilterExpression &fe, m_mediaExpressionList ) {
    if ( fe.isTrue( c ) ) {
        QString uri = QUrl::fromLocalFile( engine->mediaUri( Util::getSafeString( fe.resolve( c ) ) ) ).toString();
      if ( uri.isEmpty() )
        continue;
      c->addExternalMedia( uri );
      ( *stream ) << uri;
      return;
    }
  }
}
