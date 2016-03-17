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

#ifndef SCRIPTABLE_SAFESTRING
#define SCRIPTABLE_SAFESTRING

#include <QtCore/QObject>
#include <QtScript/QScriptValue>

#include "safestring.h"

class QScriptContext;

using namespace Grantlee;

QScriptValue markSafeFunction(QScriptContext *context, QScriptEngine *engine);

class ScriptableSafeString : public QObject
{
  Q_OBJECT
public:
  ScriptableSafeString(QObject *parent = 0);

  void setContent(const SafeString &content);
  SafeString wrappedString() const;

public Q_SLOTS:
  bool isSafe() const;
  void setSafety(bool safeness);
  QString rawString();

private:
  SafeString m_safeString;
};

#endif
