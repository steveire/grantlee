
#include "scriptablefilterexpression.h"

#include <QtScript/QScriptEngine>

#include "parser.h"
#include "scriptablecontext.h"

Q_SCRIPT_DECLARE_QMETAOBJECT(ScriptableFilterExpression, QObject*)

QScriptValue ScriptableFilterExpressionConstructor(QScriptContext *context,
                                        QScriptEngine *engine)
{
  // TODO: Decide what the parent should be;
  // It should be the owning scriptableNode. I think I can get that from the scriptContext.

  QObject *parent = 0;
  ScriptableFilterExpression *object = new ScriptableFilterExpression(parent);

  QObject *parserObj = context->argument(1).toQObject();
  Parser *p = qobject_cast<Parser*>(parserObj);

  object->init(context->argument(0).toString(), p);

  return engine->newQObject(object);
}


ScriptableFilterExpression::ScriptableFilterExpression(QObject *parent)
  : QObject(parent)
{

}

void ScriptableFilterExpression::init(const QString& content, Grantlee::Parser* parser)
{
  m_filterExpression = FilterExpression(content, parser);
}


QVariant ScriptableFilterExpression::resolve(ScriptableContext* c)
{
  return m_filterExpression.resolve(c->context());
}

bool ScriptableFilterExpression::isTrue(ScriptableContext* c)
{
  return m_filterExpression.isTrue(c->context());
}

