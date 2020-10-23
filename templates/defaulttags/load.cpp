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

#include "load.h"

#include "../lib/exception.h"
#include "parser.h"

LoadNodeFactory::LoadNodeFactory() {}

Node *LoadNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  auto expr = tagContent.split(QLatin1Char(' '),
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                               QString::SkipEmptyParts
#else
                               Qt::SkipEmptyParts
#endif
  );

  if (expr.size() <= 1) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("%1 expects at least one argument").arg(expr.first()));
  }

  expr.takeAt(0);

  for (auto &i : expr) {
    p->loadLib(i);
  }

  return new LoadNode(p);
}

LoadNode::LoadNode(QObject *parent) : Node(parent) {}

void LoadNode::render(OutputStream *stream, Context *c) const
{
  Q_UNUSED(stream)
  Q_UNUSED(c)
}
