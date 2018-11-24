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

#include "engine.h"
#include "grantlee_paths.h"
#include "nulllocalizer_p.h"
#include "qtlocalizer.h"
#include "util.h"

#include "coverageobject.h"
#include <QtCore/QScopedPointer>
#include <QtCore/QTranslator>
#include <QtTest/QTest>

using namespace Grantlee;

#define INIT_LOCALIZER(localizer)                                              \
  localizer->setAppTranslatorPrefix(QStringLiteral("test_"));                  \
  localizer->setAppTranslatorPath(QStringLiteral(":/"));

class TestInternationalization : public CoverageObject
{
  Q_OBJECT
public:
  explicit TestInternationalization(QObject *parent = 0)
      : CoverageObject(parent), nullLocalizer(new NullLocalizer()),
        deLocalizer(
            new QtLocalizer(QLocale(QLocale::German, QLocale::Germany))),
        frLocalizer(new QtLocalizer(QLocale(QLocale::French, QLocale::France))),
        en_GBLocalizer(
            new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedKingdom))),
        en_USLocalizer(
            new QtLocalizer(QLocale(QLocale::English, QLocale::UnitedStates))),
        m_engine(new Engine(this))
  {
    auto cLocale = QLocale::c();
    cLocale.setNumberOptions(QLocale::OmitGroupSeparator);
    cLocalizer.reset(new QtLocalizer(cLocale));
    m_engine->setPluginPaths(QStringList()
                             << QStringLiteral(GRANTLEE_PLUGIN_PATH));
    INIT_LOCALIZER(cLocalizer)
    INIT_LOCALIZER(deLocalizer)
    INIT_LOCALIZER(frLocalizer)
    INIT_LOCALIZER(en_GBLocalizer)
    INIT_LOCALIZER(en_USLocalizer)
    {
      auto deTranslator = new QTranslator(this);
      const auto result = deTranslator->load(QStringLiteral(":/test_de_DE"));
      Q_UNUSED(result);
      Q_ASSERT(result);
      deLocalizer->installTranslator(deTranslator, QStringLiteral("de_DE"));
    }

    {
      auto frTranslator = new QTranslator(this);
      const auto result = frTranslator->load(QStringLiteral(":/test_fr_FR.qm"));
      Q_UNUSED(result);
      Q_ASSERT(result);
      frLocalizer->installTranslator(frTranslator, QStringLiteral("fr_FR"));
    }
    m_engine->addDefaultLibrary(QStringLiteral("grantlee_i18ntags"));
  }

private Q_SLOTS:
  void initTestCase();

  void testContext();
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
  QSharedPointer<QtLocalizer> cLocalizer;
  const QSharedPointer<AbstractLocalizer> nullLocalizer;
  const QSharedPointer<QtLocalizer> deLocalizer;
  const QSharedPointer<QtLocalizer> frLocalizer;
  const QSharedPointer<QtLocalizer> en_GBLocalizer;
  const QSharedPointer<QtLocalizer> en_USLocalizer;

  Grantlee::Engine *m_engine;
};

void TestInternationalization::initTestCase() { Q_INIT_RESOURCE(testresource); }

void TestInternationalization::testContext()
{
  Context c;
  QVERIFY(c.localizer());
  c.setLocalizer(cLocalizer);
  QVERIFY(c.localizer());
  c.setLocalizer(QSharedPointer<Grantlee::AbstractLocalizer>());
  QVERIFY(c.localizer());
}

void TestInternationalization::testStrings()
{
  QFETCH(QString, input);
  QFETCH(QString, null_output);
  QFETCH(QString, de_output);
  QFETCH(QString, fr_output);
  QFETCH(QString, disambiguation);
  QFETCH(QString, plural);
  QFETCH(QVariantList, args);

  if (!disambiguation.isEmpty()) {
    if (!plural.isEmpty()) {
      QCOMPARE(nullLocalizer->localizePluralContextString(input, plural,
                                                          disambiguation, args),
               null_output);
      QCOMPARE(deLocalizer->localizePluralContextString(input, plural,
                                                        disambiguation, args),
               de_output);
      QCOMPARE(frLocalizer->localizePluralContextString(input, plural,
                                                        disambiguation, args),
               fr_output);
      return;
    }
    QCOMPARE(nullLocalizer->localizeContextString(input, disambiguation, args),
             null_output);
    QCOMPARE(deLocalizer->localizeContextString(input, disambiguation, args),
             de_output);
    QCOMPARE(frLocalizer->localizeContextString(input, disambiguation, args),
             fr_output);
    return;
  }
  if (!plural.isEmpty()) {
    QCOMPARE(nullLocalizer->localizePluralString(input, plural, args),
             null_output);
    QCOMPARE(deLocalizer->localizePluralString(input, plural, args), de_output);
    QCOMPARE(frLocalizer->localizePluralString(input, plural, args), fr_output);
    return;
  }
  QCOMPARE(nullLocalizer->localizeString(input, args), null_output);
  QCOMPARE(deLocalizer->localizeString(input, args), de_output);
  QCOMPARE(frLocalizer->localizeString(input, args), fr_output);
}

void TestInternationalization::testStrings_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("null_output");
  QTest::addColumn<QString>("de_output");
  QTest::addColumn<QString>("fr_output");
  QTest::addColumn<QString>("disambiguation");
  QTest::addColumn<QString>("plural");
  QTest::addColumn<QVariantList>("args");

  // Translations here are not meant to be accurate, but are meant
  // only to test for example that disambiguation and arg reordering works.

  QTest::newRow("string-01")
      << "Birthday"
      << "Birthday"
      << "Geburtstag"
      << "Anniversaire" << QString() << QString() << QVariantList();

  QTest::newRow("string-02")
      << "%n People"
      << "1 People"
      << "1 Person"
      << "1 Personne" << QString() << QStringLiteral("%n People")
      << (QVariantList() << 1);

  QTest::newRow("string-03")
      << "%n People"
      << "2 People"
      << "2 Personen"
      << "2 Personnes" << QString() << QStringLiteral("%n People")
      << (QVariantList() << 2);

  QTest::newRow("string-04")
      << "Name"
      << "Name"
      << "Name eines Buches"
      << "Nom d'un livre" << QStringLiteral("Name of a Book") << QString()
      << QVariantList();

  QTest::newRow("string-05")
      << "Name"
      << "Name"
      << "Namen einer Person"
      << "Nom d'une personne" << QStringLiteral("Name of a Person") << QString()
      << QVariantList();

  QTest::newRow("string-06")
      << "%n People"
      << "1 People"
      << "1 Person angemeldet" << QStringLiteral("1 Personne connecté")
      << QStringLiteral("%n people are logged in")
      << QStringLiteral("%n People") << (QVariantList() << 1);

  QTest::newRow("string-07")
      << "%n People"
      << "2 People"
      << "2 Personen angemeldet" << QStringLiteral("2 Personnes connecté")
      << QStringLiteral("%n people are logged in")
      << QStringLiteral("%n People") << (QVariantList() << 2);

  QTest::newRow("string-08")
      << "%n file(s) copied to %1"
      << "1 files copied to destinationFolder"
      << "1 Datei in destinationFolder kopiert"
      << QStringLiteral("1 fichier copié dans destinationFolder") << QString()
      << QStringLiteral("%n files copied to %1")
      << (QVariantList() << 1 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-09")
      << "%n file(s) copied to %1"
      << "2 files copied to destinationFolder"
      << "2 Datein in destinationFolder kopiert"
      << QStringLiteral("2 fichiers copiés dans destinationFolder") << QString()
      << QStringLiteral("%n files copied to %1")
      << (QVariantList() << 2 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-10")
      << "%n to %1"
      << "1 copied to destinationFolder"
      << "1 Datei wird nach destinationFolder kopiert"
      << QStringLiteral("1 fichier est copié sur destinationFolder")
      << QStringLiteral("Files are being copied")
      << QStringLiteral("%n copied to %1")
      << (QVariantList() << 1 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-11")
      << "%n to %1"
      << "1 copied to destinationFolder"
      << "1 Datei war nach destinationFolder kopiert"
      << QStringLiteral("1 fichier a été copié à destinationFolder")
      << QStringLiteral("Files have already been copied")
      << QStringLiteral("%n copied to %1")
      << (QVariantList() << 1 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-12")
      << "%n to %1"
      << "2 copied to destinationFolder"
      << "2 Datein wird nach destinationFolder kopiert"
      << QStringLiteral("2 fichiers sont copiés à destinationFolder")
      << QStringLiteral("Files are being copied")
      << QStringLiteral("%n copied to %1")
      << (QVariantList() << 2 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-13")
      << "%n to %1"
      << "2 copied to destinationFolder"
      << "2 Datein war nach destinationFolder kopiert"
      << QStringLiteral("2 fichiers ont été copiés sur destinationFolder")
      << QStringLiteral("Files have already been copied")
      << QStringLiteral("%n copied to %1")
      << (QVariantList() << 2 << QStringLiteral("destinationFolder"));

  QTest::newRow("string-14")
      << "from %1 to %2"
      << "from sourceFolder to destinationFolder"
      << "nach destinationFolder von sourceFolder"
      << QStringLiteral("à partir de sourceFolder destinationFolder")
      << QStringLiteral("Files are being copied from %1 to %2") << QString()
      << (QVariantList() << QStringLiteral("sourceFolder")
                         << QStringLiteral("destinationFolder"));

  QTest::newRow("string-15")
      << "%1 messages at %2, fraction of total: %3. Rating : %4"
      << "1000 messages at 2005-05-07, fraction of total: 0.6. Rating : 4.8"
      << "1000 Nachrichten am 2005-05-07, ratio: 0.6. Bemessungen : 4.8"
      << QStringLiteral("1000 messages au 2005-05-07, la fraction du total: "
                        "0.6. Note: 4.8")
      << QString() << QString()
      << (QVariantList() << 1000 << QDate(2005, 5, 7) << 0.6 << 4.8);

#ifndef Q_CC_MSVC
  QTest::newRow("string-16")
      << "Snowman: ☃"
      << "Snowman: ☃"
      << "Schneemann: ☃" << QStringLiteral("bonhomme de neige: ☃") << QString()
      << QString() << QVariantList();
#endif
}

typedef QHash<QString, QVariant> Dict;

void TestInternationalization::testLocalizedTemplate()
{
  QFETCH(QString, input);
  QFETCH(QString, cFragment);
  QFETCH(QString, en_USFragment);
  QFETCH(QString, en_GBFragment);
  QFETCH(QString, deFragment);
  QFETCH(QString, frFragment);
  QFETCH(Dict, dict);

  auto t = m_engine->newTemplate(input, QString());
  Context c(dict);
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
  QTest::addColumn<QString>("cFragment");
  QTest::addColumn<QString>("en_USFragment");
  QTest::addColumn<QString>("en_GBFragment");
  QTest::addColumn<QString>("deFragment");
  QTest::addColumn<QString>("frFragment");
  QTest::addColumn<Dict>("dict");

  Dict dict;
  dict.insert(QStringLiteral("date"), QDate(2005, 5, 7));
  QTest::newRow("fragment-01")
      << QStringLiteral("{% i18n '%1 messages at %2, fraction of total: %3. "
                        "Rating : %4' _(1000) _(date) _(0.6) _(4.8) %}")
      << QStringLiteral("1000 messages at 7 May 2005, fraction of total: 0.60. "
                        "Rating : 4.80")
      << QStringLiteral(
             "1,000 messages at 5/7/05, fraction of total: 0.60. Rating : 4.80")
      << QStringLiteral("1,000 messages at 07/05/2005, fraction of total: "
                        "0.60. Rating : 4.80")
      << QStringLiteral(
             "1.000 Nachrichten am 07.05.05, ratio: 0,60. Bemessungen : 4,80")
      << QStringLiteral("1\u00A0000 messages au 07/05/2005, la fraction du "
                        "total: 0,60. Note: 4,80")
      << dict;

  dict.insert(QStringLiteral("integer"), 1000);
  dict.insert(QStringLiteral("smallFloat"), 0.6);
  dict.insert(QStringLiteral("largeFloat"), 4.8);

  QTest::newRow("fragment-02")
      << QStringLiteral(
             "{% i18n '%1 messages at %2, fraction of total: %3. Rating : %4' "
             "_(integer) _(date) _(smallFloat) _(largeFloat) %}")
      << QStringLiteral("1000 messages at 7 May 2005, fraction of total: 0.60. "
                        "Rating : 4.80")
      << QStringLiteral(
             "1,000 messages at 5/7/05, fraction of total: 0.60. Rating : 4.80")
      << QStringLiteral("1,000 messages at 07/05/2005, fraction of total: "
                        "0.60. Rating : 4.80")
      << QStringLiteral(
             "1.000 Nachrichten am 07.05.05, ratio: 0,60. Bemessungen : 4,80")
      << QStringLiteral("1\u00A0000 messages au 07/05/2005, la fraction du "
                        "total: 0,60. Note: 4,80")
      << dict;

  dict.insert(QStringLiteral("time"), QTime(4, 5, 6));
  dict.insert(QStringLiteral("dateTime"),
              QDateTime(QDate(2005, 5, 7), QTime(4, 5, 6)));

  QTest::newRow("fragment-03")
      << QStringLiteral(
             "{{ _(integer) }} -- {{ _(date) }} -- {{ _(smallFloat) }} -- {{ "
             "_(largeFloat) }} -- {{ _(time) }} -- {{ _(dateTime) }}")
      << QStringLiteral("1000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 "
                        "May 2005 04:05:06")
      << QStringLiteral(
             "1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 AM")
      << QStringLiteral(
             "1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 04:05")
      << QStringLiteral(
             "1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 04:05")
      << QStringLiteral("1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
                        "07/05/2005 04:05")
      << dict;

#ifndef Q_CC_MSVC
  QTest::newRow("fragment-04")
      << QString::fromLatin1(
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% with_locale 'de_DE' %}"
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% endwith_locale %}"
             "{% with_locale 'fr_FR' %}"
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% endwith_locale %}")
      << QString::fromUtf8(
             "Today"
             " -- 1000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 May "
             "2005 "
             "04:05:06"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05")
      << QString::fromUtf8(
             "Today"
             " -- 1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 "
             "AM"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05")
      << QString::fromUtf8(
             "Today"
             " -- 1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05")
      << QString::fromUtf8(
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05")
      << QString::fromUtf8(
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05")
      << dict;

  QTest::newRow("fragment-05")
      << QString::fromLatin1(
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% with_locale 'de_DE' %}"
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% with_locale 'fr_FR' %}"
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% endwith_locale %}"
             "{{ _('Today') }} -- {{ _(integer) }} -- {{ _(date) }} -- {{ "
             "_(smallFloat) }} -- {{ _(largeFloat) }} -- {{ _(time) }} -- {{ "
             "_(dateTime) }}"
             "{% endwith_locale %}")
      << QString::fromUtf8(
             "Today"
             " -- 1000 -- 7 May 2005 -- 0.60 -- 4.80 -- 04:05:06 -- 7 May "
             "2005 "
             "04:05:06"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05")
      << QString::fromUtf8(
             "Today"
             " -- 1,000 -- 5/7/05 -- 0.60 -- 4.80 -- 4:05 AM -- 5/7/05 4:05 "
             "AM"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05")
      << QString::fromUtf8(
             "Today"
             " -- 1,000 -- 07/05/2005 -- 0.60 -- 4.80 -- 04:05 -- 07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05")
      << QString::fromUtf8(
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05")
      << QString::fromUtf8(
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05"
             "Aujourd&#39;hui"
             " -- 1\u00A0000 -- 07/05/2005 -- 0,60 -- 4,80 -- 04:05 -- "
             "07/05/2005 "
             "04:05"
             "Heute"
             " -- 1.000 -- 07.05.05 -- 0,60 -- 4,80 -- 04:05 -- 07.05.05 "
             "04:05")
      << dict;
#endif

  dict.insert(QStringLiteral("list"), QVariantList() << 1000);
  QTest::newRow("fragment-06")
      << QStringLiteral("{{ _(list.0) }}") << QStringLiteral("1000")
      << QStringLiteral("1,000") << QStringLiteral("1,000")
      << QStringLiteral("1.000") << QStringLiteral("1\u00A0000") << dict;

  dict.clear();
  dict.insert(QStringLiteral("longlong"), (qlonglong)1000);
  dict.insert(QStringLiteral("float"), (float)0.6);
  dict.insert(QStringLiteral("double"), (float)4.8);
  dict.insert(QStringLiteral("hash"), QVariantHash());

  QTest::newRow("fragment-07")
      << QStringLiteral(
             "{{ _(longlong) }} {{ _(float) }} {{ _(double) }}{{ _(hash) }}")
      << QStringLiteral("1000 0.60 4.80") << QStringLiteral("1,000 0.60 4.80")
      << QStringLiteral("1,000 0.60 4.80") << QStringLiteral("1.000 0,60 4,80")
      << QStringLiteral("1\u00A0000 0,60 4,80") << dict;

  QTest::newRow("fragment-08")
      << QStringLiteral("{{ 'this'|cut:_(\"i\") }}") << QStringLiteral("ths")
      << QStringLiteral("ths") << QStringLiteral("ths") << QStringLiteral("ths")
      << QStringLiteral("ths") << dict;
}

void TestInternationalization::testSafeContent()
{
  QFETCH(QString, input);
  QFETCH(QString, cFragment);
  QFETCH(QString, frFragment);
  QFETCH(Dict, dict);

  auto t = m_engine->newTemplate(input, QString());
  Context c(dict);
  c.setLocalizer(cLocalizer);
  QCOMPARE(t->render(&c), cFragment);
  c.setLocalizer(frLocalizer);
  QCOMPARE(t->render(&c), frFragment);
}

void TestInternationalization::testSafeContent_data()
{
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("cFragment");
  QTest::addColumn<QString>("frFragment");
  QTest::addColumn<Dict>("dict");

  Dict dict;
  dict.insert(QStringLiteral("date"), QDate(2005, 5, 7));

  QTest::newRow("safe-01") << QStringLiteral("{% i18n 'Today is %1' _(date) %}")
                           << QStringLiteral("Today is 7 May 2005")
                           << QStringLiteral("Aujourd&#39;hui est 07/05/2005")
                           << dict;

  QTest::newRow("safe-02") << QStringLiteral(
      "{% autoescape off %}{% i18n 'Today is %1' _(date) %}{% endautoescape %}")
                           << QStringLiteral("Today is 7 May 2005")
                           << QStringLiteral("Aujourd'hui est 07/05/2005")
                           << dict;

  QTest::newRow("safe-03") << QStringLiteral(
      "{% i18n_var 'Today is %1' _(date) as today_greeting %}-{{ "
      "today_greeting }}-") << QStringLiteral("-Today is 7 May 2005-")
                           << QStringLiteral("-Aujourd&#39;hui est 07/05/2005-")
                           << dict;

  QTest::newRow("safe-04") << QStringLiteral(
      "{% autoescape off %}{% i18n_var 'Today is %1' _(date) as today_greeting "
      "%}-{{ today_greeting }}-{% endautoescape %}")
                           << QStringLiteral("-Today is 7 May 2005-")
                           << QStringLiteral("-Aujourd'hui est 07/05/2005-")
                           << dict;

  QTest::newRow("safe-05") << QStringLiteral(
      "{% with 'Today' as rawText %}-{{ _(rawText) }}-{% endwith %}")
                           << QStringLiteral("-Today-")
                           << QStringLiteral("-Aujourd&#39;hui-") << dict;

  QTest::newRow("safe-06") << QStringLiteral(
      "{% autoescape off %}{% with 'Today' as rawText %}-{{ _(rawText) }}-{% "
      "endwith %}{% endautoescape %}")
                           << QStringLiteral("-Today-")
                           << QStringLiteral("-Aujourd'hui-") << dict;

  dict.insert(QStringLiteral("today_text"), QStringLiteral("Today"));

  QTest::newRow("safe-07") << QStringLiteral("-{{ _(today_text) }}-")
                           << QStringLiteral("-Today-")
                           << QStringLiteral("-Aujourd&#39;hui-") << dict;

  QTest::newRow("safe-08") << QStringLiteral(
      "{% autoescape off %}-{{ _(today_text) }}-{% endautoescape %}")
                           << QStringLiteral("-Today-")
                           << QStringLiteral("-Aujourd'hui-") << dict;

  dict.insert(QStringLiteral("today_and_tomorrow_text"),
              markSafe(QStringLiteral("Today &amp; tomorrow")));

  QTest::newRow("safe-09")
      << QStringLiteral("-{{ _(today_and_tomorrow_text) }}-")
      << QStringLiteral("-Today &amp;amp; tomorrow-")
      << QStringLiteral("-Aujourd&#39;hui &amp;amp; demain-") << dict;

  QTest::newRow("safe-10") << QStringLiteral(
      "{% autoescape off %}-{{ _(today_and_tomorrow_text) }}-{% endautoescape "
      "%}") << QStringLiteral("-Today &amp; tomorrow-")
                           << QStringLiteral("-Aujourd'hui &amp; demain-")
                           << dict;

  QTest::newRow("safe-11") << QStringLiteral(
      "{% i18nc 'The current day is' 'Today is' %}")
                           << QStringLiteral("Today is")
                           << QStringLiteral("Aujourd&#39;hui est") << dict;

  QTest::newRow("safe-12") << QStringLiteral(
      "{% autoescape off %}{% i18nc 'The current day is' 'Today is' %}{% "
      "endautoescape %}") << QStringLiteral("Today is")
                           << QStringLiteral("Aujourd'hui est") << dict;

  QTest::newRow("safe-13") << QStringLiteral(
      "{% i18nc_var 'The current day is' 'Today is' as today_greeting %}-{{ "
      "today_greeting }}-") << QStringLiteral("-Today is-")
                           << QStringLiteral("-Aujourd&#39;hui est-") << dict;

  QTest::newRow("safe-14") << QStringLiteral(
      "{% autoescape off %}{% i18nc_var 'The current day is' 'Today is' as "
      "today_greeting %}-{{ today_greeting }}-{% endautoescape %}")
                           << QStringLiteral("-Today is-")
                           << QStringLiteral("-Aujourd'hui est-") << dict;

  QTest::newRow("safe-15")
      << QStringLiteral("{% i18np '%n people today' 1 %}")
      << QStringLiteral("1 people today") // Not really testing English here.
      << QStringLiteral("1 personne aujourd&#39;hui") << dict;

  QTest::newRow("safe-16") << QStringLiteral("{% i18np '%n people today' 2 %}")
                           << QStringLiteral("2 people today")
                           << QStringLiteral("2 personnes aujourd&#39;hui")
                           << dict;

  QTest::newRow("safe-17") << QStringLiteral(
      "{% autoescape off %}{% i18np '%n people today' 1 %}{% endautoescape %}")
                           << QStringLiteral(
                                  "1 people today") // Not really testing
                                                    // English here.
                           << QStringLiteral("1 personne aujourd'hui") << dict;

  QTest::newRow("safe-18") << QStringLiteral(
      "{% autoescape off %}{% i18np '%n people today' 2 %}{% endautoescape %}")
                           << QStringLiteral("2 people today")
                           << QStringLiteral("2 personnes aujourd'hui") << dict;

  QTest::newRow("safe-19") << QStringLiteral(
      "{% i18np_var '%n people today' 1 as num_people %}-{{ num_people }}-")
                           << QStringLiteral(
                                  "-1 people today-") // Not really testing
                                                      // English here.
                           << QStringLiteral("-1 personne aujourd&#39;hui-")
                           << dict;

  QTest::newRow("safe-20") << QStringLiteral(
      "{% i18np_var '%n people today' 2 as num_people %}-{{ num_people }}-")
                           << QStringLiteral("-2 people today-")
                           << QStringLiteral("-2 personnes aujourd&#39;hui-")
                           << dict;

  QTest::newRow("safe-21")
      << QStringLiteral("{% autoescape off %}{% i18np_var '%n people today' 1 "
                        "as num_people %}-{{ num_people }}-{% endautoescape %}")
      << QStringLiteral("-1 people today-") // Not really testing English here.
      << QStringLiteral("-1 personne aujourd'hui-") << dict;

  QTest::newRow("safe-22") << QStringLiteral(
      "{% autoescape off %}{% i18np_var '%n people today' 2 as num_people "
      "%}-{{ num_people }}-{% endautoescape %}")
                           << QStringLiteral("-2 people today-")
                           << QStringLiteral("-2 personnes aujourd'hui-")
                           << dict;

  QTest::newRow("safe-23")
      << QStringLiteral("{% i18ncp 'The number of people who have visited "
                        "today' '%n people visited today' 1 %}")
      << QStringLiteral(
             "1 people visited today") // Not really testing English here.
      << QStringLiteral("1 personne a visité aujourd&#39;hui") << dict;

  QTest::newRow("safe-24")
      << QStringLiteral("{% i18ncp 'The number of people who have visited "
                        "today' '%n people visited today' 2 %}")
      << QStringLiteral("2 people visited today")
      << QStringLiteral("2 personnes a visité aujourd&#39;hui") << dict;

  QTest::newRow("safe-25")
      << QStringLiteral(
             "{% autoescape off %}{% i18ncp 'The number of people who have "
             "visited today' '%n people visited today' 1 %}{% endautoescape %}")
      << QStringLiteral(
             "1 people visited today") // Not really testing English here.
      << QStringLiteral("1 personne a visité aujourd'hui") << dict;

  QTest::newRow("safe-26") << QStringLiteral(
      "{% autoescape off %}{% i18ncp 'The number of people who have visited "
      "today' '%n people visited today' 2 %}{% endautoescape %}")
                           << QStringLiteral("2 people visited today")
                           << QStringLiteral("2 personnes a visité aujourd'hui")
                           << dict;

  QTest::newRow("safe-27")
      << QStringLiteral(
             "{% i18ncp_var 'The number of people who have visited today' '%n "
             "people visited today' 1 as num_people %}-{{ num_people }}-")
      << QStringLiteral(
             "-1 people visited today-") // Not really testing English here.
      << QStringLiteral("-1 personne a visité aujourd&#39;hui-") << dict;

  QTest::newRow("safe-28") << QStringLiteral(
      "{% i18ncp_var 'The number of people who have visited today' '%n people "
      "visited today' 2 as num_people %}-{{ num_people }}-")
                           << QStringLiteral("-2 people visited today-")
                           << QStringLiteral(
                                  "-2 personnes a visité aujourd&#39;hui-")
                           << dict;

  QTest::newRow("safe-29")
      << QStringLiteral(
             "{% autoescape off %}{% i18ncp_var 'The number of people who have "
             "visited today' '%n people visited today' 1 as num_people %}-{{ "
             "num_people }}-{% endautoescape %}")
      << QStringLiteral(
             "-1 people visited today-") // Not really testing English here.
      << QStringLiteral("-1 personne a visité aujourd'hui-") << dict;

  QTest::newRow("safe-30") << QStringLiteral(
      "{% autoescape off %}{% i18ncp_var 'The number of people who have "
      "visited today' '%n people visited today' 2 as num_people %}-{{ "
      "num_people }}-{% endautoescape %}")
                           << QStringLiteral("-2 people visited today-")
                           << QStringLiteral(
                                  "-2 personnes a visité aujourd'hui-")
                           << dict;
}

void TestInternationalization::testFailure()
{
  QFETCH(QString, input);

  auto t = m_engine->newTemplate(input, QString());
  QVERIFY(t->error() != NoError);
}

void TestInternationalization::testFailure_data()
{
  QTest::addColumn<QString>("input");

  QTest::newRow("fail-01") << QStringLiteral(
      "{% i18np_var '%n people visited today' as num_people %}");
  QTest::newRow("fail-02") << QStringLiteral(
      "{% i18ncp_var 'The number of people who have visited today' '%n people "
      "visited today' as num_people %}");
  QTest::newRow("fail-03") << QStringLiteral(
      "{% i18np '%n people visited today' %}");
  QTest::newRow("fail-04") << QStringLiteral(
      "{% i18ncp 'The number of people who have visited today' '%n people "
      "visited today' %}");
}

void TestInternationalization::testDates()
{
  QFETCH(QDate, date);
  QFETCH(QString, cDate);
  QFETCH(QString, en_USDate);
  QFETCH(QString, en_GBDate);
  QFETCH(QString, deDate);
  QFETCH(QString, frDate);

  QCOMPARE(nullLocalizer->localizeDate(date), date.toString());
  QCOMPARE(cLocalizer->localizeDate(date), cDate);
  QCOMPARE(en_USLocalizer->localizeDate(date), en_USDate);
  QCOMPARE(en_GBLocalizer->localizeDate(date), en_GBDate);
  QCOMPARE(deLocalizer->localizeDate(date), deDate);
  QCOMPARE(frLocalizer->localizeDate(date), frDate);
}

void TestInternationalization::testDates_data()
{
  QTest::addColumn<QDate>("date");
  QTest::addColumn<QString>("cDate");
  QTest::addColumn<QString>("en_USDate");
  QTest::addColumn<QString>("en_GBDate");
  QTest::addColumn<QString>("deDate");
  QTest::addColumn<QString>("frDate");

  QTest::newRow("date-01") << QDate(2010, 5, 9) << QStringLiteral("9 May 2010")
                           << QStringLiteral("5/9/10")
                           << QStringLiteral("09/05/2010")
                           << QStringLiteral("09.05.10")
                           << QStringLiteral("09/05/2010");

  QTest::newRow("date-02") << QDate(2010, 10, 11)
                           << QStringLiteral("11 Oct 2010")
                           << QStringLiteral("10/11/10")
                           << QStringLiteral("11/10/2010")
                           << QStringLiteral("11.10.10")
                           << QStringLiteral("11/10/2010");
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
      << 7 << QStringLiteral("7") << QStringLiteral("7") << QStringLiteral("7")
      << QStringLiteral("7") << QStringLiteral("7") << QStringLiteral("7");

  QTest::newRow("integer-02")
      << 7000 << QStringLiteral("7000") << QStringLiteral("7000")
      << QStringLiteral("7,000") << QStringLiteral("7,000")
      << QStringLiteral("7.000") << QStringLiteral("7\u00A0000");
}

void TestInternationalization::testFloats()
{
  QFETCH(qreal, floatingPoint);
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
  QTest::addColumn<qreal>("floatingPoint");
  QTest::addColumn<QString>("nullFloatingPoint");
  QTest::addColumn<QString>("cFloatingPoint");
  QTest::addColumn<QString>("en_USFloatingPoint");
  QTest::addColumn<QString>("en_GBFloatingPoint");
  QTest::addColumn<QString>("deFloatingPoint");
  QTest::addColumn<QString>("frFloatingPoint");

  QTest::newRow("float-01")
      << qreal(7.4) << QStringLiteral("7.4") << QStringLiteral("7.40")
      << QStringLiteral("7.40") << QStringLiteral("7.40")
      << QStringLiteral("7,40") << QStringLiteral("7,40");
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

  QTest::newRow("time-01") << QTime(5, 6, 7) << QStringLiteral("05:06:07")
                           << QStringLiteral("05:06:07")
                           << QStringLiteral("5:06 AM")
                           << QStringLiteral("05:06") << QStringLiteral("05:06")
                           << QStringLiteral("05:06");

  QTest::newRow("time-02") << QTime(11, 12, 13) << QStringLiteral("11:12:13")
                           << QStringLiteral("11:12:13")
                           << QStringLiteral("11:12 AM")
                           << QStringLiteral("11:12") << QStringLiteral("11:12")
                           << QStringLiteral("11:12");

  QTest::newRow("time-03") << QTime(15, 12, 13) << QStringLiteral("15:12:13")
                           << QStringLiteral("15:12:13")
                           << QStringLiteral("3:12 PM")
                           << QStringLiteral("15:12") << QStringLiteral("15:12")
                           << QStringLiteral("15:12");
}

void TestInternationalization::testDateTimes()
{
  QFETCH(QDateTime, dateTime);
  QFETCH(QString, cDateTime);
  QFETCH(QString, en_USDateTime);
  QFETCH(QString, en_GBDateTime);
  QFETCH(QString, deDateTime);
  QFETCH(QString, frDateTime);

  QCOMPARE(nullLocalizer->localizeDateTime(dateTime), dateTime.toString());
  QCOMPARE(cLocalizer->localizeDateTime(dateTime), cDateTime);
  QCOMPARE(en_USLocalizer->localizeDateTime(dateTime), en_USDateTime);
  QCOMPARE(en_GBLocalizer->localizeDateTime(dateTime), en_GBDateTime);
  QCOMPARE(deLocalizer->localizeDateTime(dateTime), deDateTime);
  QCOMPARE(frLocalizer->localizeDateTime(dateTime), frDateTime);
}

void TestInternationalization::testDateTimes_data()
{
  QTest::addColumn<QDateTime>("dateTime");
  QTest::addColumn<QString>("cDateTime");
  QTest::addColumn<QString>("en_USDateTime");
  QTest::addColumn<QString>("en_GBDateTime");
  QTest::addColumn<QString>("deDateTime");
  QTest::addColumn<QString>("frDateTime");

  QTest::newRow("datetime-01")
      << QDateTime(QDate(2005, 6, 7), QTime(5, 6, 7))
      << QStringLiteral("7 Jun 2005 05:06:07")
      << QStringLiteral("6/7/05 5:06 AM") << QStringLiteral("07/06/2005 05:06")
      << QStringLiteral("07.06.05 05:06") << QStringLiteral("07/06/2005 05:06");

  QTest::newRow("datetime-02")
      << QDateTime(QDate(2005, 6, 7), QTime(11, 12, 13))
      << QStringLiteral("7 Jun 2005 11:12:13")
      << QStringLiteral("6/7/05 11:12 AM") << QStringLiteral("07/06/2005 11:12")
      << QStringLiteral("07.06.05 11:12") << QStringLiteral("07/06/2005 11:12");

  QTest::newRow("datetime-03")
      << QDateTime(QDate(2005, 6, 7), QTime(15, 12, 13))
      << QStringLiteral("7 Jun 2005 15:12:13")
      << QStringLiteral("6/7/05 3:12 PM") << QStringLiteral("07/06/2005 15:12")
      << QStringLiteral("07.06.05 15:12") << QStringLiteral("07/06/2005 15:12");

  QTest::newRow("datetime-04")
      << QDateTime(QDate(2005, 10, 11), QTime(5, 6, 7))
      << QStringLiteral("11 Oct 2005 05:06:07")
      << QStringLiteral("10/11/05 5:06 AM")
      << QStringLiteral("11/10/2005 05:06") << QStringLiteral("11.10.05 05:06")
      << QStringLiteral("11/10/2005 05:06");

  QTest::newRow("datetime-05")
      << QDateTime(QDate(2005, 10, 11), QTime(11, 12, 13))
      << QStringLiteral("11 Oct 2005 11:12:13")
      << QStringLiteral("10/11/05 11:12 AM")
      << QStringLiteral("11/10/2005 11:12") << QStringLiteral("11.10.05 11:12")
      << QStringLiteral("11/10/2005 11:12");

  QTest::newRow("datetime-06")
      << QDateTime(QDate(2005, 10, 11), QTime(15, 12, 13))
      << QStringLiteral("11 Oct 2005 15:12:13")
      << QStringLiteral("10/11/05 3:12 PM")
      << QStringLiteral("11/10/2005 15:12") << QStringLiteral("11.10.05 15:12")
      << QStringLiteral("11/10/2005 15:12");
}

QTEST_MAIN(TestInternationalization)
#include "testinternationalization.moc"
