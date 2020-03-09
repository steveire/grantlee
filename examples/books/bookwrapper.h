/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

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

#ifndef BOOK_WRAPPER_H
#define BOOK_WRAPPER_H

#include <QObject>

class BookWrapper : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString author READ author)
  Q_PROPERTY(QString title READ title)
  Q_PROPERTY(QString genre READ genre)
  Q_PROPERTY(int rating READ rating)
public:
  BookWrapper(QString author, QString title, QString genre, int rating,
              QObject *parent = 0)
      : QObject(parent), m_author(author), m_title(title), m_genre(genre),
        m_rating(rating)
  {
  }

  QString author() { return m_author; }
  QString title() { return m_title; }
  QString genre() { return m_genre; }
  int rating() { return m_rating; }

private:
  QString m_title;
  QString m_author;
  QString m_genre;
  int m_rating;
};

#endif
