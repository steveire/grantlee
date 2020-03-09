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

#include "firstof.h"

#include "../lib/exception.h"
#include "parser.h"
#include "util.h"

FirstOfNodeFactory::FirstOfNodeFactory() {}

Node *FirstOfNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  const auto tagName = expr.takeAt(0);

  if (expr.isEmpty()) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("%1 expects at least one argument").arg(tagName));
  }

  return new FirstOfNode(getFilterExpressionList(expr, p), p);
}

FirstOfNode::FirstOfNode(const QList<FilterExpression> &list, QObject *parent)
    : Node(parent), m_variableList(list)
{
}

void FirstOfNode::render(OutputStream *stream, Context *c) const
{
  Q_FOREACH (const FilterExpression &fe, m_variableList) {
    if (fe.isTrue(c)) {
      fe.resolve(stream, c);
      return;
    }
  }
}
