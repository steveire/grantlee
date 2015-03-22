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

#ifndef COMBOBOX_DELEGATE_H
#define COMBOBOX_DELEGATE_H

#include <QItemDelegate>

static const char * sTypes[] = {
  "int",
  "qreal",
  "QString",
  "QStringList",
  "QDateTime",
  "QPoint",
  "QFile",
  "QDir",
  "QUrl",
  "QSize",
  "QRect",
  "QObject *",
  "QVariant",
  "QModelIndex",
  "QColor",
  "QWidget *",
  "QAction *",
  "QModelIndex"
};

class ComboBoxDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  enum Type
  {
    NotEditable,
    Editable
  };
  ComboBoxDelegate(const QStringList &data, Type type = NotEditable, QObject* parent = 0);

  virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
  mutable QHash<int, QSize> m_sizes;
};

#endif
