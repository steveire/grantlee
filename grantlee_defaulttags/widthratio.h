/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef WIDTHRATIONODE_H
#define WIDTHRATIONODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class WidthRatioNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  WidthRatioNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};

class WidthRatioNode : public Node
{
  Q_OBJECT
public:
  WidthRatioNode( FilterExpression valExpr, FilterExpression maxExpr, FilterExpression maxWidth, QObject *parent );

  QString render( Context *c );

private:
  int round( qreal );

  FilterExpression m_valExpr;
  FilterExpression m_maxExpr;
  FilterExpression m_maxWidth;

};

#endif

