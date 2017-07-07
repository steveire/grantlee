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
  NullLocalizer();
  ~NullLocalizer() override;

  QString currentLocale() const override;
  void pushLocale(const QString &localeName) override;
  void popLocale() override;
  void loadCatalog(const QString &path, const QString &catalog) override;
  void unloadCatalog(const QString &catalog) override;

  QString localizeNumber(int number) const override;
  QString localizeNumber(qreal number) const override;
  QString localizeMonetaryValue(qreal value, const QString &currencyCode
                                             = QString()) const override;
  QString localizeDate(const QDate &date,
                       QLocale::FormatType formatType
                       = QLocale::ShortFormat) const override;
  QString localizeTime(const QTime &time,
                       QLocale::FormatType formatType
                       = QLocale::ShortFormat) const override;
  QString localizeDateTime(const QDateTime &dateTime,
                           QLocale::FormatType formatType
                           = QLocale::ShortFormat) const override;
  QString localizeString(const QString &string,
                         const QVariantList &arguments
                         = QVariantList()) const override;
  QString localizeContextString(const QString &string, const QString &context,
                                const QVariantList &arguments
                                = QVariantList()) const override;
  QString localizePluralContextString(const QString &string,
                                      const QString &pluralForm,
                                      const QString &context,
                                      const QVariantList &arguments
                                      = QVariantList()) const override;
  QString localizePluralString(const QString &string, const QString &pluralForm,
                               const QVariantList &arguments
                               = QVariantList()) const override;
  QString localizeFileSize(qreal size,
                           int precision,
                           int unitSystem,
                           qreal multiplier) const override;

private:
  Q_DISABLE_COPY(NullLocalizer)
};
}

#endif
