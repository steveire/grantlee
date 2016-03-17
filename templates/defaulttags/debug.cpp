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

#include "debug.h"

#include "parser.h"

DebugNodeFactory::DebugNodeFactory() {}

Node *DebugNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  Q_UNUSED(tagContent)
  return new DebugNode(p);
}

DebugNode::DebugNode(QObject *parent) : Node(parent) {}

void DebugNode::render(OutputStream *stream, Context *c) const
{
  QString ret;
  auto i = 0;
  auto h = c->stackHash(i++);

  ret += QStringLiteral("\n\nContext:\n");
  while (!h.isEmpty()) {
    QHashIterator<QString, QVariant> it(h);
    while (it.hasNext()) {
      it.next();
      ret += QStringLiteral("key ") + it.key() + QStringLiteral(", ")
             + QStringLiteral("type ") + QLatin1String(it.value().typeName())
             + QLatin1Char('\n');
    }
    h = c->stackHash(i++);
  }

  ret += QStringLiteral("End context:\n\n");

  (*stream) << ret;
}
