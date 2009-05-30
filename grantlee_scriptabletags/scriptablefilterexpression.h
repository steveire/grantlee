
#ifndef SCRIPTABLE_FILTEREXPRESSION_H
#define SCRIPTABLE_FILTEREXPRESSION_H

#include <QObject>

#include <QScriptValue>

#include "filterexpression.h"

class QScriptContext;

class ScriptableContext;

QScriptValue ScriptableFilterExpressionConstructor(QScriptContext *context,
                                        QScriptEngine *engine);


class ScriptableFilterExpression : public QObject
{
  Q_OBJECT
public:
  ScriptableFilterExpression(QObject *parent = 0);

  void init(const QString &content, Parser *parser);

public slots:
  QVariant resolve(ScriptableContext *c);

  bool isTrue(ScriptableContext *c);

  // list? QScriptValueList? Make this a ScriptClass?

private:
  FilterExpression m_filterExpression;
};

#endif
