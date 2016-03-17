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

#include "templatetag.h"

#include "../lib/exception.h"
#include "grantlee_tags_p.h"
#include "parser.h"

TemplateTagNodeFactory::TemplateTagNodeFactory() {}

Node *TemplateTagNodeFactory::getNode(const QString &tagContent,
                                      Parser *p) const
{
  auto expr = tagContent.split(QLatin1Char(' '), QString::SkipEmptyParts);
  expr.takeAt(0);
  if (expr.size() <= 0) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("'templatetag' statement takes one argument"));
  }

  auto name = expr.first();

  if (!TemplateTagNode::isKeyword(name)) {
    throw Grantlee::Exception(TagSyntaxError,
                              QStringLiteral("Not a template tag"));
  }

  return new TemplateTagNode(name, p);
}

TemplateTagNode::TemplateTagNode(const QString &name, QObject *parent)
    : Node(parent)
{
  m_name = name;
}

static QHash<QString, QString> getKeywordMap()
{
  QHash<QString, QString> map;
  map.insert(QStringLiteral("openblock"), QLatin1String(BLOCK_TAG_START));
  map.insert(QStringLiteral("closeblock"), QLatin1String(BLOCK_TAG_END));
  map.insert(QStringLiteral("openvariable"), QLatin1String(VARIABLE_TAG_START));
  map.insert(QStringLiteral("closevariable"), QLatin1String(VARIABLE_TAG_END));
  map.insert(QStringLiteral("openbrace"), QChar::fromLatin1('{'));
  map.insert(QStringLiteral("closebrace"), QChar::fromLatin1('}'));
  map.insert(QStringLiteral("opencomment"), QLatin1String(COMMENT_TAG_START));
  map.insert(QStringLiteral("closecomment"), QLatin1String(COMMENT_TAG_END));
  return map;
}

bool TemplateTagNode::isKeyword(const QString &name)
{
  static auto map = getKeywordMap();
  return map.contains(name);
}

void TemplateTagNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(c)
  static auto map = getKeywordMap();
  (*stream) << map.value(m_name);
}
