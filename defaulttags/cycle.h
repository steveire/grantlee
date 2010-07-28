/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef CYCLENODE_H
#define CYCLENODE_H

#include "node.h"

using namespace Grantlee;

class CycleNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  CycleNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

/**
Cycles endlessly over elements.
*/
template <typename T>
class RingIterator
{
public:
  RingIterator<T>( const QList<T> &list )
      : m_begin( list.constBegin() ), m_it( list.constBegin() ), m_end( list.constEnd() )
  {
    Q_ASSERT( !list.isEmpty() );
  }

  /**
    Returns the next element in the list, or the first element if already
    at the last element.
  */
  T next() {
    Q_ASSERT( m_it != m_end );

    const T t = *m_it++;
    if ( m_it == m_end )
      m_it = m_begin;
    return t;
  }

private:
  typename QList<T>::const_iterator m_begin;
  typename QList<T>::const_iterator m_it;
  typename QList<T>::const_iterator m_end;
};

typedef RingIterator<FilterExpression> FilterExpressionRotator;

class CycleNode : public Node
{
  Q_OBJECT
public:
  CycleNode( const QList<FilterExpression> &list, const QString &name, QObject *parent = 0 );

  void render( OutputStream *stream, Context *c );

private:
  const QList<FilterExpression> m_list;
  FilterExpressionRotator m_variableIterator;
  const QString m_name;
};

#endif

