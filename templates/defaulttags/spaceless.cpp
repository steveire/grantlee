/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "spaceless.h"

#include <QtCore/QRegularExpression>

#include "parser.h"
#include "util.h"

SpacelessNodeFactory::SpacelessNodeFactory() {}

Node *SpacelessNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  Q_UNUSED(tagContent)
  auto n = new SpacelessNode(p);
  auto list = p->parse(n, QStringLiteral("endspaceless"));
  n->setList(list);
  p->removeNextToken();
  return n;
}

SpacelessNode::SpacelessNode(QObject *parent) : Node(parent) {}

void SpacelessNode::setList(const NodeList &nodeList) { m_nodeList = nodeList; }

QString SpacelessNode::stripSpacesBetweenTags(const QString &input)
{
  auto stripped = input;

  static const QRegularExpression re(QStringLiteral(">\\s+<"));
  stripped.replace(re, QStringLiteral("><"));
  return stripped;
}

void SpacelessNode::render(OutputStream *stream, Context *c) const
{
  QString output;
  QTextStream textStream(&output);
  auto temp = stream->clone(&textStream);
  m_nodeList.render(temp.data(), c);
  (*stream) << markSafe(stripSpacesBetweenTags(output.trimmed()));
}
