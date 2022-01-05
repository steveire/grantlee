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

#include "i18ncp.h"

#include <QtCore/QStringList>

#include "abstractlocalizer.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "template.h"

#include <QtCore/QDebug>
#include <complex>
#include <util.h>

I18ncpNodeFactory::I18ncpNodeFactory() = default;

Node *I18ncpNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 4)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18ncp tag takes at least three arguments"));

  auto contextText = expr.at(1);

  if (!(contextText.startsWith(QLatin1Char('"'))
        && contextText.endsWith(QLatin1Char('"')))
      && !(contextText.startsWith(QLatin1Char('\''))
           && contextText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18ncp tag first argument must be a static string."));
  }
  contextText = contextText.mid(1, contextText.size() - 2);

  auto sourceText = expr.at(2);

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18ncp tag second argument must be a static string."));
  }
  sourceText = sourceText.mid(1, sourceText.size() - 2);

  auto pluralText = expr.at(3);

  auto argsStart = 4;
  if (!(pluralText.startsWith(QLatin1Char('"'))
        && pluralText.endsWith(QLatin1Char('"')))
      && !(pluralText.startsWith(QLatin1Char('\''))
           && pluralText.endsWith(QLatin1Char('\'')))) {
    argsStart = 3;
    pluralText = sourceText;
  } else {
    pluralText = pluralText.mid(1, pluralText.size() - 2);
  }

  QList<FilterExpression> feList;
  for (auto i = argsStart; i < expr.size(); ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  return new I18ncpNode(contextText, sourceText, pluralText, feList);
}

I18ncpVarNodeFactory::I18ncpVarNodeFactory() = default;

Grantlee::Node *I18ncpVarNodeFactory::getNode(const QString &tagContent,
                                              Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() < 6) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: i18ncp_var tag takes at least five arguments"));
  }

  auto contextText = expr.at(1);

  if (!(contextText.startsWith(QLatin1Char('"'))
        && contextText.endsWith(QLatin1Char('"')))
      && !(contextText.startsWith(QLatin1Char('\''))
           && contextText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18ncp_var tag first argument must be a static string."));
  }
  contextText = contextText.mid(1, contextText.size() - 2);

  auto sourceText = expr.at(2);

  if (!(sourceText.startsWith(QLatin1Char('"'))
        && sourceText.endsWith(QLatin1Char('"')))
      && !(sourceText.startsWith(QLatin1Char('\''))
           && sourceText.endsWith(QLatin1Char('\'')))) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral(
            "Error: i18ncp_var tag second argument must be a static string."));
  }
  sourceText = sourceText.mid(1, sourceText.size() - 2);

  auto pluralText = expr.at(3);

  auto argsStart = 4;
  if (!(pluralText.startsWith(QLatin1Char('"'))
        && pluralText.endsWith(QLatin1Char('"')))
      && !(pluralText.startsWith(QLatin1Char('\''))
           && pluralText.endsWith(QLatin1Char('\'')))) {
    argsStart = 3;
    pluralText = sourceText;
  } else {
    pluralText = pluralText.mid(1, pluralText.size() - 2);
  }

  QList<FilterExpression> feList;
  for (auto i = argsStart; i < expr.size() - 2; ++i) {
    feList.append(FilterExpression(expr.at(i), p));
  }

  auto resultName = expr.last();

  return new I18ncpVarNode(contextText, sourceText, pluralText, feList,
                           resultName);
}

I18ncpNode::I18ncpNode(const QString &contextText, const QString &sourceText,
                       const QString &pluralText,
                       const QList<Grantlee::FilterExpression> &feList,
                       QObject *parent)
    : Node(parent), m_contextText(contextText), m_sourceText(sourceText),
      m_pluralText(pluralText), m_filterExpressionList(feList)
{
}

void I18ncpNode::render(OutputStream *stream, Context *c) const
{
  QVariantList args;
  for (const FilterExpression &fe : m_filterExpressionList)
    args.append(fe.resolve(c));
  auto resultString = c->localizer()->localizePluralContextString(
      m_sourceText, m_pluralText, m_contextText, args);

  streamValueInContext(stream, resultString, c);
}

I18ncpVarNode::I18ncpVarNode(const QString &contextText,
                             const QString &sourceText,
                             const QString &pluralText,
                             const QList<Grantlee::FilterExpression> &feList,
                             const QString &resultName, QObject *parent)
    : Node(parent), m_contextText(contextText), m_sourceText(sourceText),
      m_pluralText(pluralText), m_filterExpressionList(feList),
      m_resultName(resultName)
{
}

void I18ncpVarNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  QVariantList args;
  for (const FilterExpression &fe : m_filterExpressionList)
    args.append(fe.resolve(c));
  auto resultString = c->localizer()->localizePluralContextString(
      m_sourceText, m_pluralText, m_contextText, args);

  c->insert(m_resultName, resultString);
}
