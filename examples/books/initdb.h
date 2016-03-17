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

#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

void addBook(QSqlQuery &q, const QString &title, int year,
             const QVariant &authorId, const QVariant &genreId, int rating)
{
  q.addBindValue(title);
  q.addBindValue(year);
  q.addBindValue(authorId);
  q.addBindValue(genreId);
  q.addBindValue(rating);
  q.exec();
}

QVariant addGenre(QSqlQuery &q, const QString &name)
{
  q.addBindValue(name);
  q.exec();
  return q.lastInsertId();
}

QVariant addAuthor(QSqlQuery &q, const QString &name, const QDate &birthdate)
{
  q.addBindValue(name);
  q.addBindValue(birthdate);
  q.exec();
  return q.lastInsertId();
}

QSqlError initDb()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(":memory:");

  if (!db.open())
    return db.lastError();

  QStringList tables = db.tables();
  if (tables.contains("books", Qt::CaseInsensitive)
      && tables.contains("authors", Qt::CaseInsensitive))
    return QSqlError();

  QSqlQuery q;
  if (!q.exec(QStringLiteral("create table books(id integer primary key, title varchar, author integer, genre integer, year integer, rating integer)")))
    return q.lastError();
  if (!q.exec(QStringLiteral("create table authors(id integer primary key, name varchar, birthdate date)")))
    return q.lastError();
  if (!q.exec(QStringLiteral(
          "create table genres(id integer primary key, name varchar)")))
    return q.lastError();

  if (!q.prepare(
          QStringLiteral("insert into authors(name, birthdate) values(?, ?)")))
    return q.lastError();
  QVariant asimovId
      = addAuthor(q, QStringLiteral("Isaac Asimov"), QDate(1920, 2, 1));
  QVariant greeneId
      = addAuthor(q, QStringLiteral("Graham Greene"), QDate(1904, 10, 2));
  QVariant pratchettId
      = addAuthor(q, QStringLiteral("Terry Pratchett"), QDate(1948, 4, 28));

  if (!q.prepare(QStringLiteral("insert into genres(name) values(?)")))
    return q.lastError();
  QVariant sfiction = addGenre(q, QStringLiteral("Science Fiction"));
  QVariant fiction = addGenre(q, QStringLiteral("Fiction"));
  QVariant fantasy = addGenre(q, QStringLiteral("Fantasy"));

  if (!q.prepare(QStringLiteral("insert into books(title, year, author, genre, rating) values(?, ?, ?, ?, ?)")))
    return q.lastError();
  addBook(q, QStringLiteral("Foundation"), 1951, asimovId, sfiction, 3);
  addBook(q, QStringLiteral("Foundation and Empire"), 1952, asimovId, sfiction,
          4);
  addBook(q, QStringLiteral("Second Foundation"), 1953, asimovId, sfiction, 3);
  addBook(q, QStringLiteral("Foundation's Edge"), 1982, asimovId, sfiction, 3);
  addBook(q, QStringLiteral("Foundation and Earth"), 1986, asimovId, sfiction,
          4);
  addBook(q, QStringLiteral("Prelude to Foundation"), 1988, asimovId, sfiction,
          3);
  addBook(q, QStringLiteral("Forward the Foundation"), 1993, asimovId, sfiction,
          3);
  addBook(q, QStringLiteral("The Power and the Glory"), 1940, greeneId, fiction,
          4);
  addBook(q, QStringLiteral("The Third Man"), 1950, greeneId, fiction, 5);
  addBook(q, QStringLiteral("Our Man in Havana"), 1958, greeneId, fiction, 4);
  addBook(q, QStringLiteral("Guards! Guards!"), 1989, pratchettId, fantasy, 3);
  addBook(q, QStringLiteral("Night Watch"), 2002, pratchettId, fantasy, 3);
  addBook(q, QStringLiteral("Going Postal"), 2004, pratchettId, fantasy, 3);

  return QSqlError();
}

#endif
