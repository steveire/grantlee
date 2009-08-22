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

#ifndef FORNODE_H
#define FORNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


using namespace Grantlee;


class ForNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ForNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};


class ForNode : public Node
{
  Q_OBJECT
public:
  enum Reversed {
    IsNotReversed,
    IsReversed
  };

  ForNode( QStringList loopVars, FilterExpression fe, int reversed, QObject *parent = 0 );

  void setLoopList( NodeList loopNodeList );
  void setEmptyList( NodeList emptyList );

  QString render( Context *c );

private:
  void insertLoopVariables( Context *c, int listSize, int i );
  QString iterateHash( Context* c, QVariantHash varHash, bool unpack );
  QString renderLoop( Context *c );
  QString handleHashItem( Context *c, QString key, QVariant value, int listSize, int i, bool unpack );

  FilterExpression m_filterExpression;
  QStringList m_loopVars;
  NodeList m_loopNodeList;
  NodeList m_emptyNodeList;
  int m_isReversed;
};


#endif

