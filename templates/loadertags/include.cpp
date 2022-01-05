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

#include "include.h"

#include "block.h"
#include "blockcontext.h"
#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "rendercontext.h"
#include "template.h"
#include "util.h"

IncludeNodeFactory::IncludeNodeFactory() = default;

Node *IncludeNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() != 2)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: Include tag takes only one argument"));

  auto includeName = expr.at(1);
  auto size = includeName.size();

  if ((includeName.startsWith(QLatin1Char('"'))
       && includeName.endsWith(QLatin1Char('"')))
      || (includeName.startsWith(QLatin1Char('\''))
          && includeName.endsWith(QLatin1Char('\'')))) {
    return new ConstantIncludeNode(includeName.mid(1, size - 2));
  }
  return new IncludeNode(FilterExpression(includeName, p), p);
}

IncludeNode::IncludeNode(const FilterExpression &fe, QObject *parent)
    : Node(parent), m_filterExpression(fe)
{
}

void IncludeNode::render(OutputStream *stream, Context *c) const
{
  QString filename = getSafeString(m_filterExpression.resolve(c));

  auto ti = containerTemplate();

  auto t = ti->engine()->loadByName(filename);

  if (!t)
    throw Grantlee::Exception(
        TagSyntaxError, QStringLiteral("Template not found %1").arg(filename));

  if (t->error())
    throw Grantlee::Exception(t->error(), t->errorString());

  t->render(stream, c);

  if (t->error())
    throw Grantlee::Exception(t->error(), t->errorString());
}

ConstantIncludeNode::ConstantIncludeNode(const QString &name, QObject *parent)
    : Node(parent)
{
  m_name = name;
}

void ConstantIncludeNode::render(OutputStream *stream, Context *c) const
{
  auto ti = containerTemplate();

  auto t = ti->engine()->loadByName(m_name);
  if (!t)
    throw Grantlee::Exception(
        TagSyntaxError, QStringLiteral("Template not found %1").arg(m_name));

  if (t->error())
    throw Grantlee::Exception(t->error(), t->errorString());

  t->render(stream, c);

  if (t->error())
    throw Grantlee::Exception(t->error(), t->errorString());

  QVariant &variant = c->renderContext()->data(nullptr);
  auto blockContext = variant.value<BlockContext>();
  auto nodes = t->findChildren<BlockNode *>();
  blockContext.remove(nodes);
  variant.setValue(blockContext);
}
