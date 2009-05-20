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

Node* IncludeNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  QString includeName = expr.at(1);
  int size = includeName.size();

  if ((includeName.startsWith("\"") && includeName.endsWith("\""))
    || ( includeName.startsWith("'") && includeName.endsWith("'") ))
  {
    return new ConstantIncludeNode(includeName.mid(1, size-2), parent);
  }
  return new IncludeNode(includeName, parent);
}

IncludeNode::IncludeNode(const QString &name, QObject *parent)
  : Node(parent)
{
  m_filterExpression = FilterExpression(name);
}

QString IncludeNode::render(Context *c)
{
  QString filename = m_filterExpression.resolve(c).toString();

  TemplateLoader *loader = TemplateLoader::instance();

  Template *t = loader->getTemplate(this);
  connect(t, SIGNAL(error(int, QString)), SIGNAL(error(int, QString)));
  loader->loadByName(t, filename);

  return t->render(c);
}

ConstantIncludeNode::ConstantIncludeNode(const QString &name, QObject *parent)
  : Node(parent)
{
  m_name = name;
}

QString ConstantIncludeNode::render(Context *c)
{
  TemplateLoader *loader = TemplateLoader::instance();

  Template* t = loader->getTemplate(this);

  loader->loadByName(t, m_name);

  return t->render(c);
}
