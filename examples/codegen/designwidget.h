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

#include <QWidget>

#ifndef DESIGNWIDGET_H
#define DESIGNWIDGET_H

#include "ui_designwidget.h"

#include <grantlee/context.h>

class ArgsModel;
class MethodModel;

class DesignWidget : public QWidget
{
  Q_OBJECT
public:
  DesignWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

  Grantlee::Context getContext();

  QString outputType() const;

Q_SIGNALS:
  void generateClicked(bool clicked);

private:
  void setInitialContent();
  void insertProperty(int row, const QString &type, const QString &name,
                      bool readonly);
  void insertMethod(const QString &access, bool _virtual, const QString &type,
                    const QString &name, bool _const,
                    QList<QStringList> args = QList<QStringList>());

private Q_SLOTS:
  void setArgsRootIndex(const QModelIndex &index);

private:
  Ui::DesignWidget ui;

  MethodModel *m_methodModel;
  ArgsModel *m_argsModel;
};

#endif
