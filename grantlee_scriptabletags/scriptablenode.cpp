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

#include "scriptablenode.h"

#include <QtScript/QScriptEngine>

#include "context.h"
#include "scriptablecontext.h"
#include "scriptableparser.h"
#include "parser.h"


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

  QScriptValue renderMethod = concreteNode.property( "render" );

  ScriptableNode *object = new ScriptableNode( engine );
  object->setObjectName( scriptableNodeName );
  object->setEngine( engine );
  object->init( concreteNode, renderMethod );
  return engine->newQObject( object );
}


ScriptableNode::ScriptableNode( QObject* parent ): Node( parent ), m_scriptEngine( 0 )
{
}


void ScriptableNode::setEngine( QScriptEngine *engine )
{
  m_scriptEngine = engine;
}

void ScriptableNode::init( const QScriptValue &concreteNode,
                           const QScriptValue &renderMethod )
{
  m_concreteNode = concreteNode;
  m_renderMethod = renderMethod;
}

QString ScriptableNode::render( Context *c )
{
  ScriptableContext sc( c );
  QScriptValue contextObject = m_scriptEngine->newQObject( &sc );

  QScriptValueList args;
  args << contextObject;

  // Call the render method in the context of the concreteNode
  return m_renderMethod.call( m_concreteNode, args ).toString();
}

ScriptableNodeFactory::ScriptableNodeFactory( QObject* parent )
    : AbstractNodeFactory( parent ), m_scriptEngine( 0 )
{

}

void ScriptableNodeFactory::setEngine( QScriptEngine *engine )
{
  m_scriptEngine = engine;
}

void ScriptableNodeFactory::setFactory( QScriptValue factoryMethod )
{
  m_factoryMethod = factoryMethod;
}

Node* ScriptableNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  if ( m_scriptEngine->hasUncaughtException() ) {
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( " " ) );
  }
  ScriptableParser *sp = new ScriptableParser( p, m_scriptEngine );
  QScriptValue parserObject = m_scriptEngine->newQObject( sp );

  QScriptValueList args;
  args << tagContent;
  args << parserObject;

  QScriptValue factory = m_factoryMethod;

  QScriptValue scriptNode = factory.call( factory, args );
  if (m_scriptEngine->hasUncaughtException())
    throw Grantlee::Exception( TagSyntaxError, m_scriptEngine->uncaughtExceptionBacktrace().join( " " ) );

  Node* node = qscriptvalue_cast<Node*>( scriptNode );
  ScriptableNode *sn = qobject_cast<ScriptableNode *>( node );
  return node;

}

QScriptEngine* ScriptableNode::engine()
{
  return m_scriptEngine;
}

void ScriptableNode::setNodeList( const QString &name, QObjectList objectList )
{
  QScriptValue objectListArray = m_scriptEngine->newArray( objectList.size() );

  for ( int i = 0; i < objectList.size(); ++i )
  {
    objectListArray.setProperty( i, m_scriptEngine->newQObject( objectList.at( i ) ) );
  }
  m_concreteNode.setProperty( name, objectListArray );
}

