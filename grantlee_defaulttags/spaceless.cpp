/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "spaceless.h"

#include <QStringList>
#include "parser.h"


#include <QDebug>

SpacelessNodeFactory::SpacelessNodeFactory()
{

}

Node* SpacelessNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  NodeList list = p->parse(QStringList() << "endspaceless", parent);
  p->nextToken();
  return new SpacelessNode(list, parent);
}

SpacelessNode::SpacelessNode(NodeList nodeList, QObject *parent)
  : Node(parent), m_nodeList(nodeList)
{

}

QString SpacelessNode::stripSpacesBetweenTags(const QString& input)
{
  QString stripped = input;

  QRegExp re( ">\\s+<" );
  stripped.replace(re, "><");
  return stripped;
}


QString SpacelessNode::render(Context *c)
{
  return stripSpacesBetweenTags(m_nodeList.render(c).trimmed());
}

