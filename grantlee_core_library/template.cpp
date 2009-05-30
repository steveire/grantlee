/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "template.h"

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "grantlee.h"

using namespace Grantlee;

NodeList Template::compileString(const QString &str)
{
  Lexer l(str);
  Parser p( l.tokenize(), m_pluginDirs, this );
  connect(&p, SIGNAL(error(int, QString)), SIGNAL(error(int, QString)));
  return p.parse(this);
}

Template::Template( const QStringList &pluginDirs, QObject *parent ) : QObject(parent)
{
  m_pluginDirs = pluginDirs;
}

void Template::setContent(const QString &templateString)
{
  if (!templateString.isEmpty())
    m_nodeList = compileString( templateString );
}

NodeList Template::getNodesByType(const char* className)
{
  return m_nodeList.getNodesByType(className);
}

QString Template::render(Context *c)
{
  QString ret = m_nodeList.render(c);
  if (ret.isNull())
  {
    error(TagSyntaxError, "someError");
    return QString();
  }
  return ret;
}

NodeList Template::nodeList() const
{
  return m_nodeList;
}

void Template::setNodeList(const NodeList &list)
{
  m_nodeList = list;
}
