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

#ifndef IFCHANGEDNODE_H
#define IFCHANGEDNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class IfChangedNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfChangedNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;
};

class IfChangedNode : public Node
{
  Q_OBJECT
public:
  IfChangedNode( QList<FilterExpression> feList, QObject *parent = 0 );

  void setTrueList( NodeList trueList );
  void setFalseList( NodeList falseList );

  QString render( Context *c );

private:
  NodeList m_trueList;
  NodeList m_falseList;
  QList<FilterExpression> m_filterExpressions;
  QVariant m_lastSeen;
  QString m_id;

};

#endif

