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

#ifndef GRANTLEE_ABSTRACTLOCALIZER_H
#define GRANTLEE_ABSTRACTLOCALIZER_H

#include "grantlee_core_export.h"

#include <QtCore/QLocale>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariantList>

class QDateTime;

namespace Grantlee
{

class GRANTLEE_CORE_EXPORT AbstractLocalizer
{
public:
  typedef QSharedPointer<AbstractLocalizer> Ptr;

  AbstractLocalizer();
  virtual QString localize( const QVariant &variant ) const;

  virtual ~AbstractLocalizer();

  virtual QString currentLocale() const = 0;
  virtual void pushLocale( const QString &localeName ) = 0;
  virtual void popLocale() = 0;
  virtual void loadCatalog( const QString &path, const QString &catalog ) = 0;
  virtual void unloadCatalog( const QString &catalog ) = 0;

  virtual QString localizeNumber( int number ) const = 0;
  virtual QString localizeNumber( qreal number ) const = 0;
  virtual QString localizeMonetaryValue( qreal value, const QString &currencyCode = QString() ) const = 0;
  virtual QString localizeDate( const QDate &date, QLocale::FormatType formatType = QLocale::ShortFormat ) const = 0;
  virtual QString localizeTime( const QTime &time, QLocale::FormatType formatType = QLocale::ShortFormat ) const = 0;
  virtual QString localizeDateTime( const QDateTime &dateTime, QLocale::FormatType formatType = QLocale::ShortFormat ) const = 0;
  virtual QString localizeString( const QString &string, const QVariantList &arguments = QVariantList() ) const = 0;
  virtual QString localizeContextString( const QString &string, const QString &context, const QVariantList &arguments = QVariantList() ) const = 0;
  virtual QString localizePluralString( const QString &string, const QString& pluralForm, const QVariantList &arguments = QVariantList() ) const = 0;
  virtual QString localizePluralContextString( const QString &string, const QString& pluralForm, const QString &context, const QVariantList &arguments = QVariantList() ) const = 0;
private:
  Q_DISABLE_COPY( AbstractLocalizer )
};

}

#endif

