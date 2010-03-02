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
#include "template_p.h"

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "engine.h"
#include "exception.h"

using namespace Grantlee;

NodeList TemplatePrivate::compileString( const QString &str )
{
  Q_Q( TemplateImpl );
  Lexer l( str );
  Parser p( l.tokenize(), q );

  return p.parse( q );
}

TemplateImpl::TemplateImpl( Engine const *engine, QObject *parent )
    : QObject( parent ), d_ptr( new TemplatePrivate( engine, this ) )
{
  Q_D( Template );
  d->m_settingsToken = reinterpret_cast<qint64>( this );
}

TemplateImpl::~TemplateImpl()
{
  delete d_ptr;
}

void TemplateImpl::setContent( const QString &templateString )
{
  Q_D( Template );
  if ( templateString.isEmpty() )
    return;

  try {
    d->m_nodeList = d->compileString( templateString );
  } catch ( Grantlee::Exception &e ) {
    d->setError( e.errorCode(), e.what() );
  }
}

QString TemplateImpl::render( Context *c )
{
  Q_D( Template );

  QString result;
  try {
    result = d->m_nodeList.render( c );
  } catch ( Grantlee::Exception &e ) {
    d->setError( e.errorCode(), e.what() );
    return QString();
  }

  return result;
}

NodeList TemplateImpl::nodeList() const
{
  Q_D( const Template );
  return d->m_nodeList;
}

void TemplateImpl::setNodeList( const NodeList &list )
{
  Q_D( Template );
  d->m_nodeList = list;
}

void TemplatePrivate::setError( Error type, const QString &message )
{
  m_error = type;
  m_errorString = message;
}

Error TemplateImpl::error()
{
  Q_D( Template );
  return d->m_error;
}

QString TemplateImpl::errorString()
{
  Q_D( Template );
  return d->m_errorString;
}

Engine const * TemplateImpl::engine() const
{
  Q_D( const Template );
  return d->m_engine.data();
}

#include "template.moc"

