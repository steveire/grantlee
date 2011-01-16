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
#include "nulllocalizer_p.h"
#include "engine.h"
#include "util.h"

#include <QtCore/QScopedPointer>
#include <QtTest/QtTest>

using namespace Grantlee;

#define INIT_LOCALIZER(localizer)                                \
localizer->setAppTranslatorPrefix(QLatin1String( "test_" ) );    \
localizer->setAppTranslatorPath(QLatin1String( ":/" ) );         \

class TestInternationalization : public QObject
{
  Q_OBJECT
public:
  explicit TestInternationalization(QObject* parent = 0)
    : QObject(parent),
      cLocalizer(new QtLocalizer(QLocale::c())),
      nullLocalizer(new NullLocalizer()),
      deLocalizer(new QtLocalizer(QLocale(QLocale::German, QLocale::Germany))),
      frLocalizer(new QtLocalizer(QLocale(QLocale::French, QLocale::France))),
      en_GBLocalizer(new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedKingdom))),
      en_USLocalizer(new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedStates))),
      m_engine(new Engine(this))
  {
    INIT_LOCALIZER(cLocalizer)
    INIT_LOCALIZER(deLocalizer)
    INIT_LOCALIZER(frLocalizer)
    INIT_LOCALIZER(en_GBLocalizer)
    INIT_LOCALIZER(en_USLocalizer)
    {
      QTranslator *deTranslator = new QTranslator(this);
      const bool result = deTranslator->load( QLatin1String( ":/test_de_DE" ) );
      Q_ASSERT(result);
      deLocalizer->installTranslator(deTranslator, QLatin1String( "de_DE" ) );
    }

    {
      QTranslator *frTranslator = new QTranslator(this);
      const bool result = frTranslator->load( QLatin1String( ":/test_fr_FR.qm" ) );
      Q_ASSERT(result);
      frLocalizer->installTranslator(frTranslator, QLatin1String( "fr_FR" ) );
    }
    m_engine->addDefaultLibrary(QLatin1String("grantlee_i18ntags"));
  }

private slots:
  void testStrings();
  void testIntegers();
  void testFloats();
  void testDates();
  void testTimes();
  void testDateTimes();
  void testLocalizedTemplate();
  void testSafeContent();
  void testFailure();

  void testStrings_data();
  void testIntegers_data();
  void testFloats_data();
  void testDates_data();
  void testTimes_data();
  void testDateTimes_data();
  void testLocalizedTemplate_data();
  void testSafeContent_data();
  void testFailure_data();

private:
  const QSharedPointer<QtLocalizer> cLocalizer;
  const QSharedPointer<AbstractLocalizer> nullLocalizer;
  const QSharedPointer<QtLocalizer> deLocalizer;
  const QSharedPointer<QtLocalizer> frLocalizer;
  const QSharedPointer<QtLocalizer> en_GBLocalizer;
  const QSharedPointer<QtLocalizer> en_USLocalizer;

  Grantlee::Engine *m_engine;
};

void TestInternationalization::testStrings()
{
  QFETCH(QString, input);
  QFETCH(QString, de_output);
  QFETCH(QString, fr_output);
  QFETCH(QString, disambiguation);
  QFETCH(QString, plural);
  QFETCH(QVariantList, args);

  if (!disambiguation.isEmpty()) {
    if (!plural.isEmpty()) {
      QCOMPARE(deLocalizer->localizePluralContextString(input, plural, disambiguation, args), de_output);
      QCOMPARE(frLocalizer->localizePluralContextString(input, plural, disambiguation, args), fr_output);
      return;
    }
    QCOMPARE(deLocalizer->localizeContextString(input, disambiguation, args), de_output);
    QCOMPARE(frLocalizer->localizeContextString(input, disambiguation, args), fr_output);
    return;
  }
  if (!plural.isEmpty()) {
    QCOMPARE(deLocalizer->localizePluralString(input, plural, args), de_output);
    QCOMPARE(frLocalizer->localizePluralString(input, plural, args), fr_output);
    return;
  }
  QCOMPARE(deLocalizer->localizeString(input, args), de_output);
  QCOMPARE(frLocalizer->localizeString(input, args), fr_output);
}

void TestInternationalization::testStrings_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("de_output");
  QTest::addColumn<QString>("fr_output");
  QTest::addColumn<QString>("disambiguation");
  QTest::addColumn<QString>("plural");
  QTest::addColumn<QVariantList>("args");

  // Translations here are not meant to be accurate, but are meant
  // only to test for example that disambiguation and arg reordering works.

  QTest::newRow("string-01") << "Birthday"
                             << "Geburtstag"
                             << "Anniversaire"
                             << QString()
                             << QString()
                             << QVariantList();

  QTest::newRow("string-02") << "%n People"
                             << "1 Person"
                             << "1 Personne"
                             << QString()
                             << QString::fromLatin1("%n People")
                             << (QVariantList() << 1);

  QTest::newRow("string-03") << "%n People"
                             << "2 Personen"
                             << "2 Personnes"
                             << QString()
                             << QString::fromLatin1("%n People")
                             << (QVariantList() << 2);

  QTest::newRow("string-04") << "Name"
                             << "Name eines Buches"
                             << "Nom d'un livre"
                             << QString::fromLatin1("Name of a Book")
                             << QString()
                             << QVariantList();

  QTest::newRow("string-05") << "Name"
                             << "Namen einer Person"
                             << "Nom d'une personne"
                             << QString::fromLatin1("Name of a Person")
                             << QString()
                             << QVariantList();

  QTest::newRow("string-06") << "%n People"
                             << "1 Person angemeldet"
                             << QString::fromUtf8("1 Personne connecté")
                             << QString::fromLatin1("%n people are logged in")
                             << QString::fromLatin1("%n People")
                             << (QVariantList() << 1);

  QTest::newRow("string-07") << "%n People"
                             << "2 Personen angemeldet"
                             << QString::fromUtf8("2 Personnes connecté")
                             << QString::fromLatin1("%n people are logged in")
                             << QString::fromLatin1("%n People")
                             << (QVariantList() << 2);

  QTest::newRow("string-08") << "%n file(s) copied to %1"
                             << "1 Datei in destinationFolder kopiert"
                             << QString::fromUtf8("1 fichier copié dans destinationFolder")
                             << QString()
                             << QString::fromLatin1("%n files copied to %1")
                             << (QVariantList() << 1 << QString::fromLatin1("destinationFolder") );

  QTest::newRow("string-09") << "%n file(s) copied to %1"
                             << "2 Datein in destinationFolder kopiert"
                             << QString::fromUtf8("2 fichiers copiés dans destinationFolder")
                             << QString()
                             << QString::fromLatin1("%n files copied to %1")
                             << (QVariantList() << 2 << QString::fromLatin1("destinationFolder") );

  QTest::newRow("string-10") << "%n to %1"
                             << "1 Datei wird nach destinationFolder kopiert"
                             << QString::fromUtf8("1 fichier est copié sur destinationFolder")
                             << QString::fromLatin1("Files are being copied")
                             << QString::fromLatin1("%n copied to %1")
                             << (QVariantList() << 1 << QString::fromLatin1("destinationFolder"));

  QTest::newRow("string-11") << "%n to %1"
                             << "1 Datei war nach destinationFolder kopiert"
                             << QString::fromUtf8("1 fichier a été copié à destinationFolder")
                             << QString::fromLatin1("Files have already been copied")
                             << QString::fromLatin1("%n copied to %1")
                             << (QVariantList() << 1 << QString::fromLatin1("destinationFolder"));

  QTest::newRow("string-12") << "%n to %1"
                             << "2 Datein wird nach destinationFolder kopiert"
                             << QString::fromUtf8("2 fichiers sont copiés à destinationFolder")
                             << QString::fromLatin1("Files are being copied")
                             << QString::fromLatin1("%n copied to %1")
                             << (QVariantList() << 2 << QString::fromLatin1("destinationFolder"));

  QTest::newRow("string-13") << "%n to %1"
                             << "2 Datein war nach destinationFolder kopiert"
                             << QString::fromUtf8("2 fichiers ont été copiés sur destinationFolder")
                             << QString::fromLatin1("Files have already been copied")
                             << QString::fromLatin1("%n copied to %1")
                             << (QVariantList() << 2 << QString::fromLatin1("destinationFolder"));

  QTest::newRow("string-14") << "from %1 to %2"
                             << "nach destinationFolder von sourceFolder"
                             << QString::fromUtf8("à partir de sourceFolder destinationFolder")
                             << QString::fromLatin1("Files are being copied from %1 to %2")
                             << QString()
                             << (QVariantList() << QString::fromLatin1("sourceFolder") << QString::fromLatin1("destinationFolder"));

  QTest::newRow("string-15") << "%1 messages at %2, fraction of total: %3. Rating : %4"
                             << "1000 Nachrichten am 2005-05-07, ratio: 0.6. Bemessungen : 4.8"
                             << QString::fromUtf8("1000 messages au 2005-05-07, la fraction du total: 0.6. Note: 4.8")
                             << QString()
                             << QString()
                             << (QVariantList() << 1000 << QDate(2005, 5, 7) << 0.6 << 4.8 );
}

typedef QHash<QString, QVariant> Dict;
Q_DECLARE_METATYPE( Dict )

void TestInternationalization::testLocalizedTemplate()
{
  QFETCH(QString, input);
  QFETCH(QString, nullFragment);
  QFETCH(QString, cFragment);
  QFETCH(QString, en_USFragment);
  QFETCH(QString, en_GBFragment);
  QFETCH(QString, deFragment);
  QFETCH(QString, frFragment);
  QFETCH(Dict, dict);

  Template t = m_engine->newTemplate( input, QString());
  Context c(dict);
  QCOMPARE(t->render(&c), nullFragment);
  c.setLocalizer(cLocalizer);
  QCOMPARE(t->render(&c), cFragment);
  c.setLocalizer(en_USLocalizer);
  QCOMPARE(t->render(&c), en_USFragment);
  c.setLocalizer(en_GBLocalizer);
  QCOMPARE(t->render(&c), en_GBFragment);
  c.setLocalizer(deLocalizer);
  QCOMPARE(t->render(&c), deFragment);
  c.setLocalizer(frLocalizer);
  QCOMPARE(t->render(&c), frFragment);
}

void TestInternationalization::testLocalizedTemplate_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("nullFragment");
  QTest::addColumn<QString>("cFragment");
  QTest::addColumn<QString>("en_USFragment");
  QTest::addColumn<QString>("en_GBFragment");
  QTest::addColumn<QString>("deFragment");
  QTest::addColumn<QString>("frFragment");
  QTest::addColumn<Dict>("dict");

  Dict dict;
  dict.insert(QLatin1String("date"), QDate(2005, 5, 7) );
  QTest::newRow("fragment-01")
    << QString::fromLatin1("{% i18n '%1 messages at %2, fraction of total: %3. Rating : %4' _(1000) _(date) _(0.6) _(4.8) %}")
    << QString::fromLatin1("1000 messages at Sat May 7 2005, fraction of total: 0.6. Rating : 4.8")
    << QString::fromLatin1("1,000 messages at 7 May 2005, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1,000 messages at 5/7/05, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1,000 messages at 07/05/2005, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1.000 Nachrichten am 07.05.05, ratio: 0,60. Bemessungen : 4,80")
    << QString::fromUtf8("1 000 messages au 07/05/05, la fraction du total: 0,60. Note: 4,80")
    << dict;

  dict.insert(QLatin1String("integer"), 1000);
  dict.insert(QLatin1String("smallFloat"), 0.6);
  dict.insert(QLatin1String("largeFloat"), 4.8);

  QTest::newRow("fragment-02")
    << QString::fromLatin1("{% i18n '%1 messages at %2, fraction of total: %3. Rating : %4' _(integer) _(date) _(smallFloat) _(largeFloat) %}")
    << QString::fromLatin1("1000 messages at Sat May 7 2005, fraction of total: 0.6. Rating : 4.8")
    << QString::fromLatin1("1,000 messages at 7 May 2005, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1,000 messages at 5/7/05, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1,000 messages at 07/05/2005, fraction of total: 0.60. Rating : 4.80")
    << QString::fromLatin1("1.000 Nachrichten am 07.05.05, ratio: 0,60. Bemessungen : 4,80")
    << QString::fromUtf8("1 000 messages au 07/05/05, la fraction du total: 0,60. Note: 4,80")
    << dict;

  dict.insert(QLatin1String("time"), QTime(4, 5, 6));
  dict.insert(QLatin1String("dateTime"), QDateTime(QDate(2005, 5, 7), QTime(4, 5, 6)));

  QTest::newRow("fragment-03")
    << QString::fromLatin1("{{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}")
    << QString::fromLatin1("1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                          " 04:05:06 2005")
    << QString::fromLatin1("1,000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 May 2005 04:05:06")
    << QString::fromLatin1("1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 AM")
    << QString::fromLatin1("1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 04:05")
    << QString::fromLatin1("1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << QString::fromUtf8("1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << dict;

  QTest::newRow("fragment-04")
    << QString::fromLatin1(
      "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
      "{% with_locale 'de_DE' %}"
        "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
      "{% endwith_locale %}"
      "{% with_locale 'fr_FR' %}"
        "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
      "{% endwith_locale %}")
    << QString::fromLatin1("Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005"
                           "Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005"
                           "Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005")
    << QString::fromUtf8("Today"           " -- 1,000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 May 2005 04:05:06"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << QString::fromUtf8("Today"           " -- 1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 AM"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << QString::fromUtf8("Today"           " -- 1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << QString::fromUtf8("Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << QString::fromUtf8("Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05")
    << dict;

  QTest::newRow("fragment-05")
    << QString::fromLatin1(
      "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
      "{% with_locale 'de_DE' %}"
        "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
        "{% with_locale 'fr_FR' %}"
          "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
        "{% endwith_locale %}"
        "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}"
      "{% endwith_locale %}")
    << QString::fromLatin1("Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005"
                           "Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005"
                           "Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005"
                           "Today -- 1000 -- Sat May 7 2005 -- 0.6 -- 4.8 -- 04:05:06 -- "
#ifdef Q_OS_WIN
                                                                                "Sat 7. May"
#else
                                                                                "Sat May 7"
#endif
                                                                                " 04:05:06 2005")
    << QString::fromUtf8("Today"           " -- 1,000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 May 2005 04:05:06"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << QString::fromUtf8("Today"           " -- 1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 AM"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << QString::fromUtf8("Today"           " -- 1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << QString::fromUtf8("Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << QString::fromUtf8("Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05"
                         "Aujourd&#39;hui" " -- 1 000 -- 07/05/05 -- 0,60 -- 4,80 -- 04:05 -- 07/05/05 04:05"
                         "Heute"           " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
    << dict;
}

void TestInternationalization::testSafeContent()
{
  QFETCH(QString, input);
  QFETCH(QString, nullFragment);
  QFETCH(QString, frFragment);
  QFETCH(Dict, dict);

  Template t = m_engine->newTemplate( input, QString());
  Context c(dict);
  QCOMPARE(t->render(&c), nullFragment);
  c.setLocalizer(frLocalizer);
  QCOMPARE(t->render(&c), frFragment);

}

void TestInternationalization::testSafeContent_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("nullFragment");
  QTest::addColumn<QString>("frFragment");
  QTest::addColumn<Dict>("dict");

  Dict dict;
  dict.insert(QLatin1String("date"), QDate(2005, 5, 7) );

  QTest::newRow("safe-01")
    << QString::fromLatin1("{% i18n 'Today is %1' _(date) %}")
    << QString::fromLatin1("Today is Sat May 7 2005")
    << QString::fromUtf8("Aujourd&#39;hui est 07/05/05")
    << dict;

  QTest::newRow("safe-02")
    << QString::fromLatin1("{% autoescape off %}{% i18n 'Today is %1' _(date) %}{% endautoescape %}")
    << QString::fromLatin1("Today is Sat May 7 2005")
    << QString::fromUtf8("Aujourd'hui est 07/05/05")
    << dict;

  QTest::newRow("safe-03")
    << QString::fromLatin1("{% i18n_var 'Today is %1' _(date) as today_greeting %}-{{ today_greeting }}-")
    << QString::fromLatin1("-Today is Sat May 7 2005-")
    << QString::fromUtf8("-Aujourd&#39;hui est 07/05/05-")
    << dict;

  QTest::newRow("safe-04")
    << QString::fromLatin1("{% autoescape off %}{% i18n_var 'Today is %1' _(date) as today_greeting %}-{{ today_greeting }}-{% endautoescape %}")
    << QString::fromLatin1("-Today is Sat May 7 2005-")
    << QString::fromUtf8("-Aujourd'hui est 07/05/05-")
    << dict;

  QTest::newRow("safe-05")
    << QString::fromLatin1("{% with 'Today' as rawText %}-{{ _(rawText) }}-{% endwith %}")
    << QString::fromLatin1("-Today-")
    << QString::fromUtf8("-Aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-06")
    << QString::fromLatin1("{% autoescape off %}{% with 'Today' as rawText %}-{{ _(rawText) }}-{% endwith %}{% endautoescape %}")
    << QString::fromLatin1("-Today-")
    << QString::fromUtf8("-Aujourd'hui-")
    << dict;

  dict.insert( QLatin1String( "today_text" ), QLatin1String( "Today" ) );

  QTest::newRow("safe-07")
    << QString::fromLatin1("-{{ _(today_text) }}-")
    << QString::fromLatin1("-Today-")
    << QString::fromUtf8("-Aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-08")
    << QString::fromLatin1("{% autoescape off %}-{{ _(today_text) }}-{% endautoescape %}")
    << QString::fromLatin1("-Today-")
    << QString::fromUtf8("-Aujourd'hui-")
    << dict;

  dict.insert( QLatin1String( "today_and_tomorrow_text" ), markSafe( QString::fromLatin1( "Today &amp; tomorrow" ) ) );

  QTest::newRow("safe-09")
    << QString::fromLatin1("-{{ _(today_and_tomorrow_text) }}-")
    << QString::fromLatin1("-Today &amp;amp; tomorrow-")
    << QString::fromUtf8("-Aujourd&#39;hui &amp;amp; demain-")
    << dict;

  QTest::newRow("safe-10")
    << QString::fromLatin1("{% autoescape off %}-{{ _(today_and_tomorrow_text) }}-{% endautoescape %}")
    << QString::fromLatin1("-Today &amp; tomorrow-")
    << QString::fromUtf8("-Aujourd'hui &amp; demain-")
    << dict;

  QTest::newRow("safe-11")
    << QString::fromLatin1("{% i18nc 'The current day is' 'Today is' %}")
    << QString::fromLatin1("Today is")
    << QString::fromUtf8("Aujourd&#39;hui est")
    << dict;

  QTest::newRow("safe-12")
    << QString::fromLatin1("{% autoescape off %}{% i18nc 'The current day is' 'Today is' %}{% endautoescape %}")
    << QString::fromLatin1("Today is")
    << QString::fromUtf8("Aujourd'hui est")
    << dict;

  QTest::newRow("safe-13")
    << QString::fromLatin1("{% i18nc_var 'The current day is' 'Today is' as today_greeting %}-{{ today_greeting }}-")
    << QString::fromLatin1("-Today is-")
    << QString::fromUtf8("-Aujourd&#39;hui est-")
    << dict;

  QTest::newRow("safe-14")
    << QString::fromLatin1("{% autoescape off %}{% i18nc_var 'The current day is' 'Today is' as today_greeting %}-{{ today_greeting }}-{% endautoescape %}")
    << QString::fromLatin1("-Today is-")
    << QString::fromUtf8("-Aujourd'hui est-")
    << dict;

  QTest::newRow("safe-15")
    << QString::fromLatin1("{% i18np '%n people today' 1 %}")
    << QString::fromLatin1("1 people today") // Not really testing English here.
    << QString::fromUtf8("1 personne aujourd&#39;hui")
    << dict;

  QTest::newRow("safe-16")
    << QString::fromLatin1("{% i18np '%n people today' 2 %}")
    << QString::fromLatin1("2 people today")
    << QString::fromUtf8("2 personnes aujourd&#39;hui")
    << dict;

  QTest::newRow("safe-17")
    << QString::fromLatin1("{% autoescape off %}{% i18np '%n people today' 1 %}{% endautoescape %}")
    << QString::fromLatin1("1 people today") // Not really testing English here.
    << QString::fromUtf8("1 personne aujourd'hui")
    << dict;

  QTest::newRow("safe-18")
    << QString::fromLatin1("{% autoescape off %}{% i18np '%n people today' 2 %}{% endautoescape %}")
    << QString::fromLatin1("2 people today")
    << QString::fromUtf8("2 personnes aujourd'hui")
    << dict;

  QTest::newRow("safe-19")
    << QString::fromLatin1("{% i18np_var '%n people today' 1 as num_people %}-{{ num_people }}-")
    << QString::fromLatin1("-1 people today-") // Not really testing English here.
    << QString::fromUtf8("-1 personne aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-20")
    << QString::fromLatin1("{% i18np_var '%n people today' 2 as num_people %}-{{ num_people }}-")
    << QString::fromLatin1("-2 people today-")
    << QString::fromUtf8("-2 personnes aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-21")
    << QString::fromLatin1("{% autoescape off %}{% i18np_var '%n people today' 1 as num_people %}-{{ num_people }}-{% endautoescape %}")
    << QString::fromLatin1("-1 people today-") // Not really testing English here.
    << QString::fromUtf8("-1 personne aujourd'hui-")
    << dict;

  QTest::newRow("safe-22")
    << QString::fromLatin1("{% autoescape off %}{% i18np_var '%n people today' 2 as num_people %}-{{ num_people }}-{% endautoescape %}")
    << QString::fromLatin1("-2 people today-")
    << QString::fromUtf8("-2 personnes aujourd'hui-")
    << dict;

  QTest::newRow("safe-23")
    << QString::fromLatin1("{% i18ncp 'The number of people who have visited today' '%n people visited today' 1 %}")
    << QString::fromLatin1("1 people visited today") // Not really testing English here.
    << QString::fromUtf8("1 personne a visité aujourd&#39;hui")
    << dict;

  QTest::newRow("safe-24")
    << QString::fromLatin1("{% i18ncp 'The number of people who have visited today' '%n people visited today' 2 %}")
    << QString::fromLatin1("2 people visited today")
    << QString::fromUtf8("2 personnes a visité aujourd&#39;hui")
    << dict;

  QTest::newRow("safe-25")
    << QString::fromLatin1("{% autoescape off %}{% i18ncp 'The number of people who have visited today' '%n people visited today' 1 %}{% endautoescape %}")
    << QString::fromLatin1("1 people visited today") // Not really testing English here.
    << QString::fromUtf8("1 personne a visité aujourd'hui")
    << dict;

  QTest::newRow("safe-26")
    << QString::fromLatin1("{% autoescape off %}{% i18ncp 'The number of people who have visited today' '%n people visited today' 2 %}{% endautoescape %}")
    << QString::fromLatin1("2 people visited today")
    << QString::fromUtf8("2 personnes a visité aujourd'hui")
    << dict;

  QTest::newRow("safe-27")
    << QString::fromLatin1("{% i18ncp_var 'The number of people who have visited today' '%n people visited today' 1 as num_people %}-{{ num_people }}-")
    << QString::fromLatin1("-1 people visited today-") // Not really testing English here.
    << QString::fromUtf8("-1 personne a visité aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-28")
    << QString::fromLatin1("{% i18ncp_var 'The number of people who have visited today' '%n people visited today' 2 as num_people %}-{{ num_people }}-")
    << QString::fromLatin1("-2 people visited today-")
    << QString::fromUtf8("-2 personnes a visité aujourd&#39;hui-")
    << dict;

  QTest::newRow("safe-29")
    << QString::fromLatin1("{% autoescape off %}{% i18ncp_var 'The number of people who have visited today' '%n people visited today' 1 as num_people %}-{{ num_people }}-{% endautoescape %}")
    << QString::fromLatin1("-1 people visited today-") // Not really testing English here.
    << QString::fromUtf8("-1 personne a visité aujourd'hui-")
    << dict;

  QTest::newRow("safe-30")
    << QString::fromLatin1("{% autoescape off %}{% i18ncp_var 'The number of people who have visited today' '%n people visited today' 2 as num_people %}-{{ num_people }}-{% endautoescape %}")
    << QString::fromLatin1("-2 people visited today-")
    << QString::fromUtf8("-2 personnes a visité aujourd'hui-")
    << dict;
}

void TestInternationalization::testFailure()
{
  QFETCH(QString, input);

  Template t = m_engine->newTemplate( input, QString());
  QVERIFY(t->error() != NoError);
}

void TestInternationalization::testFailure_data()
{
  QTest::addColumn<QString>("input");

  QTest::newRow("fail-01")
    << QString::fromLatin1("{% i18np_var '%n people visited today' as num_people %}");
  QTest::newRow("fail-02")
    << QString::fromLatin1("{% i18ncp_var 'The number of people who have visited today' '%n people visited today' as num_people %}");
  QTest::newRow("fail-03")
    << QString::fromLatin1("{% i18np '%n people visited today' %}");
  QTest::newRow("fail-04")
    << QString::fromLatin1("{% i18ncp 'The number of people who have visited today' '%n people visited today' %}");
}

void TestInternationalization::testDates()
{
  QFETCH(QDate, date);
  QFETCH(QString, nullDate);
  QFETCH(QString, cDate);
  QFETCH(QString, en_USDate);
  QFETCH(QString, en_GBDate);
  QFETCH(QString, deDate);
  QFETCH(QString, frDate);

  QCOMPARE(nullLocalizer->localizeDate(date), nullDate);
  QCOMPARE(cLocalizer->localizeDate(date), cDate);
  QCOMPARE(en_USLocalizer->localizeDate(date), en_USDate);
  QCOMPARE(en_GBLocalizer->localizeDate(date), en_GBDate);
  QCOMPARE(deLocalizer->localizeDate(date), deDate);
  QCOMPARE(frLocalizer->localizeDate(date), frDate);
}

void TestInternationalization::testDates_data()
{
  QTest::addColumn<QDate>("date");
  QTest::addColumn<QString>("nullDate");
  QTest::addColumn<QString>("cDate");
  QTest::addColumn<QString>("en_USDate");
  QTest::addColumn<QString>("en_GBDate");
  QTest::addColumn<QString>("deDate");
  QTest::addColumn<QString>("frDate");

  QTest::newRow("date-01")
    << QDate(2010, 5, 9)
    << QString::fromLatin1("Sun May 9 2010")
    << QString::fromLatin1("9 May 2010")
    << QString::fromLatin1("5/9/10")
    << QString::fromLatin1("09/05/2010")
    << QString::fromLatin1("09.05.10")
    << QString::fromLatin1("09/05/10");

  QTest::newRow("date-02")
    << QDate(2010, 10, 11)
    << QString::fromLatin1("Mon Oct 11 2010")
    << QString::fromLatin1("11 Oct 2010")
    << QString::fromLatin1("10/11/10")
    << QString::fromLatin1("11/10/2010")
    << QString::fromLatin1("11.10.10")
    << QString::fromLatin1("11/10/10");
}

void TestInternationalization::testIntegers()
{
  QFETCH(int, integer);
  QFETCH(QString, nullInteger);
  QFETCH(QString, cInteger);
  QFETCH(QString, en_USInteger);
  QFETCH(QString, en_GBInteger);
  QFETCH(QString, deInteger);
  QFETCH(QString, frInteger);

  QCOMPARE(nullLocalizer->localizeNumber(integer), nullInteger);
  QCOMPARE(cLocalizer->localizeNumber(integer), cInteger);
  QCOMPARE(en_USLocalizer->localizeNumber(integer), en_USInteger);
  QCOMPARE(en_GBLocalizer->localizeNumber(integer), en_GBInteger);
  QCOMPARE(deLocalizer->localizeNumber(integer), deInteger);
  QCOMPARE(frLocalizer->localizeNumber(integer), frInteger);
}

void TestInternationalization::testIntegers_data()
{
  QTest::addColumn<int>("integer");
  QTest::addColumn<QString>("nullInteger");
  QTest::addColumn<QString>("cInteger");
  QTest::addColumn<QString>("en_USInteger");
  QTest::addColumn<QString>("en_GBInteger");
  QTest::addColumn<QString>("deInteger");
  QTest::addColumn<QString>("frInteger");

  QTest::newRow("integer-01")
    << 7
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7")
    << QString::fromLatin1("7");

  QTest::newRow("integer-02")
    << 7000
    << QString::fromLatin1("7000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7,000")
    << QString::fromLatin1("7.000")
    << QString::fromUtf8("7 000");
}

void TestInternationalization::testFloats()
{
  QFETCH(double, floatingPoint);
  QFETCH(QString, nullFloatingPoint);
  QFETCH(QString, cFloatingPoint);
  QFETCH(QString, en_USFloatingPoint);
  QFETCH(QString, en_GBFloatingPoint);
  QFETCH(QString, deFloatingPoint);
  QFETCH(QString, frFloatingPoint);

  QCOMPARE(nullLocalizer->localizeNumber(floatingPoint), nullFloatingPoint);
  QCOMPARE(cLocalizer->localizeNumber(floatingPoint), cFloatingPoint);
  QCOMPARE(en_USLocalizer->localizeNumber(floatingPoint), en_USFloatingPoint);
  QCOMPARE(en_GBLocalizer->localizeNumber(floatingPoint), en_GBFloatingPoint);
  QCOMPARE(deLocalizer->localizeNumber(floatingPoint), deFloatingPoint);
  QCOMPARE(frLocalizer->localizeNumber(floatingPoint), frFloatingPoint);
}

void TestInternationalization::testFloats_data()
{
  QTest::addColumn<double>("floatingPoint");
  QTest::addColumn<QString>("nullFloatingPoint");
  QTest::addColumn<QString>("cFloatingPoint");
  QTest::addColumn<QString>("en_USFloatingPoint");
  QTest::addColumn<QString>("en_GBFloatingPoint");
  QTest::addColumn<QString>("deFloatingPoint");
  QTest::addColumn<QString>("frFloatingPoint");

  QTest::newRow("float-01")
    << 7.4
    << QString::fromLatin1("7.4")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7.40")
    << QString::fromLatin1("7,40")
    << QString::fromLatin1("7,40");
}

void TestInternationalization::testTimes()
{
  QFETCH(QTime, time);
  QFETCH(QString, nullTime);
  QFETCH(QString, cTime);
  QFETCH(QString, en_USTime);
  QFETCH(QString, en_GBTime);
  QFETCH(QString, deTime);
  QFETCH(QString, frTime);

  QCOMPARE(nullLocalizer->localizeTime(time), nullTime);
  QCOMPARE(cLocalizer->localizeTime(time), cTime);
  QCOMPARE(en_USLocalizer->localizeTime(time), en_USTime);
  QCOMPARE(en_GBLocalizer->localizeTime(time), en_GBTime);
  QCOMPARE(deLocalizer->localizeTime(time), deTime);
  QCOMPARE(frLocalizer->localizeTime(time), frTime);
}

void TestInternationalization::testTimes_data()
{
  QTest::addColumn<QTime>("time");
  QTest::addColumn<QString>("nullTime");
  QTest::addColumn<QString>("cTime");
  QTest::addColumn<QString>("en_USTime");
  QTest::addColumn<QString>("en_GBTime");
  QTest::addColumn<QString>("deTime");
  QTest::addColumn<QString>("frTime");

  QTest::newRow("time-01")
    << QTime(5, 6, 7)
    << QString::fromLatin1("05:06:07")
    << QString::fromLatin1("05:06:07")
    << QString::fromLatin1("5:06 AM")
    << QString::fromLatin1("05:06")
    << QString::fromLatin1("05:06")
    << QString::fromLatin1("05:06");

  QTest::newRow("time-02")
    << QTime(11, 12, 13)
    << QString::fromLatin1("11:12:13")
    << QString::fromLatin1("11:12:13")
    << QString::fromLatin1("11:12 AM")
    << QString::fromLatin1("11:12")
    << QString::fromLatin1("11:12")
    << QString::fromLatin1("11:12");

  QTest::newRow("time-03")
    << QTime(15, 12, 13)
    << QString::fromLatin1("15:12:13")
    << QString::fromLatin1("15:12:13")
    << QString::fromLatin1("3:12 PM")
    << QString::fromLatin1("15:12")
    << QString::fromLatin1("15:12")
    << QString::fromLatin1("15:12");

}

void TestInternationalization::testDateTimes()
{
  QFETCH(QDateTime, dateTime);
  QFETCH(QString, nullDateTime);
  QFETCH(QString, cDateTime);
  QFETCH(QString, en_USDateTime);
  QFETCH(QString, en_GBDateTime);
  QFETCH(QString, deDateTime);
  QFETCH(QString, frDateTime);

  QCOMPARE(nullLocalizer->localizeDateTime(dateTime), nullDateTime);
  QCOMPARE(cLocalizer->localizeDateTime(dateTime), cDateTime);
  QCOMPARE(en_USLocalizer->localizeDateTime(dateTime), en_USDateTime);
  QCOMPARE(en_GBLocalizer->localizeDateTime(dateTime), en_GBDateTime);
  QCOMPARE(deLocalizer->localizeDateTime(dateTime), deDateTime);
  QCOMPARE(frLocalizer->localizeDateTime(dateTime), frDateTime);
}

void TestInternationalization::testDateTimes_data()
{
  QTest::addColumn<QDateTime>("dateTime");
  QTest::addColumn<QString>("nullDateTime");
  QTest::addColumn<QString>("cDateTime");
  QTest::addColumn<QString>("en_USDateTime");
  QTest::addColumn<QString>("en_GBDateTime");
  QTest::addColumn<QString>("deDateTime");
  QTest::addColumn<QString>("frDateTime");

  QTest::newRow("datetime-01")
    << QDateTime(QDate(2005, 6, 7), QTime(5, 6, 7))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
           "Tue 7. Jun 05:06:07 2005"
#else
           "Tue Jun 7 05:06:07 2005"
#endif
            )
    << QString::fromLatin1("7 Jun 2005 05:06:07")
    << QString::fromLatin1("6/7/05 5:06 AM")
    << QString::fromLatin1("07/06/2005 05:06")
    << QString::fromLatin1("07.06.05 05:06")
    << QString::fromLatin1("07/06/05 05:06");

  QTest::newRow("datetime-02")
    << QDateTime(QDate(2005, 6, 7), QTime(11, 12, 13))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
            "Tue 7. Jun 11:12:13 2005"
#else
            "Tue Jun 7 11:12:13 2005"
#endif
            )
    << QString::fromLatin1("7 Jun 2005 11:12:13")
    << QString::fromLatin1("6/7/05 11:12 AM")
    << QString::fromLatin1("07/06/2005 11:12")
    << QString::fromLatin1("07.06.05 11:12")
    << QString::fromLatin1("07/06/05 11:12");

  QTest::newRow("datetime-03")
    << QDateTime(QDate(2005, 6, 7), QTime(15, 12, 13))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
            "Tue 7. Jun 15:12:13 2005"
#else
            "Tue Jun 7 15:12:13 2005"
#endif
            )
    << QString::fromLatin1("7 Jun 2005 15:12:13")
    << QString::fromLatin1("6/7/05 3:12 PM")
    << QString::fromLatin1("07/06/2005 15:12")
    << QString::fromLatin1("07.06.05 15:12")
    << QString::fromLatin1("07/06/05 15:12");

  QTest::newRow("datetime-04")
    << QDateTime(QDate(2005, 10, 11), QTime(5, 6, 7))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
            "Tue 11. Oct 05:06:07 2005"
#else
            "Tue Oct 11 05:06:07 2005"
#endif
            )
    << QString::fromLatin1("11 Oct 2005 05:06:07")
    << QString::fromLatin1("10/11/05 5:06 AM")
    << QString::fromLatin1("11/10/2005 05:06")
    << QString::fromLatin1("11.10.05 05:06")
    << QString::fromLatin1("11/10/05 05:06");

  QTest::newRow("datetime-05")
    << QDateTime(QDate(2005, 10, 11), QTime(11, 12, 13))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
            "Tue 11. Oct 11:12:13 2005"
#else
            "Tue Oct 11 11:12:13 2005"
#endif
            )
    << QString::fromLatin1("11 Oct 2005 11:12:13")
    << QString::fromLatin1("10/11/05 11:12 AM")
    << QString::fromLatin1("11/10/2005 11:12")
    << QString::fromLatin1("11.10.05 11:12")
    << QString::fromLatin1("11/10/05 11:12");

  QTest::newRow("datetime-06")
    << QDateTime(QDate(2005, 10, 11), QTime(15, 12, 13))
    << QString::fromLatin1(
#ifdef Q_OS_WIN
            "Tue 11. Oct 15:12:13 2005"
#else
            "Tue Oct 11 15:12:13 2005"
#endif
            )
    << QString::fromLatin1("11 Oct 2005 15:12:13")
    << QString::fromLatin1("10/11/05 3:12 PM")
    << QString::fromLatin1("11/10/2005 15:12")
    << QString::fromLatin1("11.10.05 15:12")
    << QString::fromLatin1("11/10/05 15:12");

}


QTEST_MAIN( TestInternationalization )
#include "testinternationalization.moc"
