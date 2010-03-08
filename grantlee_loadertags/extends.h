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

#ifndef EXTENDSNODE_H
#define EXTENDSNODE_H

#include "node.h"
#include "template.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class ExtendsNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ExtendsNodeFactory( QObject *parent = 0 );

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class ExtendsNode : public Node
{
  Q_OBJECT
public:
  ExtendsNode( const QString &filename, FilterExpression fe, QObject *parent = 0 );
  ~ExtendsNode();

  void setNodeList( NodeList list );

  void render( OutputStream *stream, Context *c );

  void appendNode( Node* node );

  Template getParent( Context *c );

  virtual bool mustBeFirst() {
    return true;
  }


private:
  FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;
  Template m_parentTemplate;
};

#endif

