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

#include "i18n.h"

#include <QtCore/QStringList>

#include "abstractlocalizer.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "template.h"

#include <QtCore/QDebug>
#include <complex>
#include <util.h>

I18nNodeFactory::I18nNodeFactory() {}

Node *I18nNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 2)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18n tag takes at least one argument"));

  auto sourceText = expr.at(1);
  auto size = sourceText.size();

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18n tag first argument must be a static string."));
  }
  sourceText = sourceText.mid(1, size - 2);

  QList<FilterExpression> feList;
  for (auto i = 2; i < expr.size(); ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  return new I18nNode(sourceText, feList);
}

I18nVarNodeFactory::I18nVarNodeFactory() {}

Grantlee::Node *I18nVarNodeFactory::getNode(const QString &tagContent,
                                            Parser *p) const
{

  auto expr = smartSplit(tagContent);

  if (expr.size() < 4)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18n_var tag takes at least three arguments"));

  auto sourceText = expr.at(1);
  auto size = sourceText.size();

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18n tag first argument must be a static string."));
  }
  sourceText = sourceText.mid(1, size - 2);

  QList<FilterExpression> feList;
  for (auto i = 2; i < expr.size() - 2; ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  auto resultName = expr.last();

  return new I18nVarNode(sourceText, feList, resultName);
}

I18nNode::I18nNode(const QString &sourceText,
                   const QList<Grantlee::FilterExpression> &feList,
                   QObject *parent)
    : Node(parent), m_sourceText(sourceText), m_filterExpressionList(feList)
{
}

void I18nNode::render(OutputStream *stream, Context *c) const
{
  QVariantList args;
  Q_FOREACH (const FilterExpression &fe, m_filterExpressionList)
    args.append(fe.resolve(c));
  const auto resultString = c->localizer()->localizeString(m_sourceText, args);

  streamValueInContext(stream, resultString, c);
}

I18nVarNode::I18nVarNode(const QString &sourceText,
                         const QList<Grantlee::FilterExpression> &feList,
                         const QString &resultName, QObject *parent)
    : Node(parent), m_sourceText(sourceText), m_filterExpressionList(feList),
      m_resultName(resultName)
{
}

void I18nVarNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  QVariantList args;
  Q_FOREACH (const FilterExpression &fe, m_filterExpressionList)
    args.append(fe.resolve(c));
  auto resultString = c->localizer()->localizeString(m_sourceText, args);

  c->insert(m_resultName, resultString);
}
