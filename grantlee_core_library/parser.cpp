/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "parser.h"

#include <QPluginLoader>

#include "interfaces/taglibraryinterface.h"
#include "grantlee.h"
#include "filter.h"

using namespace Grantlee;

namespace Grantlee
{

class ParserPrivate
{
public:
  ParserPrivate(Parser *parser, const QList<Token> &tokenList, const QStringList &pluginDirs)
    : q_ptr(parser),
    m_tokenList(tokenList),
    m_pluginDirs(pluginDirs)
  {

  }

  NodeList extendNodeList(NodeList list, Node *node);

  QList<Token> m_tokenList;
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
  QHash<QString, Filter*> m_filters;
  QHash<QString, TagLibraryInterface*> m_tags;
  NodeList m_nodeList;
  QStringList m_pluginDirs;

  QStringList m_libraryPaths;

  Q_DECLARE_PUBLIC(Parser);
  Parser *q_ptr;
};

}

Parser::Parser(const QList<Token> &tokenList, const QStringList &pluginDirs, QObject *parent)
  : QObject(parent), d_ptr(new ParserPrivate(this, tokenList, pluginDirs))
{
  Q_D(Parser);

  loadLib("grantlee_defaulttags_library");
  loadLib("grantlee_loadertags_library");
  loadLib("grantlee_defaultfilters_library");
}

Parser::~Parser()
{
  qDeleteAll(d_ptr->m_nodeFactories);
  delete d_ptr;
}

void Parser::loadLib(const QString &name)
{
  Q_D(Parser);

  int pluginIndex = 0;
  QString libFileName;

  QObject *plugin;
  while (d->m_pluginDirs.size() > pluginIndex)
  {
    libFileName = d->m_pluginDirs.at(pluginIndex++) + "lib" + name + ".so";
    QPluginLoader loader( libFileName );

    plugin = loader.instance();
    if (plugin)
      break;
  }
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

  QHashIterator<QString, Filter*> filterIter(tagLibrary->filters());
  while (filterIter.hasNext())
  {
    filterIter.next();
    Filter *f = filterIter.value();
    f->setParent(this->parent());
    d->m_filters[filterIter.key()] = f;
  }
  delete tagLibrary;
}

NodeList ParserPrivate::extendNodeList(NodeList list, Node *node)
{
  list.append(node);
  return list;
}

void Parser::skipPast(const QString &tag)
{
  while (hasNextToken())
  {
    Token token = nextToken();
    if (token.tokenType == BlockToken && token.content.trimmed() == tag )
      return;
  }
  // Error. Unclosed tag
}

Filter *Parser::getFilter(const QString &name) const
{
  Q_D(const Parser);
  return d->m_filters.value(name);
}

void Parser::emitError(int err, const QString & message)
{
  error(err, message);
}

NodeList Parser::parse(QObject *parent)
{
  return parse(QStringList(), parent);
}

NodeList Parser::parse(const QStringList &stopAt, QObject *parent)
{
  Q_D(Parser);
  NodeList nodeList;

  while (hasNextToken())
  {
    Token token = nextToken();
    if (token.tokenType == TextToken)
    {
      nodeList = d->extendNodeList(nodeList, new TextNode(token.content, parent));
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
        return NodeList();
      }
      FilterExpression filterExpression(token.content, this);
      if (filterExpression.error() != NoError)
      {
        emit error(filterExpression.error(), "unknown filter error");
        return NodeList();
      }
      nodeList = d->extendNodeList(nodeList, new VariableNode(filterExpression, parent));
    } else if (token.tokenType == BlockToken)
    {
      if (stopAt.contains(token.content))
      {
        // put the token back.
        prependToken(token);
        return nodeList;
      }

      QStringList tagContents = token.content.split(" ");
      if (tagContents.size() == 0)
      {
        QString message;
        if (hasNextToken())
          message = QString("Empty block tag before \"%1\"").arg(nextToken().content);
        else
          message = QString("Empty block tag at end of input.");
        emit error(EmptyBlockTagError, message);
        return NodeList();
      }
      QString command = tagContents.at(0);
      AbstractNodeFactory *nodeFactory = d->m_nodeFactories[command];

      // unknown tag.
      if (!nodeFactory)
      {
        emit error(InvalidBlockTagError, QString("Unknown tag \"%1\"").arg(command));
        continue;
      }

      connect(nodeFactory, SIGNAL(error(int, const QString &)), SLOT(errorSlot(int, const QString &)));

      // TODO: Make getNode take a Token instead?
      Node *n = nodeFactory->getNode(token.content, this, parent);

      if (!n)
      {
        return NodeList();
      }

      nodeList = d->extendNodeList(nodeList, n);
    }

  }

  if (stopAt.size() > 0)
    error(UnclosedBlockTagError, QString("Unclosed tag in template. Expected one of: %1").arg(stopAt.join(" ")));

  return nodeList;

}

void Parser::errorSlot(int type, const QString &message )
{
  error(type, message);
}

bool Parser::hasNextToken() const
{
  Q_D(const Parser);
  return d->m_tokenList.size() > 0;
}

Token Parser::nextToken()
{
  Q_D(Parser);
  return d->m_tokenList.takeAt(0);
}

void Parser::prependToken(const Token &token)
{
  Q_D(Parser);
  d->m_tokenList.prepend(token);
}

