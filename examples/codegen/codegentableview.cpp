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

#include "codegentableview.h"

#include "comboboxdelegate.h"
#include <QHeaderView>

MethodTableView::MethodTableView(QWidget* parent)
  : QTableView(parent)
{
  QStringList accessTypes;
  accessTypes << "public"
              << "protected"
              << "private"
              << "signals"
              << "public slots"
              << "protected slots"
              << "private slots"
              << "Q_PRIVATE_SLOT";

  setItemDelegateForColumn(0, new ComboBoxDelegate(accessTypes));

  QStringList types;
  types << "QString"
        << "QVariant"
        << "QModelIndex";

  setItemDelegateForColumn(2, new ComboBoxDelegate(types, ComboBoxDelegate::Editable));

  setEditTriggers(QAbstractItemView::AllEditTriggers);
  horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

ArgsTableView::ArgsTableView(QWidget* parent)
  : QTableView(parent)
{
  QStringList types;
  types << "const QString &"
        << "const QVariant &"
        << "const QModelIndex &";


  setItemDelegateForColumn(0, new ComboBoxDelegate(types, ComboBoxDelegate::Editable));

  setEditTriggers(QAbstractItemView::AllEditTriggers);
  horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

}

#include "codegentableview.moc"
