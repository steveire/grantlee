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

#include "scriptableparser.h"

#include "parser.h"

#include <QtQml/QJSEngine>

ScriptableParser::ScriptableParser(Grantlee::Parser *p, QJSEngine *engine)
    : QObject(engine), m_p(p), m_engine(engine)
{
}

void ScriptableParser::removeNextToken() { m_p->removeNextToken(); }

bool ScriptableParser::hasNextToken() const { return m_p->hasNextToken(); }

void ScriptableParser::loadLib(const QString &name) { m_p->loadLib(name); }

QJSValue ScriptableParser::takeNextToken()
{
  Token t = m_p->takeNextToken();
  auto obj = m_engine->newObject();
  obj.setProperty(QStringLiteral("tokenType"), t.tokenType);
  obj.setProperty(QStringLiteral("content"), t.content);
  return obj;
}

void ScriptableParser::skipPast(const QString &tag) { m_p->skipPast(tag); }

QList<QObject *> ScriptableParser::parse(QObject *parent, const QString &stopAt)
{
  return parse(parent, QStringList() << stopAt);
}

QList<QObject *> ScriptableParser::parse(QObject *parent,
                                         const QStringList &stopAt)
{
  auto node = qobject_cast<Node *>(parent);
  Q_ASSERT(node);

  auto nodeList = m_p->parse(node, stopAt);
  QList<QObject *> objList;
  for (auto n : nodeList) {
    objList << n;
  }
  return objList;
}
