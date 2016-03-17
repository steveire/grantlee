/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef I18NNODE_H
#define I18NNODE_H

#include "node.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class I18nNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  I18nNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class I18nVarNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  I18nVarNodeFactory();

  Node *getNode(const QString &tagContent, Parser *p) const override;
};

class I18nNode : public Node
{
  Q_OBJECT
public:
  I18nNode(const QString &sourceText, const QList<FilterExpression> &feList,
           QObject *parent = 0);
  void render(OutputStream *stream, Context *c) const override;

private:
  QString m_sourceText;
  QList<FilterExpression> m_filterExpressionList;
};

class I18nVarNode : public Node
{
  Q_OBJECT
public:
  I18nVarNode(const QString &sourceText, const QList<FilterExpression> &feList,
              const QString &resultName, QObject *parent = 0);
  void render(OutputStream *stream, Context *c) const override;

private:
  QString m_sourceText;
  QList<FilterExpression> m_filterExpressionList;
  QString m_resultName;
};

#endif
