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

#include "scriptablefilter.h"
#include "scriptablesafestring.h"

#include <QScriptEngine>

ScriptableFilter::ScriptableFilter( QScriptValue filterObject, QScriptEngine *engine )
    : m_filterObject( filterObject ), m_scriptEngine( engine )
{
}

ScriptableFilter::~ScriptableFilter()
{

}

bool ScriptableFilter::isSafe() const
{
  QScriptValue safety = m_filterObject.property( "isSafe" );
  if ( safety.isBool() ) {
    return safety.toBool();
  }
  return false;
}

SafeString ScriptableFilter::doFilter( const QVariant &input, const SafeString& argument, bool autoescape ) const
{
  QScriptValueList args;
  if ( input.type() == QVariant::List ) {
    QVariantList inputList = input.toList();
    QScriptValue array = m_scriptEngine->newArray( inputList.size() );
    for ( int i = 0; i < inputList.size(); ++i ) {
      array.setProperty( i, m_scriptEngine->newVariant( inputList.at( i ) ) );
    }
    args << array;
  } else {
    if ( Util::isSafeString( input ) ) {
      ScriptableSafeString *ssObj = new ScriptableSafeString( m_scriptEngine );
      ssObj->setContent( Util::getSafeString( input ) );
      args << m_scriptEngine->newQObject( ssObj );
    } else {
      args << m_scriptEngine->newVariant( input );
    }
  }

  ScriptableSafeString *ssObj = new ScriptableSafeString( m_scriptEngine );
  ssObj->setContent( argument );
  args << m_scriptEngine->newQObject( ssObj );
  QScriptValue filterObject = m_filterObject;
  QScriptValue returnValue = filterObject.call( QScriptValue(), args );

  if ( returnValue.isString() ) {
    return Util::getSafeString( returnValue.toString() );
  } else if ( returnValue.isQObject() ) {
    QObject *returnedObject = qscriptvalue_cast<QObject *>( returnValue );
    ScriptableSafeString *returnedStringObject = qobject_cast<ScriptableSafeString*>( returnedObject );
    if ( !returnedStringObject )
      return QString();
    SafeString returnedString = returnedStringObject->wrappedString();
    return returnedString;
  }
  return QString();
}

