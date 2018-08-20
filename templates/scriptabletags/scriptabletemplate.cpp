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

#include "scriptabletemplate.h"

#include "context.h"
#include "engine.h"
#include "node.h"
#include "scriptablecontext.h"

ScriptableTemplate::ScriptableTemplate(Grantlee::Template t, QObject *parent)
    : QObject(parent), m_template(t)
{
}

QString ScriptableTemplate::render(ScriptableContext *c) const
{
  return m_template->render(c->context());
}

QList<QObject *> ScriptableTemplate::nodeList() const
{
  auto nodeList = m_template->nodeList();
  QObjectList objList;

  QListIterator<Node *> it(nodeList);
  while (it.hasNext()) {
    objList << it.next();
  }
  return objList;
}

void ScriptableTemplate::setNodeList(const QList<QObject *> &list)
{
  NodeList nodeList;

  QListIterator<QObject *> it(list);

  while (it.hasNext()) {
    auto n = qobject_cast<Node *>(it.next());
    if (n) {
      nodeList << n;
    }
  }
  m_template->setNodeList(nodeList);
}
