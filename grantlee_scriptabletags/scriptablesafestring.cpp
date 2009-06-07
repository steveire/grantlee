
#include "scriptablesafestring.h"

#include <QtScript/QScriptEngine>

#include "util_p.h"

QScriptValue markSafeFunction( QScriptContext *context,
                               QScriptEngine *engine )
{
  QScriptValue inputValue = context->argument( 0 );
  if ( inputValue.isQObject() ) {
    QObject *obj = inputValue.toQObject();
    ScriptableSafeString *ssObj = qobject_cast<ScriptableSafeString*>( obj );
    if ( !ssObj )
      return engine->nullValue();

    ssObj->setSafety( true );
    return engine->newQObject( ssObj );

  } else if ( inputValue.isString() ) {
    QString str = inputValue.toString();
    ScriptableSafeString *ssObj = new ScriptableSafeString( engine );
    ssObj->setContent( Util::markSafe( str ) );
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
  return m_safeString.rawString();
}
