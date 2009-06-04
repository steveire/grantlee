
#include "scriptablefilter.h"
#include "scriptablesafestring.h"

#include <QScriptEngine>

ScriptableFilter::ScriptableFilter(QScriptValue filterObject, QScriptEngine *engine, QObject* parent)
  : Filter(parent), m_filterObject(filterObject), m_scriptEngine(engine)
{
}

ScriptableFilter::~ScriptableFilter()
{

}


SafeString ScriptableFilter::doFilter(const QVariant &input, const SafeString& argument, bool autoescape) const
{
  QScriptValueList args;
  if (input.type() == QVariant::List)
  {
    QVariantList inputList = input.toList();
    QScriptValue array = m_scriptEngine->newArray(inputList.size());
    for (int i = 0; i < inputList.size(); ++i)
    {
      array.setProperty(i, m_scriptEngine->newVariant(inputList.at(i)));
    }
    args << array;
  }
  else
  {
    if (Util::isSafeString(input))
    {
      ScriptableSafeString *ssObj = new ScriptableSafeString(m_scriptEngine);
      ssObj->setContent(Util::getSafeString(input));
      args << m_scriptEngine->newQObject(ssObj);
    } else {
      args << m_scriptEngine->newVariant(input);
    }
  }

  ScriptableSafeString *ssObj = new ScriptableSafeString(m_scriptEngine);
  ssObj->setContent(argument);
  args << m_scriptEngine->newQObject(ssObj);
  QScriptValue filterObject = m_filterObject;
  QScriptValue returnValue = filterObject.call(QScriptValue(), args);

  if (returnValue.isString())
  {
    return Util::getSafeString(returnValue.toString());
  } else if (returnValue.isQObject())
  {
    QObject *returnedObject = qscriptvalue_cast<QObject *>(returnValue);
    ScriptableSafeString *returnedStringObject = qobject_cast<ScriptableSafeString*>(returnedObject);
    if (!returnedStringObject)
      return QString();
    SafeString returnedString = returnedStringObject->wrappedString();
    return returnedString;
  }
  return QString();
}
