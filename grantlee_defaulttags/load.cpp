/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "load.h"

#include <QStringList>
#include "parser.h"

#include "grantlee.h"

#include <QDebug>

LoadNodeFactory::LoadNodeFactory()
{
}

Node* LoadNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  expr.takeAt(0);

  if (expr.size() <= 0 )
  {
    emit error(TagSyntaxError, QString("%1 expects at least one argument").arg("expr.at(0)"));
    return 0;
  }

  QListIterator<QString> i(expr);
  while (i.hasNext())
  {
    QString libName = i.next();
    p->loadLib(libName);
  }

  return new LoadNode(parent);
}


LoadNode::LoadNode(QObject *parent)
  : Node(parent)
{
}

QString LoadNode::render(Context *c)
{
  return QString("");
}

