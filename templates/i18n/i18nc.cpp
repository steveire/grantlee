/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#include "i18nc.h"

#include <QtCore/QStringList>
#include <QtPlugin>

#include "abstractlocalizer.h"
#include "parser.h"
#include "template.h"
#include "engine.h"
#include "exception.h"

#include <QtCore/QDebug>
#include <util.h>
#include <complex>

I18ncNodeFactory::I18ncNodeFactory()
{

}

Node* I18ncNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() < 3 )
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc tag takes at least two arguments" ) );

  QString contextText = expr.at( 1 );

  if ( !( contextText.startsWith( QLatin1Char( '"' ) ) && contextText.endsWith( QLatin1Char( '"' ) ) )
       && !( contextText.startsWith( QLatin1Char( '\'' ) ) && contextText.endsWith( QLatin1Char( '\'' ) ) ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc tag first argument must be a static string." ) );
  }
  contextText = contextText.mid( 1, contextText.size() - 2 );

  QString sourceText = expr.at( 2 );

  if ( !( sourceText.startsWith( QLatin1Char( '"' ) ) && sourceText.endsWith( QLatin1Char( '"' ) ) )
       && !( sourceText.startsWith( QLatin1Char( '\'' ) ) && sourceText.endsWith( QLatin1Char( '\'' ) ) ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc tag second argument must be a static string." ) );
  }
  sourceText = sourceText.mid( 1, sourceText.size() - 2 );

  QList<FilterExpression> feList;
  for ( int i = 3; i < expr.size(); ++i ) {
    feList.append( FilterExpression( expr.at( i ), p ) );
  }

  return new I18ncNode( sourceText, contextText, feList );
}


I18ncVarNodeFactory::I18ncVarNodeFactory()
{

}

Grantlee::Node* I18ncVarNodeFactory::getNode( const QString& tagContent, Parser* p ) const
{
  QStringList expr = smartSplit( tagContent );

  if ( expr.size() < 5 )
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc_var tag takes at least four arguments" ) );

  QString contextText = expr.at( 1 );

  if ( !( contextText.startsWith( QLatin1Char( '"' ) ) && contextText.endsWith( QLatin1Char( '"' ) ) )
       && !( contextText.startsWith( QLatin1Char( '\'' ) ) && contextText.endsWith( QLatin1Char( '\'' ) ) ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc_var tag first argument must be a static string." ) );
  }
  contextText = contextText.mid( 1, contextText.size() - 2 );

  QString sourceText = expr.at( 2 );

  if ( !( sourceText.startsWith( QLatin1Char( '"' ) ) && sourceText.endsWith( QLatin1Char( '"' ) ) )
       && !( sourceText.startsWith( QLatin1Char( '\'' ) ) && sourceText.endsWith( QLatin1Char( '\'' ) ) ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Error: i18nc_var tag second argument must be a static string." ) );
  }
  sourceText = sourceText.mid( 1, sourceText.size() - 2 );

  QList<FilterExpression> feList;
  for ( int i = 3; i < expr.size() - 2; ++i ) {
    feList.append( FilterExpression( expr.at( i ), p ) );
  }

  QString resultName = expr.last();

  return new I18ncVarNode( sourceText, contextText, feList, resultName );
}


I18ncNode::I18ncNode( const QString &sourceText, const QString &context, const QList<Grantlee::FilterExpression>& feList, QObject* parent )
  : Node( parent ), m_sourceText( sourceText ), m_context( context ), m_filterExpressionList( feList )
{

}

void I18ncNode::render( OutputStream* stream, Context* c )
{
  QVariantList args;
  foreach( const FilterExpression &fe, m_filterExpressionList )
    args.append( fe.resolve( c ) );
  QString resultString = c->localizer()->localizeContextString( m_sourceText, m_context, args );

  streamValueInContext( stream, resultString, c );
}

I18ncVarNode::I18ncVarNode( const QString &sourceText, const QString &context, const QList<Grantlee::FilterExpression>& feList, const QString &resultName, QObject* parent )
  : Node( parent ), m_sourceText( sourceText ), m_context( context ), m_filterExpressionList( feList ), m_resultName( resultName )
{

}

void I18ncVarNode::render( OutputStream* stream, Context* c )
{
  Q_UNUSED( stream )
  QVariantList args;
  foreach( const FilterExpression &fe, m_filterExpressionList )
    args.append( fe.resolve( c ) );
  QString resultString = c->localizer()->localizeContextString( m_sourceText, m_context, args );

  c->insert( m_resultName, resultString );
}
