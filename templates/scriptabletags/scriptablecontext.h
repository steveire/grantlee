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

#ifndef SCRIPTABLE_CONTEXT_H
#define SCRIPTABLE_CONTEXT_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace Grantlee
{
class Context;
}

using namespace Grantlee;

class ScriptableContext : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableContext(Context *c, QObject *parent = 0);

  Context *context() { return m_c; }

public Q_SLOTS:
  QVariant lookup(const QString &name);
  void insert(const QString &name, const QVariant &variant);
  void push();
  void pop();

  QString render(const QList<QObject *> &list) const;

private:
  Context *m_c;
};

#endif
