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

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "grantlee.h"
#include "engine.h"

using namespace Grantlee;

namespace Grantlee
{

class TemplatePrivate
{
  TemplatePrivate( Template *t )
      : q_ptr(t), m_error( NoError )
  {

  }

  void parse();
  NodeList compileString( const QString &str );
  void setError( Error type, const QString &message );

  qint64 m_settingsToken;
  Error m_error;
  QString m_errorString;
  NodeList m_nodeList;

  Q_DECLARE_PUBLIC(Template);
  Template *q_ptr;
};

}

NodeList TemplatePrivate::compileString( const QString &str )
{
  Q_Q( Template );
  Lexer l( str );
  Parser p( l.tokenize(), q );
  NodeList nodeList = p.parse();

  if ( NoError != p.error() ) {
    setError( p.error(), p.errorString() );
  }
  return nodeList;
}

Template::Template( QObject *parent )
    : QObject( parent ), d_ptr(new TemplatePrivate(this))
{
  Q_D(Template);
  d->m_settingsToken = reinterpret_cast<qint64>(this);
}

Template::~Template()
{
  delete d_ptr;
}

void Template::setContent( const QString &templateString )
{
  Q_D( Template );
  if ( !templateString.isEmpty() )
    d->m_nodeList = d->compileString( templateString );
}

NodeList Template::getNodesByType( const char* className )
{
  Q_D( Template );
  return d->m_nodeList.getNodesByType( className );
}

QString Template::render( Context *c )
{
  Q_D( Template );
  QString result = d->m_nodeList.render( c );

  if ( d->m_nodeList.error() != NoError ) {
    d->setError( d->m_nodeList.error(), d->m_nodeList.errorString() );
  }
  return result;
}

NodeList Template::nodeList() const
{
  Q_D( const Template );
  return d->m_nodeList;
}

void Template::setNodeList( const NodeList &list )
{
  Q_D( Template );
  d->m_nodeList = list;
}


void Template::setSettingsToken( qint64 settingsToken )
{
  Q_D(Template);
  d->m_settingsToken = settingsToken;
}

qint64 Template::settingsToken() const
{
  Q_D(const Template);
  return d->m_settingsToken;
}

void TemplatePrivate::setError( Error type, const QString &message )
{
  m_error = type;
  m_errorString = message;
}

Error Template::error()
{
  Q_D( Template );
  return d->m_error;
}

QString Template::errorString()
{
  Q_D( Template );
  return d->m_errorString;
}

