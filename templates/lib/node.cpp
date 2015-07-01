/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "node.h"

#include "metaenumvariable_p.h"
#include "nodebuiltins_p.h"
#include "template.h"
#include "util.h"

#include <QtCore/QRegularExpressionMatchIterator>

using namespace Grantlee;

namespace Grantlee
{

class NodePrivate
{
  NodePrivate( Node *node )
      : q_ptr( node ) {

  }
  Q_DECLARE_PUBLIC( Node )
  Node * const q_ptr;
};

class AbstractNodeFactoryPrivate
{
  AbstractNodeFactoryPrivate( AbstractNodeFactory *factory )
      : q_ptr( factory ) {

#if defined(Q_CC_MSVC)
// MSVC doesn't like static string concatenations like L"foo" "bar", as
// results from QStringLiteral, so use QLatin1String here instead.
#define STRING_LITERAL QLatin1String
#else
#define STRING_LITERAL QStringLiteral
#endif
  smartSplitRe = QRegularExpression(
                  STRING_LITERAL( "("                   // match
                    "(?:[^\\s\\\'\\\"]*"                // things that are not whitespace or escaped quote chars
                      "(?:"                             // followed by
                        "(?:\""                         // Either a quote starting with "
                          "(?:[^\"\\\\]|\\\\.)*\""      // followed by anything that is not the end of the quote
                        "|\'"                           // Or a quote starting with '
                          "(?:[^\'\\\\]|\\\\.)*\'"      // followed by anything that is not the end of the quote
                        ")"                             // (End either)
                        "[^\\s\'\"]*"                   // To the start of the next such fragment
                      ")+"                              // Perform multiple matches of the above.
                    ")"                                 // End of quoted string handling.
                    "|\\S+"                             // Apart from quoted strings, match non-whitespace fragments also
                  ")"                                   // End match
                ) );

#undef STRING_LITERAL
  }

  Q_DECLARE_PUBLIC( AbstractNodeFactory )
  AbstractNodeFactory * const q_ptr;

public:
  QRegularExpression smartSplitRe;
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

void Node::streamValueInContext( OutputStream *stream, const QVariant& input, Context* c ) const
{
  Grantlee::SafeString inputString;
  if ( input.type() == QVariant::List ) {
    inputString = toString( input.toList() );
  } else if ( input.userType() == qMetaTypeId<MetaEnumVariable>() ) {
    const MetaEnumVariable mev = input.value<MetaEnumVariable>();
    if ( mev.value >= 0 )
      ( *stream ) << QString::number( mev.value );
  } else {
    inputString = getSafeString( input );
  }
  if ( c->autoEscape() && !inputString.isSafe() )
    inputString.setNeedsEscape( true );

  ( *stream ) << inputString;
}

TemplateImpl* Node::containerTemplate() const
{
  QObject *_parent = parent();
  TemplateImpl *ti = qobject_cast<TemplateImpl *>( _parent );
  while ( _parent && !ti ) {
    _parent = _parent->parent();
    ti = qobject_cast<TemplateImpl *>( _parent );
  }
  Q_ASSERT( ti );
  return ti;
}


NodeList::NodeList()
    : QList<Grantlee::Node*>(), m_containsNonText( false )
{

}

NodeList::NodeList( const NodeList &list )
    : QList<Grantlee::Node*>( list )
{
  m_containsNonText = list.m_containsNonText;
}

NodeList::NodeList( const QList<Grantlee::Node *> &list )
    : QList<Grantlee::Node*>( list )
{
  Q_FOREACH( Grantlee::Node *node, list ) {
    TextNode *textNode = qobject_cast<TextNode *>( node );
    if ( !textNode ) {
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
  if ( !m_containsNonText ) {
    TextNode *textNode = qobject_cast<TextNode *>( node );
    if ( !textNode )
      m_containsNonText = true;
  }

  QList<Grantlee::Node *>::append( node );
}

void NodeList::append( QList<Grantlee::Node*> nodeList )
{
  if ( !m_containsNonText ) {
    Q_FOREACH( Grantlee::Node *node, nodeList ) {
      TextNode *textNode = qobject_cast<TextNode *>( node );
      if ( !textNode ) {
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

void NodeList::render( OutputStream *stream, Context *c ) const
{
  for ( int i = 0; i < this->size(); ++i ) {
    this->at( i )->render( stream, c );
  }

  return;
}

AbstractNodeFactory::AbstractNodeFactory( QObject *parent )
    : QObject( parent ), d_ptr( new AbstractNodeFactoryPrivate( this ) )
{

}

AbstractNodeFactory::~AbstractNodeFactory()
{
  delete d_ptr;
}

QList< FilterExpression > AbstractNodeFactory::getFilterExpressionList( const QStringList &list, Parser *p ) const
{
  QList<FilterExpression> fes;
  QListIterator<QString> it( list );
  while ( it.hasNext() ) {
    const QString varString = it.next();
    fes << FilterExpression( varString, p );
  }
  return fes;
}

QStringList AbstractNodeFactory::smartSplit( const QString &str ) const
{
  Q_D(const AbstractNodeFactory);
  QStringList l;

  QRegularExpression re = d->smartSplitRe;
  QRegularExpressionMatchIterator i = re.globalMatch(str);
  while (i.hasNext()) {
    QRegularExpressionMatch match = i.next();
    l.append(match.captured());
  }

  return l;
}
