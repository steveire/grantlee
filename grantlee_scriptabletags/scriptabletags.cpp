
#include "scriptabletags.h"

#include <QtPlugin>
#include <QFile>

#include <QScriptEngine>

#include "scriptablenode.h"
#include "scriptablevariable.h"
#include "scriptablefilterexpression.h"
#include "scriptablefilter.h"
#include "scriptabletemplate.h"
#include "scriptablesafestring.h"

#include "token.h"

Q_DECLARE_METATYPE(Token)

QScriptValue tokenToScriptValue(QScriptEngine *engine, const Token &t)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("tokenType", t.tokenType);
  obj.setProperty("content", t.content);
  return obj;
}

void tokenFromScriptValue(const QScriptValue &obj, Token &t)
{
  t.tokenType = obj.property("tokenType").toInt32();
  t.content = obj.property("content").toString();
}

// TODO: create conversions to/from SafeString, create a mark_safe function.

ScriptableTagLibrary::ScriptableTagLibrary(QObject *parent)
  : QObject(parent), m_scriptEngine(0)
{
  m_scriptEngine = new QScriptEngine(this);

  qScriptRegisterMetaType(m_scriptEngine, tokenToScriptValue, tokenFromScriptValue);
  qScriptRegisterMetaType(m_scriptEngine, nodeToScriptValue, nodeFromScriptValue);
//   qScriptRegisterMetaType(m_scriptEngine.data(), tokenToScriptValue, tokenFromScriptValue);
//   qScriptRegisterMetaType(m_scriptEngine.data(), nodeToScriptValue, nodeFromScriptValue);

  // Make Node new-able
  QScriptValue nodeCtor = m_scriptEngine->newFunction(ScriptableNodeConstructor);
  QScriptValue nodeMetaObject = m_scriptEngine->newQMetaObject(&ScriptableNode::staticMetaObject, nodeCtor);
  m_scriptEngine->globalObject().setProperty("Node", nodeMetaObject);

  // Make Variable new-able
  QScriptValue variableCtor = m_scriptEngine->newFunction(ScriptableVariableConstructor);
  QScriptValue variableMetaObject = m_scriptEngine->newQMetaObject(&VariableNode::staticMetaObject, variableCtor);
  m_scriptEngine->globalObject().setProperty("Variable", variableMetaObject);

  // Make FilterExpression new-able
  QScriptValue filterExpressionCtor = m_scriptEngine->newFunction(ScriptableFilterExpressionConstructor);
  QScriptValue filterExpressionMetaObject = m_scriptEngine->newQMetaObject(&ScriptableFilterExpression::staticMetaObject, filterExpressionCtor);
  m_scriptEngine->globalObject().setProperty("FilterExpression", filterExpressionMetaObject);

  // Make Template new-able
  QScriptValue templateCtor = m_scriptEngine->newFunction(ScriptableTemplateConstructor);
  QScriptValue templateMetaObject = m_scriptEngine->newQMetaObject(&ScriptableTemplate::staticMetaObject, templateCtor);
  m_scriptEngine->globalObject().setProperty("Template", templateMetaObject);

  // Create a global Library object
  QScriptValue libraryObject = m_scriptEngine->newQObject(this);
  m_scriptEngine->globalObject().setProperty("Library", libraryObject);

  // Create a global AbstractNodeFactory object to make smartSplit available.
  ScriptableNodeFactory *nodeFactory = new ScriptableNodeFactory(this);
  QScriptValue nodeFactoryObject = m_scriptEngine->newQObject(nodeFactory);
  m_scriptEngine->globalObject().setProperty("AbstractNodeFactory", nodeFactoryObject);

  // Make mark_safe a globally available object.
  QScriptValue markSafeFunctionObject = m_scriptEngine->newFunction(markSafeFunction);
  m_scriptEngine->globalObject().setProperty("mark_safe", markSafeFunctionObject);

}

bool ScriptableTagLibrary::evaluateScript(const QString &name)
{
  QFile scriptFile(name);

  if ( !scriptFile.exists() || !scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QString fileContent = scriptFile.readAll();

  scriptFile.close();

  m_scriptEngine->evaluate(fileContent);

  if (m_scriptEngine->hasUncaughtException())
  {
    return false;
  }
  return true;
}

QHash<QString, AbstractNodeFactory*> ScriptableTagLibrary::nodeFactories(const QString &name)
{
  m_factoryNames.clear();
  m_nodeFactories.clear();
  QHash<QString, AbstractNodeFactory*> h;

  if (!evaluateScript(name))
  {
    return h;
  }

  return getFactories();
}

QHash< QString, Filter* > ScriptableTagLibrary::filters(const QString& name)
{
  m_filterNames.clear();
  m_filters.clear();
  QHash<QString, Filter*> filters;

  if (!evaluateScript(name))
  {
    return filters;
  }

  return getFilters();
}

QHash<QString, AbstractNodeFactory*> ScriptableTagLibrary::getFactories()
{
  QHash<QString, AbstractNodeFactory*> factories;
  QListIterator<QString> it(m_factoryNames);
  while (it.hasNext())
  {
    QString factoryName = it.next();

    QScriptValue factoryObject = m_scriptEngine->globalObject().property(factoryName);

    factoryObject.call(factoryObject);
    QString tagName = factoryObject.property("tagName").toString();

    ScriptableNodeFactory *snf = new ScriptableNodeFactory();
    snf->setEngine(m_scriptEngine);
    snf->setFactory(factoryObject);

    factories.insert(tagName, snf);
  }

  return factories;
}

QHash<QString, Filter*> ScriptableTagLibrary::getFilters()
{
  QHash<QString, Filter*> filters;

  QListIterator<QString> it(m_filterNames);
  while (it.hasNext())
  {
    QScriptValue filterObject = m_scriptEngine->globalObject().property(it.next());
    filterObject.call(QScriptValue());
    QString filterName = filterObject.property("filterName").toString();
    ScriptableFilter *filter = new ScriptableFilter(filterObject, m_scriptEngine);
    filters.insert(filterName, filter);
  }

  return filters;
}

void ScriptableTagLibrary::addFactory(const QString &factoryName)
{
  m_factoryNames << factoryName;
}

void ScriptableTagLibrary::addFilter(const QString &filterName)
{
  m_filterNames << filterName;
}

Q_EXPORT_PLUGIN2(grantlee_scriptabletags_library, ScriptableTagLibrary);

#include "scriptabletags.moc"



