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

#ifndef EXTENDSNODE_H
#define EXTENDSNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
class Template;
}

using namespace Grantlee;

class ExtendsNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  ExtendsNodeFactory( QObject *parent = 0 );

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};

class ExtendsNode : public Node
{
  Q_OBJECT
public:
  ExtendsNode( NodeList list, const QString &filename, FilterExpression fe, QObject *parent );

  QString render( Context *c );

  NodeList getNodesByType( const char * className );

  void appendNode( Node* node );

  Template *getParent( Context *c );

private:
  FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;
};

#endif

