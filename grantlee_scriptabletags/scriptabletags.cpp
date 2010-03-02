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
#include "exception.h"

#include "token.h"

Q_DECLARE_METATYPE( Token )
Q_DECLARE_METATYPE( Engine* )

QScriptValue tokenToScriptValue( QScriptEngine *engine, const Token &t )
{
  QScriptValue obj = engine->newObject();
  obj.setProperty( "tokenType", t.tokenType );
  obj.setProperty( "content", t.content );
  return obj;
}

void tokenFromScriptValue( const QScriptValue &obj, Token &t )
{
  t.tokenType = obj.property( "tokenType" ).toInt32();
  t.content = obj.property( "content" ).toString();
}

ScriptableTagLibrary::ScriptableTagLibrary( QObject *parent )
    : QObject( parent ), m_scriptEngine( 0 )
{
  m_scriptEngine = new QScriptEngine( this );

  qScriptRegisterMetaType( m_scriptEngine, tokenToScriptValue, tokenFromScriptValue );
  qScriptRegisterMetaType( m_scriptEngine, nodeToScriptValue, nodeFromScriptValue );
//   qScriptRegisterMetaType(m_scriptEngine.data(), tokenToScriptValue, tokenFromScriptValue);
//   qScriptRegisterMetaType(m_scriptEngine.data(), nodeToScriptValue, nodeFromScriptValue);

  // Make Node new-able
  QScriptValue nodeCtor = m_scriptEngine->newFunction( ScriptableNodeConstructor );
  QScriptValue nodeMetaObject = m_scriptEngine->newQMetaObject( &ScriptableNode::staticMetaObject, nodeCtor );
  m_scriptEngine->globalObject().setProperty( "Node", nodeMetaObject );

  // Make Variable new-able
  QScriptValue variableCtor = m_scriptEngine->newFunction( ScriptableVariableConstructor );
  QScriptValue variableMetaObject = m_scriptEngine->newQMetaObject( &VariableNode::staticMetaObject, variableCtor );
  m_scriptEngine->globalObject().setProperty( "Variable", variableMetaObject );

  // Make FilterExpression new-able
  QScriptValue filterExpressionCtor = m_scriptEngine->newFunction( ScriptableFilterExpressionConstructor );
  QScriptValue filterExpressionMetaObject = m_scriptEngine->newQMetaObject( &ScriptableFilterExpression::staticMetaObject, filterExpressionCtor );
  m_scriptEngine->globalObject().setProperty( "FilterExpression", filterExpressionMetaObject );

  // Make Template new-able
  QScriptValue templateCtor = m_scriptEngine->newFunction( ScriptableTemplateConstructor );
  QScriptValue templateMetaObject = m_scriptEngine->newQMetaObject( &ScriptableTemplate::staticMetaObject, templateCtor );
  m_scriptEngine->globalObject().setProperty( "Template", templateMetaObject );

  // Create a global Library object
  QScriptValue libraryObject = m_scriptEngine->newQObject( this );
  m_scriptEngine->globalObject().setProperty( "Library", libraryObject );

  // Create a global AbstractNodeFactory object to make smartSplit available.
  ScriptableNodeFactory *nodeFactory = new ScriptableNodeFactory( this );
  QScriptValue nodeFactoryObject = m_scriptEngine->newQObject( nodeFactory );
  m_scriptEngine->globalObject().setProperty( "AbstractNodeFactory", nodeFactoryObject );

  // Make mark_safe a globally available object.
  QScriptValue markSafeFunctionObject = m_scriptEngine->newFunction( markSafeFunction );
  m_scriptEngine->globalObject().setProperty( "mark_safe", markSafeFunctionObject );

}

void ScriptableTagLibrary::setEngine( Engine *engine )
{
  m_scriptEngine->setProperty( "templateEngine", QVariant::fromValue( engine ) );
}

bool ScriptableTagLibrary::evaluateScript( const QString &name )
{
  QFile scriptFile( name );

  if ( !scriptFile.exists() || !scriptFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
    return false;
  }

  QString fileContent = scriptFile.readAll();

  scriptFile.close();

  m_scriptEngine->evaluate( fileContent );

  if ( m_scriptEngine->hasUncaughtException() ) {
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( " " ) );
  }
  return true;
}

QHash<QString, AbstractNodeFactory*> ScriptableTagLibrary::nodeFactories( const QString &name )
{
  m_factoryNames.clear();
  m_nodeFactories.clear();
  QHash<QString, AbstractNodeFactory*> h;

  if ( !evaluateScript( name ) ) {
    return h;
  }

  return getFactories();
}

QHash< QString, Filter* > ScriptableTagLibrary::filters( const QString& name )
{
  m_filterNames.clear();
  m_filters.clear();
  QHash<QString, Filter*> filters;

  if ( !evaluateScript( name ) ) {
    return filters;
  }

  return getFilters();
}

QHash<QString, AbstractNodeFactory*> ScriptableTagLibrary::getFactories()
{
  QHash<QString, AbstractNodeFactory*> factories;
  QHashIterator<QString, QString> it( m_factoryNames );
  while ( it.hasNext() ) {
    it.next();
    QString factoryName = it.value();
    QString tagName = it.key();

    QScriptValue factoryObject = m_scriptEngine->globalObject().property( factoryName );

    ScriptableNodeFactory *snf = new ScriptableNodeFactory();
    snf->setEngine( m_scriptEngine );
    snf->setFactory( factoryObject );

    factories.insert( tagName, snf );
  }

  return factories;
}

QHash<QString, Filter*> ScriptableTagLibrary::getFilters()
{
  QHash<QString, Filter*> filters;

  QListIterator<QString> it( m_filterNames );
  while ( it.hasNext() ) {
    QScriptValue filterObject = m_scriptEngine->globalObject().property( it.next() );
    QString filterName = filterObject.property( "filterName" ).toString();
    ScriptableFilter *filter = new ScriptableFilter( filterObject, m_scriptEngine );
    filters.insert( filterName, filter );
  }
  if ( m_scriptEngine->hasUncaughtException() ) {
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( " " ) );
  }

  return filters;
}

void ScriptableTagLibrary::addFactory( const QString &factoryName, const QString &tagName )
{
  m_factoryNames.insert( tagName, factoryName );
}

void ScriptableTagLibrary::addFilter( const QString &filterName )
{
  m_filterNames << filterName;
}

Q_EXPORT_PLUGIN2( grantlee_scriptabletags_library, ScriptableTagLibrary )

