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

#include "regroup.h"

#include "../lib/exception.h"
#include "parser.h"
#include "util.h"

RegroupNodeFactory::RegroupNodeFactory() {}

Node *RegroupNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = tagContent.split(QLatin1Char(' '));

  if (expr.size() != 6) {
    throw Grantlee::Exception(
        TagSyntaxError, QStringLiteral("widthratio takes five arguments"));
  }
  FilterExpression target(expr.at(1), p);
  if (expr.at(2) != QStringLiteral("by")) {
    throw Grantlee::Exception(TagSyntaxError,
                              QStringLiteral("second argument must be 'by'"));
  }

  if (expr.at(4) != QStringLiteral("as")) {
    throw Grantlee::Exception(TagSyntaxError,
                              QStringLiteral("fourth argument must be 'as'"));
  }

  FilterExpression expression(
      QStringLiteral("\"") + expr.at(3) + QStringLiteral("\""), p);

  auto name = expr.at(5);

  return new RegroupNode(target, expression, name, p);
}

RegroupNode::RegroupNode(const FilterExpression &target,
                         const FilterExpression &expression,
                         const QString &varName, QObject *parent)
    : Node(parent), m_target(target), m_expression(expression),
      m_varName(varName)
{
}

void RegroupNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  auto objList = m_target.toList(c);
  if (objList.isEmpty()) {
    c->insert(m_varName, QVariantHash());
    return;
  }

  // What's going on?
  //
  // objList is a flat list of objects with a common parameter. For example,
  // Person objects with
  // a name parameter. The list is already sorted.
  // Say the objList contains ["David Beckham", "David Blain", "Keira
  // Nightly"]
  // etc.
  // We want to regroup the list into separate lists of people with the same
  // first name.
  // ie objHash should be: {"David": ["David Beckham", "David Blain"],
  // "Keira":
  // ["Keira Nightly"]}
  //
  // We then insert the objHash into the Context ready for rendering later in
  // a
  // for loop.

  QVariantList contextList;
  const QString keyName = getSafeString(m_expression.resolve(c));
  QListIterator<QVariant> i(objList);
  while (i.hasNext()) {
    const auto var = i.next();
    c->push();
    c->insert(QStringLiteral("var"), var);
    const QString key = getSafeString(
        FilterExpression(QStringLiteral("var.") + keyName, 0).resolve(c));
    c->pop();
    QVariantHash hash;
    if (!contextList.isEmpty()) {
      auto hashVar = contextList.last();
      hash = hashVar.value<QVariantHash>();
    }
    if (!hash.contains(QStringLiteral("grouper"))
        || hash.value(QStringLiteral("grouper")) != key) {
      QVariantHash newHash;
      hash.insert(QStringLiteral("grouper"), key);
      hash.insert(QStringLiteral("list"), QVariantList());
      contextList.append(newHash);
    }

    auto list = hash.value(QStringLiteral("list")).value<QVariantList>();
    list.append(var);
    hash.insert(QStringLiteral("list"), list);
    contextList[contextList.size() - 1] = hash;
  }
  c->insert(m_varName, contextList);
}
