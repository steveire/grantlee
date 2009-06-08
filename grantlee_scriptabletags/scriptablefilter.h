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

#ifndef SCRIPTABLE_FILTER_H
#define SCRIPTABLE_FILTER_H

#include <QScriptValue>

#include "filter.h"

using namespace Grantlee;

class ScriptableFilter : public Filter
{
  Q_OBJECT
public:
  ScriptableFilter( QScriptValue filterObject, QScriptEngine *engine, QObject *parent = 0 );
  virtual ~ScriptableFilter();

  SafeString doFilter( const QVariant &input, const SafeString &argument, bool autoescape = false ) const;

  bool isSafe() const;

private:
  QScriptValue m_filterObject;
  QScriptEngine *m_scriptEngine;

};

#endif

