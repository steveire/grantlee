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

#include "parser.h"

#include <QPluginLoader>
#include <QFile>

#include "interfaces/taglibraryinterface.h"
#include "grantlee.h"
#include "template.h"
#include "engine.h"
#include "filter.h"

#include "grantlee_version.h"

static const char * __scriptableLibName = "grantlee_scriptabletags_library";

using namespace Grantlee;

namespace Grantlee
{

class ParserPrivate
{
public:
  ParserPrivate( Parser *parser, const QList<Token> &tokenList )
      : q_ptr( parser ),
      m_error( NoError ),
      m_tokenList( tokenList ),
      m_scriptableTagLibrary( 0 ) {

  }

  NodeList extendNodeList( NodeList list, Node *node );

  QList<Token> m_tokenList;
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  TagLibraryInterface *m_scriptableTagLibrary;

  QHash<QString, Filter*> m_filters;
  QHash<QString, TagLibraryInterface*> m_tags;
  NodeList m_nodeList;
  QStringList m_pluginDirs;

  Error m_error;
  QString m_errorString;

  QStringList m_libraryPaths;

  Q_DECLARE_PUBLIC( Parser );
  Parser *q_ptr;
};

}

Parser::Parser( const QList<Token> &tokenList, QObject *parent )
    : QObject( parent ), d_ptr( new ParserPrivate( this, tokenList ) )
{
  Q_D( Parser );

  Engine *tl = Engine::instance();

  d->m_pluginDirs = tl->pluginDirs();

  foreach( const QString libName, tl->defaultLibraries() ) {
    loadLib( libName );
  }

}

Parser::~Parser()
{
  qDeleteAll( d_ptr->m_nodeFactories );
  delete d_ptr;
}


void Parser::setTokens(const QList< Token >& tokenList)
{
  Q_D(Parser);
  d->m_tokenList = tokenList;
}


void Parser::loadLib( const QString &name )
{
  Q_D( Parser );

  int pluginIndex = 0;
  QString libFileName;

  if ( d->m_scriptableTagLibrary ) {
    while ( d->m_pluginDirs.size() > pluginIndex ) {
      libFileName = d->m_pluginDirs.at( pluginIndex++ ) + GRANTLEE_MAJOR_MINOR_VERSION_STRING + "/" + name + ".qs";
      QFile file( libFileName );
      if ( !file.exists() )
        continue;

      QHashIterator<QString, AbstractNodeFactory*> i( d->m_scriptableTagLibrary->nodeFactories( libFileName ) );
      while ( i.hasNext() ) {
        i.next();
        d->m_nodeFactories[i.key()] = i.value();
      }

      QHashIterator<QString, Filter*> filterIter( d->m_scriptableTagLibrary->filters( libFileName ) );
      while ( filterIter.hasNext() ) {
        filterIter.next();
        Filter *f = filterIter.value();
        f->setParent( this->parent() );
        d->m_filters[filterIter.key()] = f;
      }

      return;
    }
  }

  pluginIndex = 0;

  QObject *plugin = 0;
  while ( d->m_pluginDirs.size() > pluginIndex ) {
    libFileName = d->m_pluginDirs.at( pluginIndex++ ) + GRANTLEE_MAJOR_MINOR_VERSION_STRING + "/" + "lib" + name + ".so";
    QFile file( libFileName );
    if ( !file.exists() )
      continue;

    QPluginLoader loader( libFileName );

    plugin = loader.instance();
    if ( plugin )
      break;
  }
  if ( !plugin )
    return;

  TagLibraryInterface *tagLibrary = qobject_cast<TagLibraryInterface*>( plugin );
  if ( !tagLibrary )
    return;

  if ( name == __scriptableLibName ) {
    d->m_scriptableTagLibrary = tagLibrary;
    plugin->setParent( this->parent() );
    return;
  }

  QHashIterator<QString, AbstractNodeFactory*> i( tagLibrary->nodeFactories() );
  while ( i.hasNext() ) {
    i.next();
    d->m_nodeFactories[i.key()] = i.value();
  }

  QHashIterator<QString, Filter*> filterIter( tagLibrary->filters() );
  while ( filterIter.hasNext() ) {
    filterIter.next();
    Filter *f = filterIter.value();
    f->setParent( this->parent() );
    d->m_filters[filterIter.key()] = f;
  }
  delete tagLibrary;
}

NodeList ParserPrivate::extendNodeList( NodeList list, Node *node )
{
  list.append( node );
  return list;
}

void Parser::skipPast( const QString &tag )
{
  while ( hasNextToken() ) {
    Token token = nextToken();
    if ( token.tokenType == BlockToken && token.content.trimmed() == tag )
      return;
  }
  // Error. Unclosed tag
}

Filter *Parser::getFilter( const QString &name ) const
{
  Q_D( const Parser );
  return d->m_filters.value( name );
}

NodeList Parser::parse( QObject *parent )
{
  return parse( QStringList(), parent );
}

NodeList Parser::parse( const QString &stopAt, QObject *parent )
{
  parse( QStringList() << stopAt, parent );
}

NodeList Parser::parse( const QStringList &stopAt, QObject *parent )
{
  Q_D( Parser );
  NodeList nodeList;

  while ( hasNextToken() ) {
    Token token = nextToken();
    if ( token.tokenType == TextToken ) {
      nodeList = d->extendNodeList( nodeList, new TextNode( token.content, parent ) );
    } else if ( token.tokenType == VariableToken ) {
      if ( token.content.isEmpty() ) {
        // Error. Empty variable
        QString message;
        if ( hasNextToken() )
          message = QString( "Empty variable before \"%1\"" ).arg( nextToken().content );
        else
          message = QString( "Empty variable at end of input." );
        setError( EmptyVariableError, message );
        return NodeList();
      }
      FilterExpression filterExpression( token.content, this );
      if ( filterExpression.error() != NoError ) {
        setError( filterExpression.error(), filterExpression.errorString() );
        return NodeList();
      }
      nodeList = d->extendNodeList( nodeList, new VariableNode( filterExpression, parent ) );
    } else if ( token.tokenType == BlockToken ) {
      if ( stopAt.contains( token.content ) ) {
        // put the token back.
        prependToken( token );
        return nodeList;
      }

      QStringList tagContents = token.content.split( " " );
      if ( tagContents.size() == 0 ) {
        QString message;
        if ( hasNextToken() )
          message = QString( "Empty block tag before \"%1\"" ).arg( nextToken().content );
        else
          message = QString( "Empty block tag at end of input." );
        setError( EmptyBlockTagError, message );
        return NodeList();
      }
      QString command = tagContents.at( 0 );
      AbstractNodeFactory *nodeFactory = d->m_nodeFactories[command];

      // unknown tag.
      if ( !nodeFactory ) {
        setError( InvalidBlockTagError, QString( "Unknown tag \"%1\"" ).arg( command ) );
        continue;
      }

      // TODO: Make getNode take a Token instead?
      Node *n = nodeFactory->getNode( token.content, this, parent );

      if ( !n ) {
        setError( TagSyntaxError, QString( "Failed to get node from %1" ).arg( command ) );
        return NodeList();
      }

      if ( NoError != nodeFactory->error() ) {
        setError( nodeFactory->error(), nodeFactory->errorString() );
        return NodeList();
      }

      nodeList = d->extendNodeList( nodeList, n );
    }

  }

  if ( stopAt.size() > 0 )
    setError( UnclosedBlockTagError, QString( "Unclosed tag in template. Expected one of: (%1)" ).arg( stopAt.join( " " ) ) );

  return nodeList;

}

void Parser::setError( Error errorNumber, const QString& message )
{
  Q_D( Parser );
  d->m_error = errorNumber;
  d->m_errorString = message;
}

Error Grantlee::Parser::error() const
{
  Q_D( const Parser );
  return d->m_error;
}

QString Parser::errorString() const
{
  Q_D( const Parser );
  return d->m_errorString;
}

bool Parser::hasNextToken() const
{
  Q_D( const Parser );
  return d->m_tokenList.size() > 0;
}

Token Parser::nextToken()
{
  Q_D( Parser );
  return d->m_tokenList.takeAt( 0 );
}

void Parser::deleteNextToken()
{
  Q_D( Parser );
  if ( !d->m_tokenList.isEmpty() )
    d->m_tokenList.removeAt( 0 );
}

void Parser::prependToken( const Token &token )
{
  Q_D( Parser );
  d->m_tokenList.prepend( token );
}

