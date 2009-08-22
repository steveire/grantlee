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

#include "node.h"

#include "util_p.h"

using namespace Grantlee;

namespace Grantlee
{

class NodePrivate
{
  NodePrivate( Node *node )
      : q_ptr( node ), m_error( NoError ) {

  }
  Q_DECLARE_PUBLIC( Node )
  Node *q_ptr;

  Error m_error;
  QString m_errorString;

};

class AbstractNodeFactoryPrivate
{
  AbstractNodeFactoryPrivate( AbstractNodeFactory *factory )
      : q_ptr( factory ), m_error( NoError ) {

  }

  mutable Error m_error;
  mutable QString m_errorString;

  Q_DECLARE_PUBLIC( AbstractNodeFactory )
  AbstractNodeFactory *q_ptr;
};

}

Node::Node( QObject *parent ) : QObject( parent ),
    d_ptr( new NodePrivate( this ) )
{

}

Node::~Node()
{
  delete d_ptr;
}

QString Node::renderValueInContext( const QVariant& input, Context* c )
{
  Grantlee::SafeString inputString = Util::getSafeString( input );
  if ( c->autoescape() && !inputString.isSafe() || inputString.needsEscape() )
    return Util::escape( inputString );
  return inputString.rawString();
}

void Node::setError( Error type, const QString &message )
{
  Q_D( Node );
  d->m_error = type;
  d->m_errorString = message;
}

Error Node::error() const
{
  Q_D( const Node );
  return d->m_error;
}

QString Node::errorString() const
{
  Q_D( const Node );
  return d->m_errorString;
}

NodeList Node::getNodesByType( const char * className )
{
  return NodeList();
}

NodeList::NodeList()
    : QList<Grantlee::Node*>(), m_error( NoError ), m_containsNonText(false)
{

}

NodeList NodeList::getNodesByType( const char * className )
{
  NodeList list;
  QListIterator<Grantlee::Node *> it( *this );
  while ( it.hasNext() ) {
    Grantlee::Node *n = it.next();
    if ( n->metaObject()->className() == className )
      list << n;
    list << n->getNodesByType( className );
  }
  return list;
}

NodeList::NodeList( const NodeList &list )
    : QList<Grantlee::Node*>( list )
{
  m_error = list.error();
  m_errorString = list.errorString();
  m_containsNonText = list.m_containsNonText;
}

NodeList::NodeList( const QList<Grantlee::Node *> &list )
    : QList<Grantlee::Node*>( list ), m_error( NoError )
{
  foreach(Grantlee::Node *node, list)
  {
    TextNode *textNode = qobject_cast<TextNode *>( node );
    if (!textNode)
    {
      m_containsNonText = true;
      return;
    }
  }
  m_containsNonText = false;
}

NodeList::~NodeList()
{
}

void NodeList::append( Grantlee::Node *node )
{
  if (!m_containsNonText)
  {
    TextNode *textNode = qobject_cast<TextNode *>( node );
    if (!textNode)
      m_containsNonText = true;
  }

  QList<Grantlee::Node *>::append( node );
}

void NodeList::append( QList<Grantlee::Node*> nodeList )
{
  if (!m_containsNonText)
  {
    foreach (Grantlee::Node *node, nodeList)
    {
      TextNode *textNode = qobject_cast<TextNode *>( node );
      if (!textNode)
      {
        m_containsNonText = true;
        break;
      }
    }
  }

  QList<Grantlee::Node *>::append( nodeList );
}

bool NodeList::containsNonText() const
{
  return m_containsNonText;
}

QString NodeList::render( Context *c )
{
  if ( c->isMutating() )
    return mutableRender( c );

  QString ret;
  for ( int i = 0; i < this->size(); ++i ) {
    ret += this->at( i )->render( c );
    if ( this->at( i )->error() != NoError ) {
      setError( this->at( i )->error(), this->at( i )->errorString() );
      break;
    }
  }

  return ret;
}

QString NodeList::mutableRender( Context *c )
{
  QString renderedTemplate;
  QString renderedNode;

  QList<Grantlee::Node*>::iterator it;
  QList<Grantlee::Node*>::iterator first = begin();
  QList<Grantlee::Node*>::iterator last = end();
  for ( it = first; it != last; ++it ) {
    Grantlee::Node *node = *it;
    QString renderedNode = node->render( c );
    renderedTemplate += renderedNode;
    bool isPersistent = node->isPersistent();
    if ( it != first ) {
      Grantlee::Node *lastNode = *( it - 1 );
      TextNode *textNode = qobject_cast<TextNode*>( lastNode );
      if ( textNode && ( !isPersistent || node->isRepeatable() ) ) {
        textNode->appendContent( renderedNode );
      }
      if (*it == *( last - 1 ) )
      {
        break;
      }
      if ( !isPersistent && !lastNode->isPersistent() ) {
        it = erase( it );
        --it;
      }
    }
  }
  return renderedTemplate;
}

void NodeList::setError( Error type, const QString &message )
{
  m_error = type;
  m_errorString = message;
}

Error NodeList::error() const
{
  return m_error;
}

QString NodeList::errorString() const
{
  return m_errorString;
}

AbstractNodeFactory::AbstractNodeFactory( QObject *parent )
    : QObject( parent ), d_ptr( new AbstractNodeFactoryPrivate( this ) )
{

}

AbstractNodeFactory::~AbstractNodeFactory()
{
  delete d_ptr;
}

QList< Variable > AbstractNodeFactory::getVariableList( const QStringList &list ) const
{
  QList<Variable> variables;
  QListIterator<QString> it( list );
  while ( it.hasNext() ) {
    QString varString = it.next();
    variables << Variable( varString );
  }
  return variables;
}

QList< FilterExpression > AbstractNodeFactory::getFilterExpressionList( const QStringList &list, Parser *p ) const
{
  QList<FilterExpression> fes;
  QListIterator<QString> it( list );
  while ( it.hasNext() ) {
    QString varString = it.next();
    fes << FilterExpression( varString, p );
  }
  return fes;
}

QStringList AbstractNodeFactory::smartSplit( const QString &str ) const
{
  QRegExp r( "(\"(?:[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*)\"|\\\'(?:[^\\\'\\\\]*(?:\\\\.[^\\\'\\\\]*)*)\\\'|[^\\s]+)" );

  QStringList l;
  int count = 0;
  int pos = 0;
  while (( pos = r.indexIn( str, pos ) ) != -1 ) {
    ++count;
    pos += r.matchedLength();
    l << r.capturedTexts().at( 0 );
  }

  return l;
}

void AbstractNodeFactory::setError( Error type, const QString &message ) const
{
  Q_D( const AbstractNodeFactory );
  d->m_error = type;
  d->m_errorString = message;
}

Error AbstractNodeFactory::error() const
{
  Q_D( const AbstractNodeFactory );
  return d->m_error;
}

QString AbstractNodeFactory::errorString() const
{
  Q_D( const AbstractNodeFactory );
  return d->m_errorString;
}


TextNode::TextNode( const QString &content, QObject *parent )
    : Node( parent ), m_content( content )
{
}

VariableNode::VariableNode( const FilterExpression &fe, QObject *parent )
    : Node( parent ), m_filterExpression( fe )
{
}

QString VariableNode::render( Context *c )
{
  QVariant v = m_filterExpression.resolve( c );
  if ( !v.isValid() )
    return QString();
  return renderValueInContext( v, c );
}

