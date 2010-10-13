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

#ifndef COMBOBOX_DELEGATE_P_H
#define COMBOBOX_DELEGATE_P_H

#include "comboboxdelegate.h"

#include <QComboBox>
#include <QItemEditorCreatorBase>
#include <QItemEditorFactory>

class ComboBoxEditorCreator : public QItemEditorCreatorBase
{
public:
  explicit ComboBoxEditorCreator(const QStringList &data, ComboBoxDelegate::Type type);
  virtual ~ComboBoxEditorCreator();

  /* reimp */ QWidget *createWidget(QWidget *parent) const;

  /* reimp */ QByteArray valuePropertyName() const;

private:
  QStringList m_data;
  ComboBoxDelegate::Type m_type;
};

class ViewComboBox : public QComboBox
{
  Q_OBJECT
  Q_PROPERTY(QString choice READ choice WRITE setChoice)
public:
  explicit ViewComboBox(QWidget* parent = 0);

  QString choice() const;
  void setChoice(const QString &choice);
};

#endif
