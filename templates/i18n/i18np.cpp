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

#include "i18np.h"

#include <QtCore/QStringList>

#include "abstractlocalizer.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "template.h"

#include <QtCore/QDebug>
#include <complex>
#include <util.h>

I18npNodeFactory::I18npNodeFactory() {}

Node *I18npNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 3)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18np tag takes at least two arguments"));

  auto sourceText = expr.at(1);

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18np tag first argument must be a static string."));
  }
  sourceText = sourceText.mid(1, sourceText.size() - 2);

  auto pluralText = expr.at(2);

  auto argsStart = 3;
  if (!(pluralText.startsWith(QLatin1Char('"'))
        && pluralText.endsWith(QLatin1Char('"')))
      && !(pluralText.startsWith(QLatin1Char('\''))
           && pluralText.endsWith(QLatin1Char('\'')))) {
    argsStart = 2;
    pluralText = sourceText;
  } else {
    pluralText = pluralText.mid(1, pluralText.size() - 2);
  }

  QList<FilterExpression> feList;
  for (auto i = argsStart; i < expr.size(); ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  return new I18npNode(sourceText, pluralText, feList);
}

I18npVarNodeFactory::I18npVarNodeFactory() {}

Grantlee::Node *I18npVarNodeFactory::getNode(const QString &tagContent,
                                             Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 5)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18np_var tag takes at least four arguments"));

  auto sourceText = expr.at(1);

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18np tag first argument must be a static string."));
  }
  sourceText = sourceText.mid(1, sourceText.size() - 2);

  auto pluralText = expr.at(2);

  auto argsStart = 3;
  if (!(pluralText.startsWith(QLatin1Char('"'))
        && pluralText.endsWith(QLatin1Char('"')))
      && !(pluralText.startsWith(QLatin1Char('\''))
           && pluralText.endsWith(QLatin1Char('\'')))) {
    argsStart = 2;
    pluralText = sourceText;
  } else {
    pluralText = pluralText.mid(1, pluralText.size() - 2);
  }

  QList<FilterExpression> feList;
  for (auto i = argsStart; i < expr.size() - 2; ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  auto resultName = expr.last();

  return new I18npVarNode(sourceText, pluralText, feList, resultName);
}

I18npNode::I18npNode(const QString &sourceText, const QString &pluralText,
                     const QList<Grantlee::FilterExpression> &feList,
                     QObject *parent)
    : Node(parent), m_sourceText(sourceText), m_pluralText(pluralText),
      m_filterExpressionList(feList)
{
}

void I18npNode::render(OutputStream *stream, Context *c) const
{
  QVariantList args;
  Q_FOREACH (const FilterExpression &fe, m_filterExpressionList)
    args.append(fe.resolve(c));
  auto resultString
      = c->localizer()->localizePluralString(m_sourceText, m_pluralText, args);

  streamValueInContext(stream, resultString, c);
}

I18npVarNode::I18npVarNode(const QString &sourceText, const QString &pluralText,
                           const QList<Grantlee::FilterExpression> &feList,
                           const QString &resultName, QObject *parent)
    : Node(parent), m_sourceText(sourceText), m_pluralText(pluralText),
      m_filterExpressionList(feList), m_resultName(resultName)
{
}

void I18npVarNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  QVariantList args;
  Q_FOREACH (const FilterExpression &fe, m_filterExpressionList)
    args.append(fe.resolve(c));
  auto resultString
      = c->localizer()->localizePluralString(m_sourceText, m_pluralText, args);

  c->insert(m_resultName, resultString);
}
