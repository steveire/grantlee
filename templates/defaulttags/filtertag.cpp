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

#include "filtertag.h"

#include "../lib/exception.h"
#include "filterexpression.h"
#include "parser.h"
#include "util.h"

FilterNodeFactory::FilterNodeFactory() {}

Grantlee::Node *FilterNodeFactory::getNode(const QString &tagContent,
                                           Grantlee::Parser *p) const
{
  auto expr = tagContent.split(QLatin1Char(' '),
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                                        QString::SkipEmptyParts
#else
                                        Qt::SkipEmptyParts
#endif
);

  expr.removeFirst();

  auto expression = expr.join(QChar::fromLatin1(' '));
  FilterExpression fe(QStringLiteral("var|%1").arg(expression), p);

  auto filters = fe.filters();
  if (filters.contains(QStringLiteral("safe"))
      || filters.contains(QStringLiteral("escape"))) {
    throw Grantlee::Exception(
        TagSyntaxError, QStringLiteral("Use the \"autoescape\" tag instead."));
  }

  auto n = new FilterNode(fe, p);

  auto filterNodes = p->parse(n, QStringLiteral("endfilter"));
  p->removeNextToken();

  n->setNodeList(filterNodes);
  return n;
}

FilterNode::FilterNode(const FilterExpression &fe, QObject *parent)
    : Node(parent), m_fe(fe)
{
}

void FilterNode::setNodeList(const NodeList &filterList)
{
  m_filterList = filterList;
}

void FilterNode::render(OutputStream *stream, Context *c) const
{
  QString output;
  QTextStream textStream(&output);
  auto temp = stream->clone(&textStream);
  m_filterList.render(temp.data(), c);
  c->push();
  c->insert(QStringLiteral("var"), output);
  m_fe.resolve(stream, c);
  c->pop();
}
