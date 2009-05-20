/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "templatetag.h"

#include <QStringList>

#include "parser.h"
#include "grantlee.h"

#include <QDebug>

static QHash<QString, QString> s_map;


TemplateTagNodeFactory::TemplateTagNodeFactory()
{
  s_map.insert("openblock", BLOCK_TAG_START);
  s_map.insert("closeblock", BLOCK_TAG_END);
  s_map.insert("openvariable", VARIABLE_TAG_START);
  s_map.insert("closevariable", VARIABLE_TAG_END);
  s_map.insert("openbrace", SINGLE_BRACE_START);
  s_map.insert("closebrace", SINGLE_BRACE_END);
  s_map.insert("opencomment", COMMENT_TAG_START);
  s_map.insert("closecomment", COMMENT_TAG_END);
}


Node* TemplateTagNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);
  expr.takeAt(0);
  if (expr.size() <= 0)
  {
    error(TagSyntaxError, "'templatetag' statement takes one argument");
    return 0;
  }

  QString name = expr.at(0);

  if (!TemplateTagNode::isKeyword(name))
  {
    error(TagSyntaxError, "Not a template tag");
    return 0;
  }

  return new TemplateTagNode(name, parent);
}


TemplateTagNode::TemplateTagNode(const QString &name, QObject *parent)
  : Node(parent)
{
  m_name = name;
}

bool TemplateTagNode::isKeyword(const QString &name)
{
  return s_map.contains(name);
}

QString TemplateTagNode::render(Context *c)
{
  return s_map.value(m_name);
}