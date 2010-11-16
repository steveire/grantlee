/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef I18NCNODE_H
#define I18NCNODE_H

#include "node.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class I18ncNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  I18ncNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class I18ncVarNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  I18ncVarNodeFactory();

  Node* getNode( const QString &tagContent, Parser *p ) const;

};

class I18ncNode : public Node
{
  Q_OBJECT
public:
  I18ncNode( const QString &sourceText, const QString &context, const QList<FilterExpression> &feList, QObject *parent = 0 );
  void render( OutputStream *stream, Context *c ) const;

private:
  QString m_sourceText;
  QString m_context;
  QList<FilterExpression> m_filterExpressionList;
};

class I18ncVarNode : public Node
{
  Q_OBJECT
public:
  I18ncVarNode( const QString &sourceText, const QString &context, const QList<FilterExpression> &feList, const QString &resultName, QObject *parent = 0 );
  void render( OutputStream *stream, Context *c ) const;

private:
  QString m_sourceText;
  QString m_context;
  QList<FilterExpression> m_filterExpressionList;
  QString m_resultName;
};

#endif
