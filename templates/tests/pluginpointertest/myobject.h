/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QtCore/QObject>

#include "myinterface.h"

class MyObject : public QObject, public MyInterface
{
  Q_OBJECT
  Q_INTERFACES(MyInterface)
  Q_PLUGIN_METADATA(IID "org.grantlee.MyInterface")
public:
  MyObject(int stop = 4, QObject *parent = {});

  int double_it(int input);

private:
  QByteArray m_data;
};

#endif
