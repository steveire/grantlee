/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "autoescape.h"

#include <QStringList>
#include <QFile>

#include "parser.h"
#include "template.h"

#include <QDebug>


AutoescapeNodeFactory::AutoescapeNodeFactory()
{

}

Node* AutoescapeNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  if (expr.size() != 2)
  {
    error(TagSyntaxError, "autoescape takes two arguments.");
    return 0;
  }

  QString strState = expr.at(1);
  int state;
  if (strState == "on")
    state = AutoescapeNode::On;
  else if (strState == "off")
    state = AutoescapeNode::On;
  else
  {
    error(TagSyntaxError, "argument must be 'on' or 'off'");
    return 0;
  }

  NodeList list = p->parse(QStringList() << "endautoescape", parent );
  p->nextToken();

  return new AutoescapeNode(state, list, parent);
}

AutoescapeNode::AutoescapeNode(int state, NodeList list, QObject *parent)
  : Node(parent), m_state(state), m_list(list)
{
}

QString AutoescapeNode::render(Context *c)
{
  if (On == m_state)
  {
//     return marksafe m_list.render(c);
  }
  return m_list.render(c);
}

