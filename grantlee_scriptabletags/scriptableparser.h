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

#ifndef SCRIPTABLE_PARSER_H
#define SCRIPTABLE_PARSER_H

#include <QObject>
#include <QScriptValue>
#include <QVariant>

#include "token.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class ScriptableParser : public QObject
{
  Q_OBJECT
public:
  ScriptableParser( Parser *p, QObject* parent = 0 );

  Parser* parser() {
    return m_p;
  }

public slots:
  QObjectList parse( const QString &stopAt, QObject *parent );
  QObjectList parse( const QStringList &stopAt, QObject *parent );
  QObjectList parse( QObject *parent );

  void skipPast( const QString &tag );

  Token nextToken();
  bool hasNextToken() const;
  void deleteNextToken();

  void loadLib( const QString &name );

private:
  Parser *m_p;

};

#endif

