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

#ifndef IFNODE_H
#define IFNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "grantlee_export.h"


using namespace Grantlee;


class IfNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;
};

class IfNode : public Node
{
  Q_OBJECT
public:
  enum LinkType {
    OrLink,
    AndLink
  };

  /**
  The expression.
  Nodes to render if the expression is true
  Nodes to render if the expression is false
  */
  IfNode( QList<QPair<bool, FilterExpression > > boolVars, NodeList trueList, NodeList falseList, int linkType, QObject *parent = 0 );

  QString render( Context *c );

  NodeList getNodesByType( const char * className );

protected:
  QString renderTrueList( Context *c );
  QString renderFalseList( Context *c );

private:
  QList<QPair<bool, FilterExpression > > m_boolVars;
  NodeList m_trueList;
  NodeList m_falseList;
  int m_linkType;

};

#endif

