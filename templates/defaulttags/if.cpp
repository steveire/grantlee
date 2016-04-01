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

#include "if.h"
#include "if_p.h"

#include "../lib/exception.h"
#include "parser.h"

IfNodeFactory::IfNodeFactory() {}

Node *IfNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  QVector<QPair<QSharedPointer<IfToken>, NodeList>> nodelistConditions;

  auto n = new IfNode(p);

  IfParser ip(p, expr);
  auto cond = ip.parse();
  auto nodelist = p->parse(n, QStringList() << QStringLiteral("else")
                                            << QStringLiteral("endif"));
  nodelistConditions.push_back(qMakePair(cond, nodelist));

  auto token = p->takeNextToken();

  if (token.content == QLatin1String("else")) {
    nodelist = p->parse(n, QStringList() << QStringLiteral("endif"));
    nodelistConditions.push_back(qMakePair(nullptr, nodelist));
    p->takeNextToken();
  }

  n->setNodelistConditions(nodelistConditions);

  auto commandName = expr.takeAt(0);
  if (expr.size() <= 0) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("'%1' statement requires at least one argument")
            .arg(commandName));
  }

  return n;
}

IfNode::IfNode(QObject *parent) : Node(parent) {}

void IfNode::setNodelistConditions(
    const QVector<QPair<QSharedPointer<IfToken>, NodeList>>
        &conditionNodelists)
{
  mConditionNodelists = conditionNodelists;
}

void IfNode::render(OutputStream *stream, Context *c) const
{
  // Evaluate the expression. rendering variables with the context as needed.
  // and processing nodes recursively

  Q_FOREACH (auto &pair, mConditionNodelists) {
    bool match = false;
    if (pair.first) {
      try {
        match = Grantlee::variantIsTrue(pair.first->evaluate(c));
      } catch (Grantlee::Exception) {
      }
    } else {
      match = true;
    }
    if (match) {
      pair.second.render(stream, c);
      return;
    }
  }
}
