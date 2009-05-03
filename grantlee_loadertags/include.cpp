/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "include.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"
#include "template.h"

#include <QDebug>

IncludeNodeFactory::IncludeNodeFactory()
{

}

Node* IncludeNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  QString includeName = expr.at(1);
  int size = includeName.size();

  if (includeName.at(0) == QChar('\"') && includeName.at(size-1) == QChar('\"'))
  {
    return new ConstantIncludeNode(includeName.mid(1, size-2));
  }
  return new IncludeNode(includeName);
}

IncludeNode::IncludeNode(const QString &name)
{
  m_filterExpression = FilterExpression(name);
}

QString IncludeNode::render(Context *c)
{
  QString filename = m_filterExpression.resolve(c).toString();

  TemplateLoader *loader = TemplateLoader::instance();

  Template t = loader->loadByName(filename);

  return t.render(c);
}

ConstantIncludeNode::ConstantIncludeNode(const QString &name)
{
  m_name = name;
}

QString ConstantIncludeNode::render(Context *c)
{
  TemplateLoader *loader = TemplateLoader::instance();

  Template t = loader->loadByName(m_name);

  return t.render(c);
}
