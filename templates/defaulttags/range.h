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

#ifndef RANGE_H
#define RANGE_H

#include "node.h"

using namespace Grantlee;

class RangeNodeFactory : public AbstractNodeFactory
{
public:
  RangeNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class RangeNode : public Node
{
  Q_OBJECT
public:
  RangeNode(const QString &name, const FilterExpression &startExpression,
            const FilterExpression &stopExpression, QObject *parent = 0);
  RangeNode(const QString &name, const FilterExpression &startExpression,
            const FilterExpression &stopExpression,
            const FilterExpression &stepExpression = FilterExpression(),
            QObject *parent = 0);

  void setNodeList(const NodeList &list);

  void render(OutputStream *stream, Context *c) const override;

private:
  NodeList m_list;
  QString m_name;
  FilterExpression m_startExpression;
  FilterExpression m_stopExpression;
  FilterExpression m_stepExpression;
};

#endif
