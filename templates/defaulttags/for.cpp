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

#include "for.h"

#include "../lib/exception.h"
#include "metaenumvariable_p.h"
#include "parser.h"

ForNodeFactory::ForNodeFactory() {}

Node *ForNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 4) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("'for' statements should have at least four words: %1")
            .arg(tagContent));
  }

  expr.takeAt(0);
  QStringList vars;

  int reversed = ForNode::IsNotReversed;
  if (expr.last() == QStringLiteral("reversed")) {
    reversed = ForNode::IsReversed;
    expr.removeLast();
  }

  if (expr.at(expr.size() - 2) != QStringLiteral("in")) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("'for' statements should use the form 'for x in y': %1")
            .arg(tagContent));
  }

  Q_FOREACH (const QString &arg, expr.mid(0, expr.size() - 2)) {
    vars << arg.split(QLatin1Char(','), QString::SkipEmptyParts);
  }

  Q_FOREACH (const QString &var, vars) {
    if (var.isEmpty())
      throw Grantlee::Exception(
          TagSyntaxError,
          QStringLiteral("'for' tag received invalid argument"));
  }

  FilterExpression fe(expr.last(), p);

  auto n = new ForNode(vars, fe, reversed, p);

  auto loopNodes = p->parse(n, QStringList() << QStringLiteral("empty")
                                             << QStringLiteral("endfor"));
  n->setLoopList(loopNodes);

  NodeList emptyNodes;
  if (p->takeNextToken().content == QStringLiteral("empty")) {
    emptyNodes = p->parse(n, QStringLiteral("endfor"));
    n->setEmptyList(emptyNodes);
    // skip past the endfor tag
    p->removeNextToken();
  }

  return n;
}

ForNode::ForNode(const QStringList &loopVars, const FilterExpression &fe,
                 int reversed, QObject *parent)
    : Node(parent), m_loopVars(loopVars), m_filterExpression(fe),
      m_isReversed(reversed)
{
}

void ForNode::setLoopList(const NodeList &loopNodeList)
{
  m_loopNodeList = loopNodeList;
}

void ForNode::setEmptyList(const NodeList &emptyList)
{
  m_emptyNodeList = emptyList;
}

static QString forLoopString()
{
    return QStringLiteral("forloop");
}

static QString parentLoopString()
{
    return QStringLiteral("parentloop");
}

void ForNode::insertLoopVariables(Context *c, int listSize, int i)
{
  auto forloopHash = c->lookup(forLoopString()).value<QVariantHash>();
  // some magic variables injected into the context while rendering.
  forloopHash.insert(QStringLiteral("counter0"), i);
  forloopHash.insert(QStringLiteral("counter"), i + 1);
  forloopHash.insert(QStringLiteral("revcounter"), listSize - i);
  forloopHash.insert(QStringLiteral("revcounter0"), listSize - i - 1);
  forloopHash.insert(QStringLiteral("first"), (i == 0));
  forloopHash.insert(QStringLiteral("last"), (i == listSize - 1));
  c->insert(forLoopString(), forloopHash);
}

void ForNode::renderLoop(OutputStream *stream, Context *c) const
{
  for (auto j = 0; j < m_loopNodeList.size(); j++) {
    m_loopNodeList[j]->render(stream, c);
  }
}

void ForNode::render(OutputStream *stream, Context *c) const
{
  QVariantHash forloopHash;

  auto parentLoopVariant = c->lookup(forLoopString());
  if (parentLoopVariant.isValid()) {
    // This is a nested loop.
    forloopHash = parentLoopVariant.value<QVariantHash>();
    forloopHash.insert(parentLoopString(),
                       parentLoopVariant.value<QVariantHash>());
    c->insert(forLoopString(), forloopHash);
  }

  auto unpack = m_loopVars.size() > 1;

  c->push();

  auto varFE = m_filterExpression.resolve(c);

  if (varFE.userType() == qMetaTypeId<MetaEnumVariable>()) {
    const auto mev = varFE.value<MetaEnumVariable>();

    if (mev.value != -1) {
      c->pop();
      return m_emptyNodeList.render(stream, c);
    }

    QVariantList list;
    for (auto row = 0; row < mev.enumerator.keyCount(); ++row) {
      list << QVariant::fromValue(MetaEnumVariable(mev.enumerator, row));
    }
    varFE = list;
  }

  if (!varFE.canConvert<QVariantList>()) {
    c->pop();
    return m_emptyNodeList.render(stream, c);
  }

  auto iter = varFE.value<QSequentialIterable>();
  const auto listSize = iter.size();

  // If it's an iterable type, iterate, otherwise it's a list of one.
  if (listSize < 1) {
    c->pop();
    return m_emptyNodeList.render(stream, c);
  }

  auto i = 0;
  for (auto it = m_isReversed == IsReversed ? iter.end() - 1 : iter.begin();
       m_isReversed == IsReversed ? it != iter.begin() - 1 : it != iter.end();
       m_isReversed == IsReversed ? --it : ++it) {
    const auto v = *it;
    insertLoopVariables(c, listSize, i);

    if (unpack) {
      if (v.userType() == qMetaTypeId<QVariantList>()) {
        auto vList = v.value<QVariantList>();
        auto varsSize = qMin(m_loopVars.size(), vList.size());
        auto j = 0;
        for (; j < varsSize; ++j) {
          c->insert(m_loopVars.at(j), vList.at(j));
        }
        // If any of the named vars don't have an item in the context,
        // insert an invalid object for them.
        for (; j < m_loopVars.size(); ++j) {
          c->insert(m_loopVars.at(j), QVariant());
        }

      } else {
        // We don't have a hash, but we have to unpack several values
        // from each
        // item
        // in the list. And each item in the list is not itself a list.
        // Probably have a list of objects that we're taking properties
        // from.
        Q_FOREACH (const QString &loopVar, m_loopVars) {
          c->push();
          c->insert(QStringLiteral("var"), v);
          auto v = FilterExpression(QStringLiteral("var.") + loopVar, 0)
                       .resolve(c);
          c->pop();
          c->insert(loopVar, v);
        }
      }
    } else {
      c->insert(m_loopVars[0], v);
    }
    renderLoop(stream, c);
    ++i;
  }
  c->pop();
}
