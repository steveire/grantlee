/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "scriptablenode.h"

#include <QtScript/QScriptEngine>

#include "context.h"
#include "scriptablecontext.h"
#include "scriptableparser.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"

Q_DECLARE_METATYPE( Engine* )

QScriptValue nodeToScriptValue( QScriptEngine *engine, Node* const &node )
{
  return engine->newQObject( node );
}

void nodeFromScriptValue( const QScriptValue &object, Node* &out )
{
  out = qobject_cast<Node*>( object.toQObject() );
}


Q_SCRIPT_DECLARE_QMETAOBJECT( ScriptableNode, Node* )

QScriptValue ScriptableNodeConstructor( QScriptContext *context,
                                        QScriptEngine *engine )
{
  QString scriptableNodeName = context->argument( 0 ).toString();
  QScriptValue concreteNode = engine->globalObject().property( scriptableNodeName );

  QScriptValueList args;
  // First is the node type
  for ( int i = 1; i < context->argumentCount(); ++i ) {
    args << context->argument( i );
  }

  concreteNode.call( concreteNode, args );

  QScriptValue renderMethod = concreteNode.property( QLatin1String( "render" ) );

  ScriptableNode *object = new ScriptableNode( engine );
  object->setObjectName( scriptableNodeName );
  object->setScriptEngine( engine );
  object->init( concreteNode, renderMethod );
  return engine->newQObject( object );
}


ScriptableNode::ScriptableNode( QObject* parent ): Node( parent ), m_scriptEngine( 0 )
{
}


void ScriptableNode::setScriptEngine( QScriptEngine *engine )
{
  m_scriptEngine = engine;
}

void ScriptableNode::init( const QScriptValue &concreteNode,
                           const QScriptValue &renderMethod )
{
  m_concreteNode = concreteNode;
  m_renderMethod = renderMethod;
}

void ScriptableNode::render( OutputStream *stream, Context *c )
{
  ScriptableContext sc( c );
  QScriptValue contextObject = m_scriptEngine->newQObject( &sc );

  QScriptValueList args;
  args << contextObject;

  // Call the render method in the context of the concreteNode
  QScriptValue value = m_renderMethod.call( m_concreteNode, args );

  if ( value.isValid() && !value.isUndefined() )
    ( *stream ) << value.toString();
}

ScriptableNodeFactory::ScriptableNodeFactory( QObject* parent )
    : AbstractNodeFactory( parent ), m_scriptEngine( 0 )
{

}

void ScriptableNodeFactory::setScriptEngine( QScriptEngine *engine )
{
  m_scriptEngine = engine;
}

void ScriptableNodeFactory::setEngine(Engine* engine)
{
  m_scriptEngine->setProperty( "templateEngine", QVariant::fromValue( engine ) );
}

void ScriptableNodeFactory::setFactory( QScriptValue factoryMethod )
{
  m_factoryMethod = factoryMethod;
}

Node* ScriptableNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  if ( m_scriptEngine->hasUncaughtException() ) {
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( QChar::fromLatin1( ' ' ) ) );
  }
  ScriptableParser *sp = new ScriptableParser( p, m_scriptEngine );
  QScriptValue parserObject = m_scriptEngine->newQObject( sp );

  QScriptValueList args;
  args << tagContent;
  args << parserObject;

  QScriptValue factory = m_factoryMethod;

  QScriptValue scriptNode = factory.call( factory, args );
  if ( m_scriptEngine->hasUncaughtException() )
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( QChar::fromLatin1( ' ' ) ) );

  Node* node = qscriptvalue_cast<Node*>( scriptNode );
  node->setParent( p );
  return node;
}

QScriptEngine* ScriptableNode::engine()
{
  return m_scriptEngine;
}

void ScriptableNode::setNodeList( const QString &name, QObjectList objectList )
{
  QScriptValue objectListArray = m_scriptEngine->newArray( objectList.size() );

  for ( int i = 0; i < objectList.size(); ++i ) {
    objectListArray.setProperty( i, m_scriptEngine->newQObject( objectList.at( i ) ) );
  }
  m_concreteNode.setProperty( name, objectListArray );
}


#include "scriptablenode.moc"
