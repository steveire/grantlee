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

#include "template.h"
#include "template_p.h"

#include <QtCore/QDebug>

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "engine.h"
#include "exception.h"

using namespace Grantlee;

NodeList TemplatePrivate::compileString( const QString &str )
{
  Q_Q( TemplateImpl );
  const Lexer l( str );
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
    qWarning() << e.what();
    d->setError( e.errorCode(), e.what() );
  }
}

QString TemplateImpl::render( Context *c )
{
  QString output;
  QTextStream textStream( &output );
  OutputStream outputStream( &textStream );
  render( &outputStream, c );
  return output;
}

OutputStream* TemplateImpl::render( OutputStream *stream, Context *c )
{
  Q_D( Template );

  c->clearExternalMedia();
  try {
    d->m_nodeList.render( stream, c );
  } catch ( Grantlee::Exception &e ) {
    qWarning() << e.what();
    d->setError( e.errorCode(), e.what() );
  }

  return stream;
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

