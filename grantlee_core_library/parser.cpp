/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "parser.h"

#include <QPluginLoader>

#include "interfaces/taglibraryinterface.h"

#include <QDebug>

using namespace Grantlee;

class ParserPrivate
{
public:
  ParserPrivate(Parser *parser)
    : q_ptr(parser)
  {

  }

  NodeList extendNodeList(NodeList list, Node *node);

  QList<Token> m_tokenList;
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QSet<FilterExpression> m_filters;
  QHash<QString, TagLibraryInterface*> m_tags;
  NodeList m_nodeList;
  QStringList m_pluginDirs;

  QStringList m_libraryPaths;

  Q_DECLARE_PUBLIC(Parser);
  Parser *q_ptr;
};

Parser::Parser(QList<Token> tokenList, QStringList pluginDirs)
  : d_ptr(new ParserPrivate(this))
{
  Q_D(Parser);
  d->m_tokenList = tokenList;
  d->m_pluginDirs = pluginDirs;

  loadLib("grantlee_defaulttags_library");
  loadLib("grantlee_loadertags_library");

}

void Parser::loadLib(const QString &name)
{
  Q_D(Parser);

  // TODO: Loop over m_pluginDirs and raise an error if the library is not found.
  QString libFileName = d->m_pluginDirs.at(0) + "lib" + name + ".so";

  QPluginLoader loader( libFileName );
  QObject *plugin = loader.instance();
  if (!plugin)
    return;

  TagLibraryInterface *tagLibrary = qobject_cast<TagLibraryInterface*>(plugin);
  if (!tagLibrary)
    return;

  QHashIterator<QString, AbstractNodeFactory*> i(tagLibrary->nodeFactories());
  while (i.hasNext())
  {
    i.next();
    d->m_nodeFactories[i.key()] = i.value();
  }

}

NodeList ParserPrivate::extendNodeList(NodeList list, Node *node)
{
  list.append(node);
  return list;
}

NodeList Parser::parse(QStringList stopAt)
{
  Q_D(Parser);
  NodeList nodeList;

  while (hasNextToken())
  {
    Token token = nextToken();
    if (token.tokenType == TextToken)
    {
      nodeList = d->extendNodeList(nodeList, new TextNode(token.content));
    } else if (token.tokenType == VariableToken)
    {
      if (token.content.isEmpty())
      {
        // Error. Empty variable
        QString message;
        if (hasNextToken())
          message = QString("Empty variable before \"%1\"").arg(nextToken().content);
        else
          message = QString("Empty variable at end of input.");
        emit error(EmptyVariableError, message);
      }
      FilterExpression filter(token.content, this);
      nodeList = d->extendNodeList(nodeList, new VariableNode(filter));
    } else if (token.tokenType == BlockToken)
    {
      QStringList tagContents = token.content.split(" ", QString::SkipEmptyParts);
      if (tagContents.size() == 0)
      {
        QString message;
        if (hasNextToken())
          message = QString("Empty block tag before \"%1\"").arg(nextToken().content);
        else
          message = QString("Empty block tag at end of input.");
        emit error(EmptyBlockTagError, message);
      }
      // TODO Should this be taken out, or should it be sent to the node?
      QString command = tagContents.at(0);
      if (stopAt.contains(command))
      {
        // put the token back.
        prependToken(token);
        return nodeList;
      }
      AbstractNodeFactory *nodeFactory = d->m_nodeFactories[command];

      // unknown tag.
      if (!nodeFactory)
      {
        emit error(InvalidBlockTagError, QString("Unknown tag \"%1\"").arg(command));
        continue;
      }

      connect(nodeFactory, SIGNAL(error(int, QString)), SIGNAL(error(int, QString)));
      Node *n = nodeFactory->getNode(tagContents.join(" "), this);
      nodeFactory->disconnect(SIGNAL(error(int, QString)), this, SIGNAL(error(int, QString)));

      nodeList = d->extendNodeList(nodeList, n);
    }

  }

  if (stopAt.size() > 0)
    error(UnclosedBlockTagError, QString("Unclosed tag in template. Expected one of: %1").arg(stopAt.join(" ")));

  return nodeList;

}

bool Parser::hasNextToken()
{
  Q_D(Parser);
  return d->m_tokenList.size() > 0;
}

Token Parser::nextToken()
{
  Q_D(Parser);
  return d->m_tokenList.takeAt(0);
}

void Parser::prependToken(Token token)
{
  Q_D(Parser);
  d->m_tokenList.prepend(token);
}

