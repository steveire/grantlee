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

#include "parser.h"

#include "engine.h"
#include "exception.h"
#include "filter.h"
#include "grantlee_version.h"
#include "nodebuiltins_p.h"
#include "taglibraryinterface.h"
#include "template.h"
#include "template_p.h"

#include <QtCore/QFile>

using namespace Grantlee;

namespace Grantlee
{

class ParserPrivate
{
public:
  ParserPrivate( Parser *parser, const QList<Token> &tokenList )
      : q_ptr( parser ),
      m_tokenList( tokenList ) {

  }

  NodeList extendNodeList( NodeList list, Node *node );

  /**
    Parses the template to create a Nodelist.
    The given @p parent is the parent of each node in the returned list.
  */
  NodeList parse( QObject *parent, const QStringList &stopAt = QStringList() );

  void openLibrary( TagLibraryInterface * library );
  Q_DECLARE_PUBLIC( Parser )
  Parser * const q_ptr;

  QList<Token> m_tokenList;

  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QHash<QString, Filter::Ptr> m_filters;

  NodeList m_nodeList;
};

}

void ParserPrivate::openLibrary( TagLibraryInterface *library )
{
  Q_Q( Parser );
  QHashIterator<QString, AbstractNodeFactory*> nodeIt( library->nodeFactories() );

  TemplateImpl *ti = qobject_cast<TemplateImpl *>( q->parent() );

  Engine const *cengine = ti->engine();
  if ( !cengine )
    return;
  Engine *engine = const_cast<Engine *>( cengine );

  while ( nodeIt.hasNext() ) {
    nodeIt.next();
    nodeIt.value()->setEngine( engine );
    m_nodeFactories.insert( nodeIt.key(), nodeIt.value() );
  }
  QHashIterator<QString, Filter*> filterIt( library->filters() );
  while ( filterIt.hasNext() ) {
    filterIt.next();
    Filter::Ptr f = Filter::Ptr( filterIt.value() );
    m_filters.insert( filterIt.key(), f );
  }
}

Parser::Parser( const QList<Token> &tokenList, QObject *parent )
    : QObject( parent ), d_ptr( new ParserPrivate( this, tokenList ) )
{
  Q_D( Parser );

  TemplateImpl *ti = qobject_cast<TemplateImpl *>( parent );

  Engine const *cengine = ti->engine();
  if ( !cengine )
    return;
  Engine *engine = const_cast<Engine *>( cengine );
  engine->loadDefaultLibraries();
  Q_FOREACH( const QString &libraryName, engine->defaultLibraries() ) {
    TagLibraryInterface *library = engine->loadLibrary( libraryName );
    if ( !library )
      continue;
    d->openLibrary( library );
  }
}

Parser::~Parser()
{
  // Don't delete filters here because filters must out-live the parser in the
  // filter expressions.
  qDeleteAll( d_ptr->m_nodeFactories );
  delete d_ptr;
}


void Parser::setTokens( const QList< Token >& tokenList )
{
  Q_D( Parser );
  d->m_tokenList = tokenList;
}


void Parser::loadLib( const QString &name )
{
  Q_D( Parser );
  TemplateImpl *ti = qobject_cast<TemplateImpl *>( parent() );
  Engine const *cengine = ti->engine();
  if ( !cengine )
    return;
  Engine *engine = const_cast<Engine *>( cengine );
  TagLibraryInterface *library = engine->loadLibrary( name );
  if ( !library )
    return;
  d->openLibrary( library );
}

NodeList ParserPrivate::extendNodeList( NodeList list, Node *node )
{
  if ( node->mustBeFirst() && list.containsNonText() ) {
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1(
        "Node appeared twice in template: %1" ).arg( QLatin1String( node->metaObject()->className() ) ) );
  }

  list.append( node );
  return list;
}

void Parser::skipPast( const QString &tag )
{
  while ( hasNextToken() ) {
    const Token token = takeNextToken();
    if ( token.tokenType == BlockToken && token.content.trimmed() == tag )
      return;
  }
  throw Grantlee::Exception( UnclosedBlockTagError, QString::fromLatin1( "No closing tag found for %1" ).arg( tag ) );
}

Filter::Ptr Parser::getFilter( const QString &name ) const
{
  Q_D( const Parser );
  if ( !d->m_filters.contains( name ) )
    throw Grantlee::Exception( UnknownFilterError, QString::fromLatin1( "Unknown filter: %1" ).arg( name ) );
  return d->m_filters.value( name );
}

NodeList Parser::parse( Node *parent, const QString &stopAt )
{
  Q_D( Parser );
  return d->parse( parent, QStringList() << stopAt );
}

NodeList Parser::parse( TemplateImpl *parent, const QStringList &stopAt )
{
  Q_D( Parser );
  return d->parse( parent, stopAt );
}

NodeList Parser::parse( Node *parent, const QStringList &stopAt )
{
  Q_D( Parser );
  return d->parse( parent, stopAt );
}

NodeList ParserPrivate::parse( QObject *parent, const QStringList &stopAt )
{
  Q_Q( Parser );
  NodeList nodeList;

  while ( q->hasNextToken() ) {
    const Token token = q->takeNextToken();
    if ( token.tokenType == TextToken ) {
      nodeList = extendNodeList( nodeList, new TextNode( token.content, parent ) );
    } else if ( token.tokenType == VariableToken ) {
      if ( token.content.isEmpty() ) {
        // Error. Empty variable
        QString message;
        message = QString::fromLatin1( "Empty variable before \"%1\", line %2, %3" ).arg( q->takeNextToken().content.left( 20 ) ).arg( token.linenumber ).arg( q->parent()->objectName() );
        throw Grantlee::Exception( EmptyVariableError, message );
      }

      FilterExpression filterExpression;
      try {
        filterExpression = FilterExpression( token.content, q );
      } catch ( Grantlee::Exception e ) {
        throw Grantlee::Exception( e.errorCode(), QString::fromLatin1( "%1, line %2, %3" ).arg( e.what() )
                                                                              .arg( token.linenumber )
                                                                              .arg( q->parent()->objectName() ) );
      }

      nodeList = extendNodeList( nodeList, new VariableNode( filterExpression, parent ) );
    } else if ( token.tokenType == BlockToken ) {
      if ( stopAt.contains( token.content ) ) {
        // put the token back.
        q->prependToken( token );
        return nodeList;
      }

      const QStringList tagContents = token.content.split( QLatin1Char( ' ' ) );
      if ( tagContents.size() == 0 ) {
        QString message;
        if ( q->hasNextToken() )
          message = QString::fromLatin1( "Empty block tag before \"%1\", line %2, %3" ).arg( token.content.left( 20 ) ).arg( token.linenumber ).arg( q->parent()->objectName() );
        else
          message = QLatin1String( "Empty block tag at end of input." );

        throw Grantlee::Exception( EmptyBlockTagError, message );
      }
      const QString command = tagContents.first();
      AbstractNodeFactory *nodeFactory = m_nodeFactories[command];

      // unknown tag.
      if ( !nodeFactory ) {
        throw Grantlee::Exception( InvalidBlockTagError, QString::fromLatin1( "Unknown tag: \"%1\", line %2, %3" ).arg( command ).arg( token.linenumber ).arg( q->parent()->objectName() ) );
      }

      // TODO: Make getNode take a Token instead?
      Node *n;
      try {
        n = nodeFactory->getNode( token.content, q );
      } catch ( Grantlee::Exception e ) {
        throw Grantlee::Exception( e.errorCode(), QString::fromLatin1( "%1, line %2, %3" ).arg( e.what() )
                                                                              .arg( token.linenumber )
                                                                              .arg( q->parent()->objectName() ) );
      }
      if ( !n ) {
        throw Grantlee::Exception( EmptyBlockTagError, QString::fromLatin1( "Failed to get node from %1, line %2, %3" ).arg( command ).arg( token.linenumber ).arg( q->parent()->objectName() ) );
      }

      n->setParent( parent );

      nodeList = extendNodeList( nodeList, n );
    }
  }

  if ( !stopAt.isEmpty() ) {
    const QString message = QString::fromLatin1( "Unclosed tag in template %1. Expected one of: (%2)" ).arg( q->parent()->objectName() ).arg( stopAt.join( QChar::fromLatin1( ' ' ) ) );
    throw Grantlee::Exception( UnclosedBlockTagError, message );
  }

  return nodeList;

}

bool Parser::hasNextToken() const
{
  Q_D( const Parser );
  return !d->m_tokenList.isEmpty();
}

Token Parser::takeNextToken()
{
  Q_D( Parser );
  return d->m_tokenList.takeFirst();
}

void Parser::removeNextToken()
{
  Q_D( Parser );
  if ( !d->m_tokenList.isEmpty() )
    d->m_tokenList.removeFirst();;
}

void Parser::prependToken( const Token &token )
{
  Q_D( Parser );
  d->m_tokenList.prepend( token );
}

#include "parser.moc"

