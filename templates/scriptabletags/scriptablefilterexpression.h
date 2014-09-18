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

#ifndef SCRIPTABLE_FILTEREXPRESSION_H
#define SCRIPTABLE_FILTEREXPRESSION_H

#include <QtCore/QObject>

#include <QtScript/QScriptValue>

#include "filterexpression.h"

class QScriptContext;

class ScriptableContext;

QScriptValue ScriptableFilterExpressionConstructor( QScriptContext *context,
    QScriptEngine *engine );


class ScriptableFilterExpression : public QObject
{
  Q_OBJECT
public:
  ScriptableFilterExpression( QObject *parent = 0 );
  ScriptableFilterExpression( QScriptEngine *engine, QObject *parent = 0 );

  void init( const QString &content, Grantlee::Parser *parser );

public Q_SLOTS:
  QVariant resolve( ScriptableContext *c );

  bool isTrue( ScriptableContext *c );

  bool equals( ScriptableFilterExpression *other, ScriptableContext *scriptableC );

  // list? QScriptValueList? Make this a ScriptClass?

private:
  Grantlee::FilterExpression m_filterExpression;
  QScriptEngine *m_engine;
};

#endif
