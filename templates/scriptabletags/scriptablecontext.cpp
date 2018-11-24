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

#include "scriptablecontext.h"

#include "context.h"
#include "node.h"

ScriptableContext::ScriptableContext(Context *c, QObject *parent)
    : QObject(parent), m_c(c)
{
}

QVariant ScriptableContext::lookup(const QString &name)
{
  return m_c->lookup(name);
}

void ScriptableContext::insert(const QString &name, const QVariant &variant)
{
  m_c->insert(name, variant);
}

void ScriptableContext::push() { m_c->push(); }

void ScriptableContext::pop() { m_c->pop(); }

QString ScriptableContext::render(const QList<QObject *> &list) const
{
  NodeList nodeList;
  QListIterator<QObject *> it(list);
  while (it.hasNext()) {
    auto node = qobject_cast<Node *>(it.next());
    if (node)
      nodeList << node;
  }
  QString ret;
  QTextStream t(&ret);
  OutputStream stream(&t);
  nodeList.render(&stream, m_c);
  return ret;
}
