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

#ifndef SCRIPTABLE_PARSER_H
#define SCRIPTABLE_PARSER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtScript/QScriptValue>

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
  explicit ScriptableParser( Parser *p, QObject* parent = 0 );

  Parser* parser() {
    return m_p;
  }

public Q_SLOTS:
  QObjectList parse( QObject *parent, const QString &stopAt );
  QObjectList parse( QObject *parent, const QStringList &stopAt = QStringList() );

  void skipPast( const QString &tag );

  Token takeNextToken();
  bool hasNextToken() const;
  void removeNextToken();

  void loadLib( const QString &name );

private:
  Parser *m_p;

};

#endif

