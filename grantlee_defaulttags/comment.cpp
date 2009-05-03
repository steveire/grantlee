/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "comment.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"

#include <QDebug>

CommentNodeFactory::CommentNodeFactory()
{

}

Node* CommentNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  Q_UNUSED(tagContent);
  Q_UNUSED(p);
  return new CommentNode();
}

CommentNode::CommentNode()
{

}

QString CommentNode::render(Context *c)
{
  Q_UNUSED(c);
  // TODO: figure out if this should be a null or an empty QString.
  return QString();
  //   return QString('');
}