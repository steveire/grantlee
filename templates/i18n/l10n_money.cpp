/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "l10n_money.h"

#include "abstractlocalizer.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "template.h"
#include "util.h"

L10nMoneyNodeFactory::L10nMoneyNodeFactory() {}

Node *L10nMoneyNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 2)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: l10n_money tag takes at least one argument"));

  FilterExpression value(expr.at(1), p);

  FilterExpression currency;

  if (expr.size() == 3)
    currency = FilterExpression(expr.at(2), p);

  return new L10nMoneyNode(value, currency);
}

L10nMoneyVarNodeFactory::L10nMoneyVarNodeFactory() {}

Grantlee::Node *L10nMoneyVarNodeFactory::getNode(const QString &tagContent,
                                                 Parser *p) const
{

  auto expr = smartSplit(tagContent);

  if (expr.size() < 4)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: l10n_money tag takes at least three arguments"));

  FilterExpression value(expr.at(1), p);

  FilterExpression currency;

  if (expr.size() == 3)
    currency = FilterExpression(expr.at(2), p);

  auto resultName = expr.last();

  return new L10nMoneyVarNode(value, currency, resultName);
}

L10nMoneyNode::L10nMoneyNode(const FilterExpression &value,
                             const FilterExpression &currency, QObject *parent)
    : Node(parent), m_value(value), m_currency(currency)
{
}

void L10nMoneyNode::render(OutputStream *stream, Context *c) const
{
  auto resultString = c->localizer()->localizeMonetaryValue(
      m_value.resolve(c).value<double>(),
      getSafeString(m_currency.resolve(c)).get());

  streamValueInContext(stream, resultString, c);
}

L10nMoneyVarNode::L10nMoneyVarNode(const FilterExpression &value,
                                   const FilterExpression &currency,
                                   const QString &resultName, QObject *parent)
    : Node(parent), m_value(value), m_currency(currency),
      m_resultName(resultName)
{
}

void L10nMoneyVarNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  auto resultString = c->localizer()->localizeMonetaryValue(
      m_value.resolve(c).value<double>(),
      getSafeString(m_currency.resolve(c)).get());

  c->insert(m_resultName, resultString);
}
