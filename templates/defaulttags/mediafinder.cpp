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

#include "mediafinder.h"

#include "engine.h"
#include "exception.h"
#include "parser.h"
#include "util.h"

#include <QtCore/QUrl>

MediaFinderNodeFactory::MediaFinderNodeFactory() {}

Grantlee::Node *MediaFinderNodeFactory::getNode(const QString &tagContent,
                                                Parser *p) const
{
  auto expr = smartSplit(tagContent);

  if (expr.size() <= 1) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("'media_finder' tag requires at least one argument"));
  }
  expr.takeAt(0);

  return new MediaFinderNode(getFilterExpressionList(expr, p), p);
}

MediaFinderNode::MediaFinderNode(
    const QList<FilterExpression> &mediaExpressionList, QObject *parent)
    : Node(parent), m_mediaExpressionList(mediaExpressionList)
{
}

void MediaFinderNode::render(OutputStream *stream, Context *c) const
{
  auto t = containerTemplate();
  auto engine = t->engine();

  for (const FilterExpression &fe : m_mediaExpressionList) {
    if (fe.isTrue(c)) {
      auto fileUrl = engine->mediaUri(getSafeString(fe.resolve(c)));
      if (fileUrl.second.isEmpty())
        continue;
      auto uri = QUrl::fromLocalFile(fileUrl.first).toString();
      c->addExternalMedia(uri, fileUrl.second);
      if (c->urlType() == Context::AbsoluteUrls)
        streamValueInContext(stream, uri, c);
      else if (!c->relativeMediaPath().isEmpty())
        streamValueInContext(
            stream, QVariant(c->relativeMediaPath() + QLatin1Char('/')), c);
      streamValueInContext(stream, fileUrl.second, c);
      return;
    }
  }
}
