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

#ifndef CONTACT_H
#define CONTACT_H

#include <QObject>
#include <QListWidget>
#include <QDate>

class Contact : public QObject, public QListWidgetItem
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(QString email READ email)
  Q_PROPERTY(QString phone READ phone)
  Q_PROPERTY(QString address READ address)
  Q_PROPERTY(QString nickname READ nickname)
  Q_PROPERTY(double  salary READ salary)
  Q_PROPERTY(QString salaryCurrency READ salaryCurrency)
  Q_PROPERTY(double  rating READ rating)
  Q_PROPERTY(QDate   birthday READ birthday)

public:
  Contact(QObject* parent = 0);

  QString name() const;
  void setName(const QString &name);

  QString email() const;
  void setEmail(const QString &email);

  QString phone() const;
  void setPhone(const QString &phone);

  QString address() const;
  void setAddress(const QString &address);

  QString nickname() const;
  void setNickname(const QString& nickname);

  QString salaryCurrency() const;
  void setSalaryCurrency(const QString& salaryCurrency);

  double salary() const;
  void setSalary(double salary);

  double rating() const;
  void setRating(double rating);

  QDate birthday() const;
  void setBirthday(const QDate &birthday);

  /* reimp */ QVariant data(int role) const;

private:
  QString m_name;
  QString m_email;
  QString m_phone;
  QString m_address;
  QString m_nickname;
  QString m_salaryCurrency;
  double m_salary;
  double m_rating;
  QDate m_birthday;
};

#endif

