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

#ifndef GRANTLEE_QTLOCALIZER_P_H
#define GRANTLEE_QTLOCALIZER_P_H

#include "abstractlocalizer.h"

class QTranslator;

namespace Grantlee
{

class QtLocalizerPrivate;

/**
  @brief Provides internationalization based on QLocale and QTranslator.

  This class implements access to the Qt Localization system. Localizaable types such
  as strings, numbers, dates and times can be processed.

  @code
    Template t = m_engine->loadTemplate(someTemplate);
    Context c = getContext();

    // Render with the German locale
    c.setLocalizer(m_de_Localizer);
    QString deText = t->render(&c);
    de_display->setText(deText);

    // Render with the French locale
    c.setLocalizer(m_fr_Localizer);
    QString frText = t->render(&c);
    fr_display->setText(frText);
  @endcode

*/
class GRANTLEE_TEMPLATES_EXPORT QtLocalizer : public AbstractLocalizer
{
public:
#ifndef Q_QDOC
  typedef QSharedPointer<QtLocalizer> Ptr;
#endif
  /**
    Constructs a new QtLocalizer using the @p locale
  */
  QtLocalizer( const QLocale &locale = QLocale::system() );

  /**
    Destructor
   */
  virtual ~QtLocalizer();

  /**
    Set the path to look for translations of the application strings.
   */
  void setAppTranslatorPath( const QString &path );

  /**
    Set the prefix of translation files. For example, if the German translation file is called <tt>myapp_de_DE.qm</tt>,
    the prefix should be set to <tt>myapp_</tt>.
   */
  void setAppTranslatorPrefix( const QString &prefix );

  /**
    Install a @p translator to use for a particular @p localeName.

    @code
      QTranslator *deTranslator = new QTranslator(this);
      bool loaded = deTranslator->load("myapp_de_DE");
      if (!loaded)
        return;

      de_localizer->installTranslator(deTranslator, "de_DE");
    @endcode
   */
  void installTranslator( QTranslator *translator, const QString &localeName = QLocale::system().name() );

  virtual QString currentLocale() const;
  virtual void pushLocale( const QString &localeName );
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
  Q_DECLARE_PRIVATE( QtLocalizer )
  Q_DISABLE_COPY( QtLocalizer )
  QtLocalizerPrivate * const d_ptr;
};

}

#endif
