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

#include "qtlocalizer.h"


#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include <QtCore/QVector>

#include <QtCore/QLoggingCategory>

Q_LOGGING_CATEGORY(GRANTLEE_LOCALIZER, "grantlee.localizer")

struct Locale
{
  explicit Locale( const QLocale &_locale )
    : locale( _locale )
  {

  }

  ~Locale()
  {
    qDeleteAll( systemTranslators );
    qDeleteAll( themeTranslators );
  }

  const QLocale locale;
  QVector<QTranslator*> externalSystemTranslators; // Not owned by us!
  QVector<QTranslator*> systemTranslators;
  QVector<QTranslator*> themeTranslators;
};

namespace Grantlee
{

class QtLocalizerPrivate
{
  QtLocalizerPrivate( QtLocalizer *qq, const QLocale &locale )
    : q_ptr( qq )
  {
    Locale *localeStruct = new Locale( locale );
    m_availableLocales.insert( locale.name(), localeStruct );
    m_locales.push_back( localeStruct );
  }

  ~QtLocalizerPrivate()
  {
    m_locales.clear();
    qDeleteAll( m_availableLocales );
  }

  QLocale currentLocale() const
  {
    Q_ASSERT( !m_locales.isEmpty() );
    if ( m_locales.isEmpty() ) {
      qCWarning(GRANTLEE_LOCALIZER) << "Invalid Locale";
      return QLocale();
    }
    return m_locales.last()->locale;
  }

  Q_DECLARE_PUBLIC( QtLocalizer )
  QtLocalizer * const q_ptr;

  QString translate( const QString &input, const QString &context, int count = -1 ) const;

  QHash<QString, Locale*> m_availableLocales;

  QList<Locale*> m_locales;
  QString m_appTranslatorPath;
  QString m_appTranslatorPrefix;
};

}

using namespace Grantlee;

static void replacePercentN( QString *result, int n )
{
  if ( n >= 0 ) {
    int percentPos = 0;
    int len = 0;
    while ( ( percentPos = result->indexOf( QLatin1Char( '%' ), percentPos + len ) ) != -1 ) {
      len = 1;
      QString fmt;
      if ( result->at( percentPos + len ) == QLatin1Char( 'L' ) ) {
        ++len;
        fmt = QStringLiteral( "%L1" );
      } else {
        fmt = QStringLiteral( "%1" );
      }
      if ( result->at( percentPos + len ) == QLatin1Char( 'n' ) ) {
        fmt = fmt.arg( n );
        ++len;
        result->replace( percentPos, len, fmt );
        len = fmt.length();
      }
    }
  }
}

QString QtLocalizerPrivate::translate( const QString& input, const QString& context, int count ) const
{
  QString result;

  if ( m_locales.isEmpty() ) {
    result = input;
    replacePercentN( &result, count );
    return result;
  }

  Locale *locale = m_locales.last();
  Q_FOREACH( QTranslator *translator, locale->themeTranslators ) {
    result = translator->translate( "GR_FILENAME", input.toLatin1().constData(), context.toLatin1().constData(), count );
  }
  if ( result.isEmpty() ) {
    QVector<QTranslator*> translators = locale->externalSystemTranslators + locale->systemTranslators;
    if ( translators.isEmpty() )
      return QCoreApplication::translate( "GR_FILENAME", input.toLatin1().constData(), context.toLatin1().constData(), count );
    Q_FOREACH( QTranslator *translator, translators ) {
      result = translator->translate( "GR_FILENAME", input.toLatin1().constData(), context.toLatin1().constData(), count );
      if ( !result.isEmpty() )
        break;
    }
  }
  if ( !result.isEmpty() ) {
    replacePercentN( &result, count );
    return result;
  }
  QString fallback = input;
  replacePercentN( &fallback, count );
  return fallback;
}

QtLocalizer::QtLocalizer( const QLocale &locale )
    : AbstractLocalizer(), d_ptr( new QtLocalizerPrivate( this, locale ) )
{

}

QtLocalizer::~QtLocalizer()
{
  delete d_ptr;
}

void QtLocalizer::setAppTranslatorPath( const QString& path )
{
  Q_D( QtLocalizer );
  d->m_appTranslatorPath = path;
}

void QtLocalizer::setAppTranslatorPrefix( const QString& prefix )
{
  Q_D( QtLocalizer );
  d->m_appTranslatorPrefix = prefix;
}

void QtLocalizer::installTranslator( QTranslator* translator, const QString &localeName )
{
  Q_D( QtLocalizer );
  if ( !d->m_availableLocales.contains( localeName ) ) {
    const QLocale namedLocale( localeName );
    d->m_availableLocales.insert( localeName, new Locale( namedLocale ) );
  }
  d->m_availableLocales[localeName]->externalSystemTranslators.prepend( translator );
}

QString QtLocalizer::localizeDate( const QDate& date, QLocale::FormatType formatType ) const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().toString( date, formatType );
}

QString QtLocalizer::localizeTime( const QTime& time, QLocale::FormatType formatType ) const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().toString( time, formatType );
}

QString QtLocalizer::localizeDateTime( const QDateTime& dateTime, QLocale::FormatType formatType ) const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().toString( dateTime, formatType );
}

QString QtLocalizer::localizeNumber( int number ) const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().toString( number );
}

QString QtLocalizer::localizeNumber( qreal number ) const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().toString( number, 'f', 2 );
}

QString QtLocalizer::localizeMonetaryValue( qreal value, const QString& currencyCode ) const
{
  Q_D( const QtLocalizer );
  QString currencySymbol = QStringLiteral( "$" );
  if ( currencyCode == QStringLiteral( "EUR" ) ) {
    currencySymbol = QChar( 0x20AC );
  } else if ( currencyCode == QStringLiteral( "GBP" ) ) {
    currencySymbol = QStringLiteral( "£" );
  } else {
    currencySymbol = currencyCode;
  }
  return currencySymbol + QLatin1Char( ' ' ) + d->currentLocale().toString( value, 'f', 2 );
}

static QString substituteArguments( const QString &input, const QVariantList &arguments )
{
  QString string = input;
  Q_FOREACH( const QVariant &arg, arguments ) {
    if ( arg.type() == QVariant::Int )
      string = string.arg( arg.toInt() );
    else if ( arg.type() == QVariant::Double )
      string = string.arg( arg.toDouble() );
    else if ( arg.type() == QVariant::DateTime )
      string = string.arg( arg.toDateTime().toString() );
    else
      string = string.arg( arg.toString() );
  }
  return string;
}

QString QtLocalizer::localizeContextString( const QString& string, const QString& context, const QVariantList &arguments ) const
{
  Q_D( const QtLocalizer );
  const QString translated = d->translate( string, context );
  return substituteArguments( translated, arguments );
}

QString QtLocalizer::localizeString( const QString& string, const QVariantList &arguments ) const
{
  Q_D( const QtLocalizer );
  const QString translated = d->translate( string, QString() );
  return substituteArguments( translated, arguments );
}

QString QtLocalizer::localizePluralContextString( const QString& string, const QString& pluralForm, const QString& context, const QVariantList &_arguments ) const
{
  Q_UNUSED( pluralForm )
  Q_D( const QtLocalizer );
  QVariantList arguments = _arguments;
  const int N = arguments.takeFirst().toInt();
  const QString translated = d->translate( string, context, N );
  return substituteArguments( translated, arguments );
}

QString QtLocalizer::localizePluralString( const QString& string, const QString& pluralForm, const QVariantList &_arguments ) const
{
  Q_UNUSED( pluralForm )
  Q_D( const QtLocalizer );
  QVariantList arguments = _arguments;
  const int N = arguments.takeFirst().toInt();
  const QString translated = d->translate( string, QString(), N );
  return substituteArguments( translated, arguments );
}

QString QtLocalizer::currentLocale() const
{
  Q_D( const QtLocalizer );
  return d->currentLocale().name();
}

void QtLocalizer::pushLocale( const QString& localeName )
{
  Q_D( QtLocalizer );
  Locale *localeStruct = 0;
  if ( !d->m_availableLocales.contains( localeName ) ) {
    localeStruct = new Locale( QLocale( localeName ) );
    QTranslator *qtTranslator = new QTranslator;
    qtTranslator->load( QStringLiteral( "qt_" ) + localeName,
                        QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    localeStruct->systemTranslators.append( qtTranslator );
    QTranslator *appTranslator = new QTranslator;
    appTranslator->load( d->m_appTranslatorPrefix + localeName, d->m_appTranslatorPath );
    localeStruct->systemTranslators.append( appTranslator );
    d->m_availableLocales.insert( localeName, localeStruct );
  } else {
    localeStruct = d->m_availableLocales[ localeName ];
  }
  Q_ASSERT( localeStruct );
  d->m_locales.push_back( localeStruct );
}

void QtLocalizer::popLocale()
{
  Q_D( QtLocalizer );
  Q_ASSERT( !d->m_locales.isEmpty() );
  d->m_locales.takeLast();
}

void QtLocalizer::loadCatalog( const QString &path, const QString& catalog )
{
  Q_D( QtLocalizer );
  QHash< QString, Locale* >::iterator it = d->m_availableLocales.begin();
  const QHash< QString, Locale* >::iterator end = d->m_availableLocales.end();
  for ( ; it != end; ++it ) {
    QTranslator *translator = new QTranslator();
    const bool loaded = translator->load( it.key() + QLatin1Char( '/' ) + catalog, path );
    if ( !loaded )
      continue;

    translator->setObjectName( catalog );

    it.value()->themeTranslators.prepend( translator );
  }
}

void QtLocalizer::unloadCatalog( const QString& catalog )
{
  Q_D( QtLocalizer );
  QHash< QString, Locale* >::const_iterator it = d->m_availableLocales.constBegin();
  const QHash< QString, Locale* >::const_iterator end = d->m_availableLocales.constEnd();
  for ( ; it != end; ++it ) {
    QVector<QTranslator*>::iterator tranIt = ( *it )->themeTranslators.begin();
    while ( tranIt != ( *it )->themeTranslators.end() ) {
      if ( ( *tranIt )->objectName() == catalog ) {
        delete *tranIt;
        tranIt = ( *it )->themeTranslators.erase( tranIt );
      } else {
        ++tranIt;
      }
    }
  }
}
