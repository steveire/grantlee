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

#ifndef METHOD_MODEL_H
#define METHOD_MODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class MethodModel : public QStandardItemModel
{
  Q_OBJECT
public:
  MethodModel(QObject *parent = 0);
};

class ArgsModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  ArgsModel(QObject *parent = 0);

  virtual QVariant headerData(int section, Qt::Orientation orientation,
                              int role = Qt::DisplayRole) const;
};

#endif
