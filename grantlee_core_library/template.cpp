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
  Parser p( l.tokenize(), this);
  NodeList nodeList = p.parse(this);

  if (NoError != p.error())
  {
    setError(p.error(), p.errorString());
  }
  return nodeList;
}

Template::Template( QObject *parent )
  : QObject(parent),
   m_error(NoError)
{
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
  QString result = m_nodeList.render(c);

  if (m_nodeList.error() != NoError)
  {
    setError(m_nodeList.error(), m_nodeList.errorString());
  }
  return result;
}

NodeList Template::nodeList() const
{
  return m_nodeList;
}

void Template::setNodeList(const NodeList &list)
{
  m_nodeList = list;
}

void Template::setError(Error type, const QString &message)
{
  m_error = type;
  m_errorString = message;
}

Error Template::error()
{
  return m_error;
}

QString Template::errorString()
{
  return m_errorString;
}

