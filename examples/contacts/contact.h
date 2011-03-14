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

class Address : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int houseNumber READ houseNumber)
  Q_PROPERTY(QString streetName READ streetName)
  Q_PROPERTY(QString city READ city)
public:
  Address(QObject *parent = 0);

  int houseNumber() const;
  void setHouseNumber(int houseNumber);

  QString streetName();
  void setStreetName(const QString &streetName);

  QString city();
  void setCity(const QString &city);

private:
  int m_houseNumber;
  QString m_streetName;
  QString m_city;
};

class Contact : public QObject, public QListWidgetItem
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(QString email READ email)
  Q_PROPERTY(QString phone READ phone)
  Q_PROPERTY(QObject* address READ address)
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

  QObject* address() const;
  void setAddress(Address *address);

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
  Address *m_address;
  QString m_nickname;
  QString m_salaryCurrency;
  double m_salary;
  double m_rating;
  QDate m_birthday;
};

Q_DECLARE_METATYPE(Contact*)
Q_DECLARE_METATYPE(QList<Contact*>)

#endif

