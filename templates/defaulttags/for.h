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

#ifndef FORNODE_H
#define FORNODE_H

#include "node.h"

using namespace Grantlee;

class ForNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ForNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class ForNode : public Node
{
  Q_OBJECT
public:
  enum Reversed { IsNotReversed, IsReversed };

  ForNode(const QStringList &loopVars, const FilterExpression &fe, int reversed,
          QObject *parent = {});

  void setLoopList(const NodeList &loopNodeList);
  void setEmptyList(const NodeList &emptyList);

  void render(OutputStream *stream, Context *c) const override;

private:
  static void insertLoopVariables(Context *c, int listSize, int i);
  void renderLoop(OutputStream *stream, Context *c) const;

  QStringList m_loopVars;
  FilterExpression m_filterExpression;
  NodeList m_loopNodeList;
  NodeList m_emptyNodeList;
  int m_isReversed;
};

#endif
