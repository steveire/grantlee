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


#include "designwidget.h"

#include "codegentableview.h"
#include "methodmodel.h"
#include "comboboxdelegate.h"

#include <QDebug>

DesignWidget::DesignWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f)
{
  ui.setupUi(this);
  setInitialContent();
}

void DesignWidget::setInitialContent()
{
  ui.author_edit->setText("Winston Smith");
  ui.email_edit->setText("winston@smithfamily.com");
  ui.className_edit->setText("MyClass");

  int propRows = 5;
  ui.properties_table->setRowCount(propRows);

  insertProperty(0, "QString", "name", false);
  insertProperty(1, "int", "age", true);
  insertProperty(2, "QColor", "furColor", false);
  insertProperty(3, "bool", "pedigree", true);
  insertProperty(4, "QString", "owner", false);

  QList<QStringList> args;

  m_methodModel = new MethodModel(this);
  insertMethod("public", false, "QString", "getResult", true );
  args << ( QStringList() << "QString" << "silly" );
  args << ( QStringList() << "int" << "number" << "9" );
  insertMethod("public slots", false, "void", "somethingStupid", false, args );
  insertMethod("protected", false, "void", "reset", false );
  args.clear();
  args << ( QStringList() << "int" << "var" );
  args << ( QStringList() << "QString" << "input" << "QString()" );
  insertMethod("protected", false, "QModelIndex", "findIndex", true, args );
  insertMethod("protected", true, "void", "clear", false );
  insertMethod("private", false, "void", "internalMethod", true );

  ui.methods_table->setModel(m_methodModel);

  connect( ui.methods_table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(setArgsRootIndex(QModelIndex)));
  m_argsModel = new ArgsModel(this);
  m_argsModel->setSourceModel(m_methodModel);

  setArgsRootIndex(QModelIndex());

  connect( ui.pushButton, SIGNAL(clicked(bool)), SIGNAL(generateClicked(bool)));
}

void DesignWidget::insertMethod(const QString& access, bool _virtual, const QString& type, const QString& name, bool _const, QList<QStringList> args)
{
  QList<QStandardItem*> method;
  method << new QStandardItem(access)
         << new QStandardItem()
         << new QStandardItem(type)
         << new QStandardItem(name)
         << new QStandardItem();

  method.at(1)->setEditable(false);
  method.at(4)->setEditable(false);
  method.at(1)->setCheckable(true);
  method.at(4)->setCheckable(true);
  method.at(1)->setCheckState(_virtual ? Qt::Checked : Qt::Unchecked);
  method.at(4)->setCheckState(_const ? Qt::Checked : Qt::Unchecked);

  if (!args.isEmpty())
  {
    Q_FOREACH(const QStringList &arg, args)
    {
      QList<QStandardItem*> argItem;
      argItem << new QStandardItem(arg.at(0))
              << new QStandardItem(arg.at(1))
              << new QStandardItem(arg.value(2));
      method.first()->appendRow(argItem);
    }
  }

  m_methodModel->appendRow(method);
}

void DesignWidget::insertProperty(int row, const QString& type, const QString& name, bool readonly)
{
  ui.properties_table->setItem(row, 0, new QTableWidgetItem(type));
  ui.properties_table->setItem(row, 1, new QTableWidgetItem(name));

  QTableWidgetItem *checkableItem = new QTableWidgetItem;
  checkableItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  checkableItem->setCheckState(readonly ? Qt::Unchecked : Qt::Checked);
  ui.properties_table->setItem(row, 2, checkableItem);
}

void DesignWidget::setArgsRootIndex(const QModelIndex &index)
{
  ui.args_table->setModel(index.isValid() ? m_argsModel : 0);
  ui.args_table->setRootIndex(m_argsModel->mapFromSource(index.sibling(index.row(), 0)));
}

Grantlee::Context DesignWidget::getContext()
{
  Grantlee::Context c;
  c.insert("pimpl", ui.pimpl_check->isChecked());
  c.insert("className", ui.className_edit->text());
  c.insert("author", ui.author_edit->text());
  c.insert("email", ui.email_edit->text());
  c.insert("qobject", true);
  c.insert("licence", ui.licence_combo->currentText());

  if ( ui.baseClassAccess->currentText() != "None" )
  {
    QVariantHash baseClass;
    baseClass.insert("module", ui.baseModule->currentText());
    baseClass.insert("access", ui.baseClassAccess->currentText());
    baseClass.insert("type", ui.baseClassType->currentText());
    c.insert("baseClass", baseClass);
  }

  QVariantList properties;
  for (int row = 0; row < ui.properties_table->rowCount(); ++row )
  {
    QVariantHash property;

    property.insert("type", ui.properties_table->item(row, 0)->text());
    property.insert("name", ui.properties_table->item(row, 1)->text());
    property.insert("readonly", ui.properties_table->item(row, 2)->checkState() == Qt::Checked);

    properties << property;
  }
  c.insert( "properties", QVariant( properties ) );

  QVariantList methods;
  for (int row = 0; row < m_methodModel->rowCount(); ++row)
  {
    QVariantHash method;
    QStandardItem *firstColumn = m_methodModel->item(row, 0);

    method.insert("accessType", firstColumn->text());
    method.insert("virtual", m_methodModel->item(row, 1)->checkState() == Qt::Checked);
    method.insert("type", m_methodModel->item(row, 2)->text());
    method.insert("name", m_methodModel->item(row, 3)->text());
    method.insert("const", m_methodModel->item(row, 4)->checkState() == Qt::Checked);

    if (firstColumn->hasChildren())
    {
      QVariantList args;

      for (int argRow = 0; argRow < firstColumn->rowCount(); ++argRow )
      {
        QVariantHash arg;

        arg.insert("type", firstColumn->child(argRow, 0)->text());
        arg.insert("name", firstColumn->child(argRow, 1)->text());
        arg.insert("default", firstColumn->child(argRow, 2)->text());

        args << arg;
      }
      method.insert("args", args);
    }
    methods << method;
  }

  c.insert( "methods", QVariant( methods ) );
  return c;
}

QString DesignWidget::outputType() const
{
  return ui.language_combo->currentText();
}


#include "designwidget.moc"
