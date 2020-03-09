/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

// krazy:excludeall=qclasses

#include "bookwindow.h"
#include "bookdelegate.h"
#include "bookwrapper.h"
#include "grantlee_paths.h"
#include "initdb.h"

#include <QDataWidgetMapper>
#include <QInputDialog>
#include <QMessageBox>

#include <grantlee_templates.h>

BookWindow::BookWindow()
{
  ui.setupUi(this);

  if (!QSqlDatabase::drivers().contains("QSQLITE"))
    QMessageBox::critical(this, "Unable to load database",
                          "This demo needs the SQLITE driver");

  // initialize the database
  QSqlError err = initDb();
  if (err.type() != QSqlError::NoError) {
    showError(err);
    return;
  }

  // Create the data model
  model = new QSqlRelationalTableModel(ui.bookTable);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable("books");

  // Rememeber the indexes of the columns
  authorIdx = model->fieldIndex("author");
  genreIdx = model->fieldIndex("genre");

  // Set the relations to the other database tables
  model->setRelation(authorIdx, QSqlRelation("authors", "id", "name"));
  model->setRelation(genreIdx, QSqlRelation("genres", "id", "name"));

  // Set the localized header captions
  model->setHeaderData(authorIdx, Qt::Horizontal, tr("Author Name"));
  model->setHeaderData(genreIdx, Qt::Horizontal, tr("Genre"));
  model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
  model->setHeaderData(model->fieldIndex("year"), Qt::Horizontal, tr("Year"));
  model->setHeaderData(model->fieldIndex("rating"), Qt::Horizontal,
                       tr("Rating"));

  // Populate the model
  if (!model->select()) {
    showError(model->lastError());
    return;
  }

  // Set the model and hide the ID column
  ui.bookTable->setModel(model);
  ui.bookTable->setItemDelegate(new BookDelegate(ui.bookTable));
  ui.bookTable->setColumnHidden(model->fieldIndex("id"), true);
  ui.bookTable->setSelectionMode(QAbstractItemView::SingleSelection);

  // Initialize the Author combo box
  ui.authorEdit->setModel(model->relationModel(authorIdx));
  ui.authorEdit->setModelColumn(
      model->relationModel(authorIdx)->fieldIndex("name"));

  ui.genreEdit->setModel(model->relationModel(genreIdx));
  ui.genreEdit->setModelColumn(
      model->relationModel(genreIdx)->fieldIndex("name"));

  QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
  mapper->setModel(model);
  mapper->setItemDelegate(new BookDelegate(this));
  mapper->addMapping(ui.titleEdit, model->fieldIndex("title"));
  mapper->addMapping(ui.yearEdit, model->fieldIndex("year"));
  mapper->addMapping(ui.authorEdit, authorIdx);
  mapper->addMapping(ui.genreEdit, genreIdx);
  mapper->addMapping(ui.ratingEdit, model->fieldIndex("rating"));

  connect(ui.bookTable->selectionModel(),
          SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), mapper,
          SLOT(setCurrentModelIndex(QModelIndex)));

  ui.bookTable->setCurrentIndex(model->index(0, 0));

  ui.exportTheme->insertItems(0, QStringList() << "simple"
                                               << "coloured"
                                               << "simple2"
                                               << "coloured2");

  connect(ui.exportButton, SIGNAL(pressed()), SLOT(renderBooks()));

  m_engine = new Grantlee::Engine();
  QSharedPointer<Grantlee::FileSystemTemplateLoader> loader
      = QSharedPointer<Grantlee::FileSystemTemplateLoader>(
          new Grantlee::FileSystemTemplateLoader());
  loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH);
  m_engine->addTemplateLoader(loader);

  m_engine->setPluginPaths(QStringList() << GRANTLEE_PLUGIN_PATH);
}

void BookWindow::showError(const QSqlError &err)
{
  QMessageBox::critical(this, "Unable to initialize Database",
                        "Error initializing database: " + err.text());
}

void BookWindow::renderBooks() const
{
  int rows = model->rowCount();
  QVariantHash mapping;
  QVariantList bookList;
  for (int row = 0; row < rows; ++row) {
    QString title = model->index(row, 1).data().toString();
    QString author = model->index(row, 2).data().toString();
    QString genre = model->index(row, 3).data().toString();
    int rating = model->index(row, 5).data().toInt();
    QObject *book = new BookWrapper(author, title, genre, rating,
                                    const_cast<BookWindow *>(this));
    QVariant var = QVariant::fromValue(book);
    bookList.append(var);
  }
  mapping.insert("books", bookList);

  QString themeName = ui.exportTheme->currentText();

  Grantlee::Context c(mapping);

  Grantlee::Template t = m_engine->loadByName(themeName + ".html");
  if (!t) {
    QMessageBox::critical(
        const_cast<BookWindow *>(this), "Unable to load template",
        QString("Error loading template: %1").arg(themeName + ".html"));
    return;
  }

  if (t->error()) {
    QMessageBox::critical(
        const_cast<BookWindow *>(this), "Unable to load template",
        QString("Error loading template: %1").arg(t->errorString()));
    return;
  }

  bool ok;
  QString text = QInputDialog::getText(
      const_cast<BookWindow *>(this), tr("Export Location"), tr("file name:"),
      QLineEdit::Normal, QDir::home().absolutePath() + "/book_export.html",
      &ok);
  if (!ok || text.isEmpty())
    return;

  QFile file(text);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QString content = t->render(&c);

  if (t->error()) {
    QMessageBox::critical(
        const_cast<BookWindow *>(this), "Unable render template",
        QString("Error rendering template: %1").arg(t->errorString()));
    return;
  }

  file.write(content.toLocal8Bit());
  file.close();
}
