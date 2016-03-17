/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "scriptablesafestring.h"

#include <QtScript/QScriptEngine>

#include "util.h"

QScriptValue markSafeFunction( QScriptContext *context,
                               QScriptEngine *engine )
{
  auto inputValue = context->argument( 0 );
  if ( inputValue.isQObject() ) {
    auto obj = inputValue.toQObject();
    auto ssObj = qobject_cast<ScriptableSafeString*>( obj );
    if ( !ssObj )
      return engine->nullValue();

    ssObj->setSafety( true );
    return engine->newQObject( ssObj );

  } else if ( inputValue.isString() ) {
    auto str = inputValue.toString();
    auto ssObj = new ScriptableSafeString( engine );
    ssObj->setContent( markSafe( str ) );
    return engine->newQObject( ssObj );

  }
  return engine->nullValue();

}

ScriptableSafeString::ScriptableSafeString( QObject* parent ): QObject( parent )
{

}

void ScriptableSafeString::setContent( const Grantlee::SafeString& content )
{
  m_safeString = content;
}

SafeString ScriptableSafeString::wrappedString() const
{
  return m_safeString;
}

bool ScriptableSafeString::isSafe() const
{
  return m_safeString.isSafe();
}

void ScriptableSafeString::setSafety( bool safeness )
{
  m_safeString.setSafety( safeness ? Grantlee::SafeString::IsSafe : Grantlee::SafeString::IsNotSafe );
}

QString ScriptableSafeString::rawString()
{
  return m_safeString;
}
