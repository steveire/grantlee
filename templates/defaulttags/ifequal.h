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

#ifndef IFEQUALNODE_H
#define IFEQUALNODE_H

#include "node.h"

using namespace Grantlee;

class IfEqualNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfEqualNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;

protected:
  Node *do_getNode(const QString &tagContent, Parser *p, bool negate) const;
};

class IfNotEqualNodeFactory : public IfEqualNodeFactory
{
  Q_OBJECT
public:
  IfNotEqualNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class IfEqualNode : public Node
{
  Q_OBJECT
public:
  IfEqualNode(const FilterExpression &val11, const FilterExpression &val2,
              bool negate, QObject *parent = {});

  void setTrueList(const NodeList &trueList);
  void setFalseList(const NodeList &falseList);

  void render(OutputStream *stream, Context *c) const override;

private:
  FilterExpression m_var1;
  FilterExpression m_var2;
  NodeList m_trueList;
  NodeList m_falseList;
  bool m_negate;
};

#endif
