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

#ifndef INCLUDENODE_H
#define INCLUDENODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
}


using namespace Grantlee;

class IncludeNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  IncludeNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p, QObject *parent ) const;

};

class IncludeNode : public Node
{
  Q_OBJECT
public:
  IncludeNode( const FilterExpression &fe, QObject *parent );
  QString render( Context *c );

private:
  FilterExpression m_filterExpression;

};

class ConstantIncludeNode : public Node
{
  Q_OBJECT
public:
  ConstantIncludeNode( const QString &filename, QObject *parent );
  QString render( Context *c );

private:
  QString m_name;

};

#endif

