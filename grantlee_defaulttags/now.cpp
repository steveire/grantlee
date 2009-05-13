/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "now.h"

#include <QStringList>
#include <QDateTime>

#include "parser.h"

#include <QDebug>


NowNodeFactory::NowNodeFactory()
{

}

Node* NowNodeFactory::do_getNode(const QString &tagContent, Parser *p, bool negate)
{
  QStringList expr = tagContent.split("\"", QString::KeepEmptyParts);

  if (expr.size() != 3)
  {
    error(TagSyntaxError, "now tag takes one argument");
    return 0;
  }

  QString formatString = expr.at(1);

  return new NowNode(formatString);
}

Node* NowNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  return do_getNode(tagContent, p, false);
}


NowNode::NowNode(const QString &formatString)
  : m_formatString(formatString)
{
}

QString NowNode::render(Context *c)
{
  return QDateTime::currentDateTime().toString(m_formatString);
}

