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

#ifndef L10N_MONEYNODE_H
#define L10N_MONEYNODE_H

#include "node.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class L10nMoneyNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  L10nMoneyNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class L10nMoneyVarNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  L10nMoneyVarNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class L10nMoneyNode : public Node
{
  Q_OBJECT
public:
  L10nMoneyNode( const FilterExpression &value, const FilterExpression &currency, QObject *parent = 0 );
  void render( OutputStream *stream, Context *c );

private:
  FilterExpression m_value;
  FilterExpression m_currency;
};

class L10nMoneyVarNode : public Node
{
  Q_OBJECT
public:
  L10nMoneyVarNode( const FilterExpression &value, const FilterExpression &currency, const QString &resultName, QObject *parent = 0 );
  void render( OutputStream *stream, Context *c );

private:
  FilterExpression m_value;
  FilterExpression m_currency;
  QString m_resultName;
};

#endif
