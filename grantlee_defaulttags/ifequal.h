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

#ifndef IFEQUALNODE_H
#define IFEQUALNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class IfEqualNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfEqualNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

protected:
  Node* do_getNode( const QString& tagContent, Parser *p, bool negate, QObject *parent ) const;
};

class IfNotEqualNodeFactory : public IfEqualNodeFactory
{
  Q_OBJECT
public:
  IfNotEqualNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;
};

class IfEqualNode : public Node
{
  Q_OBJECT
public:
  IfEqualNode( FilterExpression val11, FilterExpression val2, NodeList trueList, NodeList falseList, bool negate, QObject *parent );

  QString render( Context *c );

  NodeList getNodesByType( const char * className );


private:
  FilterExpression m_var1;
  FilterExpression m_var2;
  NodeList m_trueList;
  NodeList m_falseList;
  bool m_negate;

};

#endif

