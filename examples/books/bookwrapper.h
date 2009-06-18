
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
  BookWrapper(QString author, QString title, QString genre, int rating, QObject *parent = 0)
      : QObject(parent), m_author(author), m_title(title), m_genre(genre), m_rating(rating)
  {}

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

