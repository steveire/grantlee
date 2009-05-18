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

Node* CommentNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  Q_UNUSED(tagContent);

  p->skipPast("endcomment");

  return new CommentNode(parent);
}

CommentNode::CommentNode(QObject *parent)
  : Node(parent)
{

}

QString CommentNode::render(Context *c)
{
  Q_UNUSED(c);
  // TODO: figure out if this should be a null or an empty QString.
//   return QString();
  return QString("");
}

