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

#include "scriptablefilterexpression.h"

#include <QtScript/QScriptEngine>

#include "parser.h"
#include "scriptablecontext.h"
#include "util.h"

using namespace Grantlee;

Q_SCRIPT_DECLARE_QMETAOBJECT( ScriptableFilterExpression, QObject* )

QScriptValue ScriptableFilterExpressionConstructor( QScriptContext *context,
    QScriptEngine *engine )
{
  ScriptableFilterExpression *object = new ScriptableFilterExpression( engine );

  QObject *parserObj = context->argument( 1 ).toQObject();
  Parser *p = qobject_cast<Parser*>( parserObj );

  object->init( context->argument( 0 ).toString(), p );

  return engine->newQObject( object );
}


ScriptableFilterExpression::ScriptableFilterExpression( QObject *parent )
    : QObject( parent )
{

}

void ScriptableFilterExpression::init( const QString& content, Grantlee::Parser* parser )
{
  m_filterExpression = FilterExpression( content, parser );
}


QVariant ScriptableFilterExpression::resolve( ScriptableContext* c )
{
  return m_filterExpression.resolve( c->context() );
}

bool ScriptableFilterExpression::isTrue( ScriptableContext* c )
{
  return m_filterExpression.isTrue( c->context() );
}

bool ScriptableFilterExpression::equals( ScriptableFilterExpression* other, ScriptableContext *scriptableC )
{
  Context *c = scriptableC->context();
  return Grantlee::equals( m_filterExpression.resolve( c ), other->m_filterExpression.resolve( c ) );
}

