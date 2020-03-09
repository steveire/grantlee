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

  This class implements access to the %Qt Localization system. Localizable
  types such as strings, numbers, dates and times can be processed.

  @code
    auto t = m_engine->loadTemplate(someTemplate);
    auto c = getContext();

    // Render with the German locale
    c.setLocalizer(m_de_Localizer);
    auto deText = t->render(&c);
    de_display->setText(deText);

    // Render with the French locale
    c.setLocalizer(m_fr_Localizer);
    auto frText = t->render(&c);
    fr_display->setText(frText);
  @endcode

*/
class GRANTLEE_TEMPLATES_EXPORT QtLocalizer : public AbstractLocalizer
{
public:
  /**
    Constructs a new **%QtLocalizer** using the @p locale
  */
  QtLocalizer(const QLocale &locale = QLocale::system());

  /**
    Destructor
   */
  ~QtLocalizer() override;

  /**
    Set the path to look for translations of the application strings.
   */
  void setAppTranslatorPath(const QString &path);

  /**
    Set the prefix of translation files. For example, if the German translation
    file is called <tt>myapp_de_DE.qm</tt>, the prefix should be set to
    <tt>myapp_</tt>.
   */
  void setAppTranslatorPrefix(const QString &prefix);

  /**
    Install a @p translator to use for a particular @p localeName.

    @code
      auto deTranslator = new QTranslator(this);
      auto loaded = deTranslator->load("myapp_de_DE");
      if (!loaded)
        return;

      de_localizer->installTranslator(deTranslator, "de_DE");
    @endcode
   */
  void installTranslator(QTranslator *translator,
                         const QString &localeName = QLocale::system().name());

  QString currentLocale() const override;
  void pushLocale(const QString &localeName) override;
  void popLocale() override;
  void loadCatalog(const QString &path, const QString &catalog) override;
  void unloadCatalog(const QString &catalog) override;

  QString localizeNumber(int number) const override;
  QString localizeNumber(qreal number) const override;
  QString localizeMonetaryValue(qreal value, const QString &currencyCode
                                             = {}) const override;
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
                         const QVariantList &arguments = {}) const override;
  QString localizeContextString(const QString &string, const QString &context,
                                const QVariantList &arguments
                                = {}) const override;
  QString localizePluralContextString(const QString &string,
                                      const QString &pluralForm,
                                      const QString &context,
                                      const QVariantList &arguments
                                      = {}) const override;
  QString localizePluralString(const QString &string, const QString &pluralForm,
                               const QVariantList &arguments
                               = {}) const override;

private:
  Q_DECLARE_PRIVATE(QtLocalizer)
  Q_DISABLE_COPY(QtLocalizer)
  QtLocalizerPrivate *const d_ptr;
};
}

#endif
