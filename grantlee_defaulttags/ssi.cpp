/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "ssi.h"

#include <QStringList>
#include <QFile>

#include "parser.h"
#include "template.h"

#include <QDebug>


SsiNodeFactory::SsiNodeFactory()
{

}

Node* SsiNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  int exprSize = expr.size();

  bool parse = false;

  if ((exprSize != 3) && (exprSize != 2))
  {
    error(TagSyntaxError, "ssi tag takes one argument");
    return 0;
  }

  if (exprSize == 3)
  {
    if (expr.at(2) == "parsed")
    {
      parse = true;
    }
  }

  QString filename = expr.at(1);

  return new SsiNode(filename, parse, parent);
}

SsiNode::SsiNode(const QString &filename, bool parse, QObject *parent)
  : Node(parent), m_filename(filename), m_parse(parse)
{
}

QString SsiNode::render(Context *c)
{
  QFile file(m_filename);

  if ( !file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    return QString();

  QTextStream in(&file);
  QString content;
  while (!in.atEnd()) {
      content += in.readLine();
  }

  if (m_parse)
  {
    TemplateLoader *tl = TemplateLoader::instance();
    Template *t = tl->getTemplate();
    t->setContent(content);
    return t->render(c);
  }
  return content;

}

