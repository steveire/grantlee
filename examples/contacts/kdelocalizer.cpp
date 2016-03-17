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

#include "kdelocalizer.h"

#include <KDE/KComponentData>
#include <KDE/KGlobal>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <kcurrencycode.h>
#include <kstandarddirs.h>

#include <QtCore/QStack>

class KDELocalizerPrivate
{
  KDELocalizerPrivate(KDELocalizer *qq, KLocale *locale)
      : q_ptr(qq), m_locale(locale ? locale : KGlobal::locale())
  {
  }
  Q_DECLARE_PUBLIC(KDELocalizer)
  KDELocalizer *const q_ptr;

  QStringList m_applicationCatalogs;
  QStringList m_catalogs;
  QStack<KLocale *> m_localeStack;
  KLocale *m_locale;
};

KDELocalizer::KDELocalizer(KLocale *locale)
    : d_ptr(new KDELocalizerPrivate(this, locale))
{
}

KDELocalizer::~KDELocalizer() { delete d_ptr; }

void KDELocalizer::insertApplicationCatalogs(const QStringList &catalogs)
{
  Q_D(KDELocalizer);
  d->m_applicationCatalogs = catalogs;
  Q_FOREACH (const QString &catalog, d->m_applicationCatalogs) {
    d->m_locale->insertCatalog(catalog);
  }
}

QString KDELocalizer::currentLocale() const
{
  Q_D(const KDELocalizer);
  return d->m_locale->language();
}

void KDELocalizer::pushLocale(const QString &localeName)
{
  Q_D(KDELocalizer);
  d->m_localeStack.push(d->m_locale);
  const QStringList parts = localeName.split("_");
  QString country;
  if (parts.size() == 2)
    country = parts.at(1).toLower();
  d->m_locale = new KLocale(KGlobal::mainComponent().catalogName(), localeName,
                            country);
  Q_FOREACH (const QString &catalog, d->m_applicationCatalogs) {
    d->m_locale->insertCatalog(catalog);
  }
  Q_FOREACH (const QString &catalog, d->m_catalogs) {
    d->m_locale->insertCatalog(catalog);
  }
}

void KDELocalizer::popLocale()
{
  Q_D(KDELocalizer);
  Q_ASSERT(d->m_locale);
  delete d->m_locale;
  d->m_locale = d->m_localeStack.top();
  d->m_localeStack.pop();
}

void KDELocalizer::loadCatalog(const QString &path, const QString &catalog)
{
  Q_D(KDELocalizer);
  KGlobal::dirs()->addResourceDir("locale", path, true);
  d->m_catalogs.append(catalog);
  Q_ASSERT(d->m_localeStack.isEmpty());
  d->m_locale->insertCatalog(catalog);
}

void KDELocalizer::unloadCatalog(const QString &catalog)
{
  Q_D(KDELocalizer);
  //   Does not exist (probably not necessary either):
  // KGlobal::dirs()->removeResourceDir("locale", path + '/' + catalog, true);
  d->m_catalogs.removeAll(catalog);
  d->m_locale->removeCatalog(catalog);
}

QString KDELocalizer::localizeDate(const QDate &date,
                                   QLocale::FormatType formatType) const
{
  Q_D(const KDELocalizer);
  KLocale::DateFormat klocaleFormat = KLocale::ShortDate;
  if (formatType == QLocale::LongFormat)
    klocaleFormat = KLocale::LongDate;

  return d->m_locale->formatDate(date, klocaleFormat);
}

QString KDELocalizer::localizeTime(const QTime &time,
                                   QLocale::FormatType formatType) const
{
  Q_D(const KDELocalizer);
  KLocale::TimeFormatOptions klocaleFormat = KLocale::TimeDefault;
  if (formatType == QLocale::ShortFormat)
    klocaleFormat = KLocale::TimeWithoutSeconds;

  return d->m_locale->formatLocaleTime(time, klocaleFormat);
}

QString KDELocalizer::localizeDateTime(const QDateTime &dateTime,
                                       QLocale::FormatType formatType) const
{
  Q_D(const KDELocalizer);
  KLocale::DateFormat klocaleFormat = KLocale::ShortDate;
  if (formatType == QLocale::LongFormat)
    klocaleFormat = KLocale::LongDate;

  return d->m_locale->formatDateTime(dateTime, klocaleFormat);
}

QString KDELocalizer::localizeMonetaryValue(qreal value,
                                            const QString &currencyCode) const
{
  Q_D(const KDELocalizer);
  KCurrencyCode code(currencyCode);
  return d->m_locale->formatMoney(value, code.defaultSymbol());
}

QString KDELocalizer::localizeNumber(int number) const
{
  Q_D(const KDELocalizer);
  return d->m_locale->formatLong(number);
}

QString KDELocalizer::localizeNumber(qreal number) const
{
  Q_D(const KDELocalizer);
  return d->m_locale->formatNumber(number);
}

static KLocalizedString substituteArguments(const KLocalizedString &_string,
                                            const QVariantList &args)
{
  KLocalizedString string = _string;
  Q_FOREACH (const QVariant &arg, args) {
    if (arg.type() == QVariant::Int)
      string = string.subs(arg.toInt());
    else if (arg.type() == QVariant::Double)
      string = string.subs(arg.toDouble());
    else
      string = string.subs(arg.toString());
  }
  return string;
}

QString KDELocalizer::localizeContextString(const QString &string,
                                            const QString &context,
                                            const QVariantList &arguments) const
{
  Q_D(const KDELocalizer);
  KLocalizedString localizedString = ki18nc(context.toUtf8(), string.toUtf8());
  return substituteArguments(localizedString, arguments).toString(d->m_locale);
}

QString KDELocalizer::localizePluralContextString(
    const QString &string, const QString &pluralForm, const QString &context,
    const QVariantList &arguments) const
{
  Q_D(const KDELocalizer);
  KLocalizedString localizedString
      = ki18ncp(context.toUtf8(), string.toUtf8(), pluralForm.toUtf8());
  return substituteArguments(localizedString, arguments).toString(d->m_locale);
}

QString KDELocalizer::localizePluralString(const QString &string,
                                           const QString &pluralForm,
                                           const QVariantList &arguments) const
{
  Q_D(const KDELocalizer);
  KLocalizedString localizedString
      = ki18np(string.toUtf8(), pluralForm.toUtf8());
  return substituteArguments(localizedString, arguments).toString(d->m_locale);
}

QString KDELocalizer::localizeString(const QString &string,
                                     const QVariantList &arguments) const
{
  Q_D(const KDELocalizer);
  KLocalizedString localizedString = ki18n(string.toUtf8());
  return substituteArguments(localizedString, arguments).toString(d->m_locale);
}
