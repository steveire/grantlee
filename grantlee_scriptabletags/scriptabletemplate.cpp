
#include "scriptabletemplate.h"

#include <QScriptContext>
#include <QScriptEngine>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "node.h"
#include "scriptablecontext.h"


QScriptValue ScriptableTemplateConstructor( QScriptContext *context,
    QScriptEngine *engine )
{
  QString content = context->argument( 0 ).toString();
  QObject *parent = context->argument( 1 ).toQObject();

  Template *t = new Template( engine );

  t->setContent( content );

  ScriptableTemplate *object = new ScriptableTemplate( t, parent );
  object->setContent( content );
  return engine->newQObject( object );
}

ScriptableTemplate::ScriptableTemplate( Grantlee::Template* t, QObject* parent )
    : QObject( parent ), m_template( t )
{

}

void ScriptableTemplate::setContent( const QString& content )
{
  m_template->setContent( content );
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

QObjectList ScriptableTemplate::getNodesByType( const char* className )
{
  NodeList nodeList = m_template->getNodesByType( className );
  QObjectList objList;

  QListIterator<Node *> it( nodeList );
  while ( it.hasNext() ) {
    objList << it.next();
  }
  return objList;
}












