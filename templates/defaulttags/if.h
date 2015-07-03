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

#ifndef IFNODE_H
#define IFNODE_H

#include "node.h"

using namespace Grantlee;

class IfNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IfNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;
};

class IfNodeToken;
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
  IfNode( QList<QPair<bool, FilterExpression > > boolVars, int linkType, QObject *parent = 0 );

  IfNode( const QList<IfNodeToken *> &tokens, QObject *parent = 0 );

  void setTrueList( NodeList trueList );
  void setFalseList( NodeList falseList );

  void render( OutputStream *stream, Context *c ) const;

protected:
  void renderTrueList( OutputStream *stream, Context *c ) const;
  void renderFalseList( OutputStream *stream, Context *c ) const;

private:
  QList<QPair<bool, FilterExpression > > m_boolVars;
  QList<IfNodeToken *> m_tokens;
  NodeList m_trueList;
  NodeList m_falseList;
  int m_linkType;

};

#endif
