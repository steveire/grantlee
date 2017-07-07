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

#include "grantlee_templates_export.h"

#include <QtCore/QLocale>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariantList>

class QDateTime;

namespace Grantlee
{

/**
  @brief Interface for implementing an internationalization system.

  This class defines an interface for accessing an internationalization, such as
  QLocale/QTranslator or gettext/KLocale.
*/
class GRANTLEE_TEMPLATES_EXPORT AbstractLocalizer
{
public:
  /**
   Constructor
  */
  AbstractLocalizer();

  /**
   Destructor
  */
  virtual ~AbstractLocalizer();

  /**
    Processes the data in @p variant into a localized string.

    The base implementation can localize numbers, dates and times, and
    strings.
  */
  virtual QString localize(const QVariant &variant) const;

  /**
    Returns the current locale as a string.
  */
  virtual QString currentLocale() const = 0;

  /**
    Makes this localizer use the locale specified by @p localeName for output.
  */
  virtual void pushLocale(const QString &localeName) = 0;

  /**
    Makes this localizer use the previous locale for output.
   */
  virtual void popLocale() = 0;

  /**
    Loads the @p catalog from @p path.
  */
  virtual void loadCatalog(const QString &path, const QString &catalog) = 0;

  /**
    Unloads the @p catalog
  */
  virtual void unloadCatalog(const QString &catalog) = 0;

  /**
    Localizes @p number
  */
  virtual QString localizeNumber(int number) const = 0;

  /**
    Localizes @p number
  */
  virtual QString localizeNumber(qreal number) const = 0;

  /**
    Localizes @p value as a monetary value in the currency specified by @p
    currencyCode.
  */
  virtual QString localizeMonetaryValue(qreal value, const QString &currencyCode
                                                     = QString()) const = 0;

  /**
    Localizes @p date with the specified @p formatType
  */
  virtual QString localizeDate(const QDate &date,
                               QLocale::FormatType formatType
                               = QLocale::ShortFormat) const = 0;

  /**
    Localizes @p time with the specified @p formatType
  */
  virtual QString localizeTime(const QTime &time,
                               QLocale::FormatType formatType
                               = QLocale::ShortFormat) const = 0;

  /**
    Localizes @p dateTime with the specified @p formatType
  */
  virtual QString localizeDateTime(const QDateTime &dateTime,
                                   QLocale::FormatType formatType
                                   = QLocale::ShortFormat) const = 0;

  /**
    Localizes @p string with the specified @p arguments for substitution
  */
  virtual QString localizeString(const QString &string,
                                 const QVariantList &arguments
                                 = QVariantList()) const = 0;

  /**
    Localizes @p string, disambiguated by @p context with the specified @p
    arguments for substitution
  */
  virtual QString localizeContextString(const QString &string,
                                        const QString &context,
                                        const QVariantList &arguments
                                        = QVariantList()) const = 0;

  /**
    Localizes @p string or its @p pluralForm with the specified @p arguments
    for
    substitution
  */
  virtual QString localizePluralString(const QString &string,
                                       const QString &pluralForm,
                                       const QVariantList &arguments
                                       = QVariantList()) const = 0;

  /**
    Localizes @p string or its @p pluralForm, disambiguated by @p context with
    the specified @p arguments for substitution
  */
  virtual QString localizePluralContextString(const QString &string,
                                              const QString &pluralForm,
                                              const QString &context,
                                              const QVariantList &arguments
                                              = QVariantList()) const = 0;

  /**
   * Localizes @p size as file size value where @p precision defines the decimal
   * precision and @p unitSystem the used unit system. For @p unitSystem @c 2 and @c 10
   * can be used. While @c 2 will output with binary prefix like KiB and MiB, @c 10 will
   * output in decimal notation like kB and MB. Default unit system should be decimal (10).
   * Because @size has to be the byte value, you can use the @p multiplier to convert it
   * into byte if it is for example only available as KiB value - then simply use a
   * multiplier of 1024 if you want to have it with binary prefix.
   *
   * Internally you can use calcFileSize() from util.h to get the size and the unit.
   */
  virtual QString localizeFileSize(qreal size,
                                   int precision,
                                   int unitSystem,
                                   qreal multiplier) const = 0;

private:
  Q_DISABLE_COPY(AbstractLocalizer)
};
}

#endif
