#include "coverageobject.h"
#include <QTest>
#include <QMetaObject>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QtDebug>
#include <QLibrary>

#include "grantlee_paths.h"

void CoverageObject::init()
{
  initTest();
}

QString CoverageObject::generateTestName() const
{
  QString test_name;
  test_name+=QString::fromAscii(metaObject()->className());
  test_name+=QString::fromAscii("/");
  test_name+=QString::fromAscii(QTest::currentTestFunction());
  if (QTest::currentDataTag())
  {
    test_name+=QString::fromAscii("/");
    test_name+=QString::fromAscii(QTest::currentDataTag());
  }
  return test_name;
}

void CoverageObject::saveCoverageData()
{
#ifdef __COVERAGESCANNER__
  QString test_name;
  test_name=QString::fromAscii("core/");

  test_name += generateTestName();

  __coveragescanner_testname(test_name.toStdString().c_str());
  if (QTest::currentTestFailed())
    __coveragescanner_teststate("FAILED");
  else
    __coveragescanner_teststate("PASSED") ;
  __coveragescanner_save();
  __coveragescanner_testname("");
  __coveragescanner_clear();
#endif
}

void CoverageObject::saveCoverageData(const QString &plg)
{
#ifdef __COVERAGESCANNER__
  if (QFile::exists(plg))
  {
    QFileInfo plgInfo(plg);
    QString test_name = plgInfo.baseName() + QString::fromAscii("/") + generateTestName();
    QLibrary lib(plg);
    typedef void (*_coveragescanner_save)();
    typedef void (*_coveragescanner_clear)();
    typedef void (*_coveragescanner_testname)(const char*);
    typedef void (*_coveragescanner_teststate)(const char*);
    _coveragescanner_save coveragescanner_save=(_coveragescanner_save)lib.resolve("__coveragescanner_save");
    _coveragescanner_testname coveragescanner_testname=(_coveragescanner_testname)lib.resolve("__coveragescanner_testname");
    _coveragescanner_teststate coveragescanner_teststate=(_coveragescanner_teststate)lib.resolve("__coveragescanner_teststate");
    _coveragescanner_clear coveragescanner_clear=(_coveragescanner_clear)lib.resolve("__coveragescanner_clear");

    if (coveragescanner_testname)
      coveragescanner_testname(test_name.toStdString().c_str());
    else
      qDebug() << "Plugin "<<plg<< ": __coveragescanner_testname() symbol not existing";
    if (coveragescanner_teststate)
    {
      if (QTest::currentTestFailed())
        coveragescanner_teststate("FAILED");
      else
        coveragescanner_teststate("PASSED") ;
    }
    else
      qDebug() << "Plugin "<<plg<< ": __coveragescanner_teststate() symbol not existing";

    if (coveragescanner_save)
      coveragescanner_save();
    else
      qDebug() << "Plugin "<<plg<< ": __coveragescanner_save() symbol not existing";

    if (coveragescanner_clear)
      coveragescanner_clear();
    else
      qDebug() << "Plugin "<<plg<< ": __coveragescanner_clear() symbol not existing";
  }
  else
    qDebug() << "Plugin "<<plg<< " is not existing";
#else
  Q_UNUSED(plg);
#endif
}

void CoverageObject::cleanup()
{
  cleanupTest();
  saveCoverageData();

  saveCoverageData(QString::fromAscii(GRANTLEE_PLUGIN_PATH "/grantlee/0.2/grantlee_defaulttags.so"));
  saveCoverageData(QString::fromAscii(GRANTLEE_PLUGIN_PATH "/grantlee/0.2/grantlee_defaultfilters.so"));
  saveCoverageData(QString::fromAscii(GRANTLEE_PLUGIN_PATH "/grantlee/0.2/grantlee_loadertags.so"));
  saveCoverageData(QString::fromAscii(GRANTLEE_PLUGIN_PATH "/grantlee/0.2/grantlee_i18ntags.so"));
}

#include "coverageobject.moc"
