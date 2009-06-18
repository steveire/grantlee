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

#ifndef CYCLENODE_H
#define CYCLENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


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
  RingIterator<T>( QList<T> list )
      : m_it( list ) {
    Q_ASSERT( list.size() > 0 );
  }

  /**
  Returns the next element in the list, or the first element if already
  at the last element.
  */
  T next() {
    if ( m_it.hasNext() ) {
      T item = m_it.next();
      return item;
    }
    m_it.toFront();
    return m_it.next();
  }

private:
  QListIterator<T> m_it;
};

class CycleNode : public Node
{
  Q_OBJECT
public:
  CycleNode( QList<FilterExpression> list, const QString &name, QObject *parent = 0 );

  QString render( Context *c );

private:
  RingIterator<FilterExpression> m_variableIterator;
  QString m_name;

};

#endif

