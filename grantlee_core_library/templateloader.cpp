
#include "templateloader.h"

#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QFile>

TemplateLoader* TemplateLoader::m_instance = 0;
TemplateLoader* TemplateLoader::instance()
{
  if (!m_instance)
  {
    m_instance = new TemplateLoader();
  }
  return m_instance;
}

TemplateLoader::TemplateLoader()
{
  m_defaultLibraries << "grantlee_defaulttags_library"
                     << "grantlee_loadertags_library"
                     << "grantlee_defaultfilters_library"
                     << "grantlee_scriptabletags_library";
}

void TemplateLoader::setTemplateDirs(const QStringList &dirs)
{
  m_templateDirs = dirs;
}

void TemplateLoader::setPluginDirs(const QStringList &dirs)
{
  m_pluginDirs = dirs;
}

QStringList TemplateLoader::defaultLibraries() const
{
  return m_defaultLibraries;
}

void TemplateLoader::setDefaultLibraries(const QStringList &list)
{
  m_defaultLibraries = list;
}

void TemplateLoader::addDefaultLibrary(const QString &libName)
{
  m_defaultLibraries << libName;
}

void TemplateLoader::removeDefaultLibrary(const QString &libName)
{
  m_defaultLibraries.removeAll(libName);

}


void TemplateLoader::setTheme(const QString &themeName)
{
  m_themeName = themeName;
}

Template* TemplateLoader::getTemplate(QObject *parent) const
{
  return new Template(m_pluginDirs, parent);
}

bool TemplateLoader::loadFromFile(Template *t, const QString &fileName) const
{
  int i = 0;
  QFile file;

  while(!file.exists())
  {
    if ( i >= m_templateDirs.size() )
      break;

    file.setFileName(m_templateDirs.at(i) + "/" + m_themeName + "/" + fileName);
    ++i;
  }

  if ( !file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
      return false;
  }

  QTextStream in(&file);
  QString content;
  while (!in.atEnd()) {
      content += in.readLine();
  }

  t->setContent(content);
  return true;
}

void TemplateLoader::injectTemplate(const QString &name, const QString &content)
{
  m_namedTemplates.insert(name, content);
}

bool TemplateLoader::loadByName(Template *t, const QString &name) const
{
  if (m_namedTemplates.contains(name))
  {
    t->setContent(m_namedTemplates.value(name));
    return true;
  }
  return loadFromFile(t, name);
}


