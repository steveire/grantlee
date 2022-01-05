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

#include "with.h"

#include "../lib/exception.h"
#include "parser.h"

WithNodeFactory::WithNodeFactory() = default;

Node *WithNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);
  std::vector<std::pair<QString, FilterExpression>> namedExpressions;

  if (expr.size() != 4 || expr.at(2) != QStringLiteral("as")) {
    auto newSyntax = false;
    for (int i = 1; i < expr.size(); ++i) {
      const auto parts = expr.at(i).split(QLatin1Char('='));
      if (parts.size() == 2) {
        namedExpressions.push_back(
            {parts.at(0), FilterExpression(parts.at(1), p)});
        newSyntax = true;
      } else {
        newSyntax = false;
        break;
      }
    }

    if (!newSyntax) {
      throw Grantlee::Exception(
          TagSyntaxError,
          QStringLiteral(
              "%1 expected format is 'name=value' or 'value as name'")
              .arg(expr.first()));
    }
  } else {
    namedExpressions.push_back({expr.at(3), FilterExpression(expr.at(1), p)});
  }

  auto n = new WithNode(namedExpressions, p);
  auto nodeList = p->parse(n, QStringLiteral("endwith"));
  n->setNodeList(nodeList);
  p->removeNextToken();

  return n;
}

WithNode::WithNode(
    const std::vector<std::pair<QString, FilterExpression>> &namedExpressions,
    QObject *parent)
    : Node(parent), m_namedExpressions(namedExpressions)
{
}

void WithNode::setNodeList(const NodeList &nodeList) { m_list = nodeList; }

void WithNode::render(OutputStream *stream, Context *c) const
{
  c->push();
  for (const auto &pair : m_namedExpressions) {
    c->insert(pair.first, pair.second.resolve(c));
  }
  m_list.render(stream, c);
  c->pop();
}
