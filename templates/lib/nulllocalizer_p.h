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

#ifndef GRANTLEE_NULLLOCALIZER_P_H
#define GRANTLEE_NULLLOCALIZER_P_H

#include "abstractlocalizer.h"

namespace Grantlee
{

class GRANTLEE_TESTS_EXPORT NullLocalizer : public AbstractLocalizer
{
public:
  typedef QSharedPointer<NullLocalizer> Ptr;
  NullLocalizer();
  virtual ~NullLocalizer();

  virtual QString currentLocale() const;
  virtual void pushLocale(const QString &localeName);
  virtual void popLocale();
  virtual void loadCatalog( const QString &path, const QString &catalog );
  virtual void unloadCatalog( const QString &catalog );

  virtual QString localizeNumber( int number ) const;
  virtual QString localizeNumber( qreal number ) const;
  virtual QString localizeMonetaryValue( qreal value, const QString &currencyCode = QString() ) const;
  virtual QString localizeDate( const QDate &date, QLocale::FormatType formatType = QLocale::ShortFormat ) const;
  virtual QString localizeTime( const QTime &time, QLocale::FormatType formatType = QLocale::ShortFormat ) const;
  virtual QString localizeDateTime( const QDateTime& dateTime, QLocale::FormatType formatType = QLocale::ShortFormat ) const;
  virtual QString localizeString( const QString& string, const QVariantList &arguments = QVariantList() ) const;
  virtual QString localizeContextString( const QString& string, const QString& context, const QVariantList &arguments = QVariantList() ) const;
  virtual QString localizePluralContextString( const QString& string, const QString& pluralForm, const QString& context, const QVariantList &arguments = QVariantList() ) const;
  virtual QString localizePluralString( const QString& string, const QString& pluralForm, const QVariantList &arguments = QVariantList() ) const;
private:
  Q_DISABLE_COPY( NullLocalizer )
};

}

#endif
