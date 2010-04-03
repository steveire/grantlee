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

#include "methodmodel.h"

MethodModel::MethodModel(QObject* parent)
  : QStandardItemModel(parent)
{
  setHorizontalHeaderLabels(QStringList() << "Access" << "virtual" << "Type" << "Name" << "Const");
}

ArgsModel::ArgsModel(QObject* parent)
  : QSortFilterProxyModel(parent)
{

}

QVariant ArgsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
  {
    switch(section)
    {
    case 0: return "Type";
    case 1: return "Name";
    case 2: return "Default";
    }
  }
  return QSortFilterProxyModel::headerData(section, orientation, role);
}

#include "methodmodel.moc"
