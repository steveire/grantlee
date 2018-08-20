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

#include <QtQml/QJSValue>

#include "filterexpression.h"

class ScriptableFilterExpression : public QObject
{
  Q_OBJECT
public:
  ScriptableFilterExpression(QObject *parent = 0);
  ScriptableFilterExpression(QJSEngine *engine, QObject *parent = 0);

  void init(const QString &content, Grantlee::Parser *parser);

public Q_SLOTS:
  QVariant resolve(QObject *c);

  bool isTrue(QObject *c);

  bool equals(ScriptableFilterExpression *other,
              QObject *scriptableC);

  // list? QJSValueList? Make this a ScriptClass?

private:
  Grantlee::FilterExpression m_filterExpression;
  QJSEngine *m_engine;
};

#endif
