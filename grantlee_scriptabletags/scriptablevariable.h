
#ifndef SCRIPTABLE_VARIABLE_H
#define SCRIPTABLE_VARIABLE_H

#include <QObject>
#include <QScriptValue>

#include "variable.h"

class QScriptContext;

class ScriptableContext;

using namespace Grantlee;

QScriptValue ScriptableVariableConstructor(QScriptContext *context,
                                        QScriptEngine *engine);

class ScriptableVariable : public QObject
{
  Q_OBJECT
public:
  ScriptableVariable(QObject *parent = 0);

  void setContent(const QString &content);

public slots:
  QVariant resolve(ScriptableContext *c);

  bool isTrue(ScriptableContext *c);

private:
  Variable m_variable;
};

#endif
