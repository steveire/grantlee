
#ifndef SCRIPTABLE_SAFESTRING
#define SCRIPTABLE_SAFESTRING

#include <QObject>
#include <QScriptValue>

#include "safestring.h"

class QScriptContext;

using namespace Grantlee;

QScriptValue markSafeFunction(QScriptContext *context,
                       QScriptEngine *engine);

class ScriptableSafeString : public QObject
{
  Q_OBJECT
public:
  ScriptableSafeString(QObject* parent = 0);

  void setContent(const SafeString &content);
  SafeString wrappedString() const;

public slots:
  bool isSafe() const;
  void setSafety(bool safeness);
  QString rawString();

private:
  SafeString m_safeString;
};


#endif
