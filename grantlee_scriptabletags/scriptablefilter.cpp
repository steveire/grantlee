
#include "scriptablefilter.h"

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
    args << m_scriptEngine->newVariant(input);
  }
  args << QScriptValue(argument);
  QScriptValue filterObject = m_filterObject;
  return filterObject.call(QScriptValue(), args).toString();
}
