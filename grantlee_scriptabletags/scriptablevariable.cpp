
#include "scriptablevariable.h"

#include <QScriptEngine>

#include "scriptablecontext.h"

Q_SCRIPT_DECLARE_QMETAOBJECT( ScriptableVariable, QObject* )

QScriptValue ScriptableVariableConstructor( QScriptContext *context,
    QScriptEngine *engine )
{
  // TODO: Decide what the parent should be;
  // It should be the owning scriptableNode. I think I can get that from the scriptContext.

  QObject *parent = 0;
  ScriptableVariable *object = new ScriptableVariable( parent );
  object->setContent( context->argument( 0 ).toString() );

  return engine->newQObject( object );
}

ScriptableVariable::ScriptableVariable( QObject *parent )
    : QObject( parent )
{

}

void ScriptableVariable::setContent( const QString& content )
{
  m_variable = Variable( content );
}

QVariant ScriptableVariable::resolve( ScriptableContext* c )
{
  return m_variable.resolve( c->context() );
}


bool ScriptableVariable::isTrue( ScriptableContext* c )
{
  return m_variable.isTrue( c->context() );
}


