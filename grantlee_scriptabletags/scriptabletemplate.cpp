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

#include "scriptabletemplate.h"

#include <QScriptContext>
#include <QScriptEngine>

#include "engine.h"
#include "context.h"
#include "node.h"
#include "scriptablecontext.h"

Q_DECLARE_METATYPE( Engine* )


QScriptValue ScriptableTemplateConstructor( QScriptContext *context,
    QScriptEngine *engine )
{
  QString content = context->argument( 0 ).toString();
  QString name = context->argument( 1 ).toString();
  QObject *parent = context->argument( 2 ).toQObject();
  Engine *templateEngine = engine->property("templateEngine").value<Engine *>();

  if ( !templateEngine )
    return QScriptValue();

  Template t = templateEngine->newTemplate( content, name );

  ScriptableTemplate *object = new ScriptableTemplate( t, parent );
  return engine->newQObject( object );
}

ScriptableTemplate::ScriptableTemplate( Grantlee::Template t, QObject* parent )
    : QObject( parent ), m_template( t )
{

}

QString ScriptableTemplate::render( ScriptableContext* c )
{
  return m_template->render( c->context() );
}

QObjectList ScriptableTemplate::nodeList() const
{
  NodeList nodeList = m_template->nodeList();
  QObjectList objList;

  QListIterator<Node *> it( nodeList );
  while ( it.hasNext() ) {
    objList << it.next();
  }
  return objList;
}

void ScriptableTemplate::setNodeList( const QObjectList& list )
{
  NodeList nodeList;

  QListIterator<QObject *> it( list );

  while ( it.hasNext() ) {
    Node *n = qobject_cast<Node*>( it.next() );
    if ( n ) {
      nodeList << n;
    }
  }
  m_template->setNodeList( nodeList );
}
