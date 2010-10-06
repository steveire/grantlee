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

#include "propertytablewidget.h"

#include <QHeaderView>
#include <QMenu>

#include "comboboxdelegate.h"
#include <qevent.h>

PropertyTableWidget::PropertyTableWidget(QWidget* parent)
  : QTableWidget(parent)
{
  QStringList types;
  for ( int i = 0; i < sizeof sTypes / sizeof *sTypes; ++i)
    types << *(sTypes + i);

  setItemDelegateForColumn(0, new ComboBoxDelegate(types, ComboBoxDelegate::Editable));

  setEditTriggers(QAbstractItemView::AllEditTriggers);
  horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

}

void PropertyTableWidget::contextMenuEvent(QContextMenuEvent* contextMenuEvent)
{
  QMenu *popup = new QMenu(this);
  QAction *action;
  action = popup->addAction("Add");
  connect( action, SIGNAL(triggered(bool)), SLOT(slotAdd()));
  action = popup->addAction("Clear All");
  connect( action, SIGNAL(triggered(bool)), SLOT(slotClear()));
  QModelIndex index = indexAt(contextMenuEvent->pos());
  if (index.isValid())
  {
    action = popup->addAction("Remove");
    connect( action, SIGNAL(triggered(bool)), SLOT(slotRemove()));
  }
  popup->exec(contextMenuEvent->globalPos());
}

void PropertyTableWidget::slotAdd()
{
  int row = rowCount();
  insertRow(row);
  setItem(row, 0, new QTableWidgetItem(QString()));
  setItem(row, 1, new QTableWidgetItem(QString()));

  QTableWidgetItem *checkableItem = new QTableWidgetItem;
  checkableItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  checkableItem->setCheckState(Qt::Unchecked);
  setItem(row, 2, checkableItem);

}

void PropertyTableWidget::slotRemove()
{
  removeRow(currentIndex().row());
}

void PropertyTableWidget::slotClear()
{
  for (int row = rowCount() - 1; row >= 0; --row )
    removeRow(row);
}

#include "propertytablewidget.moc"
