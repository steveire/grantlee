

#ifndef TEMPLATE_LOADER_H
#define TEMPLATE_LOADER_H

#include "template.h"

namespace Grantlee
{

class AbstractTemplateResource : public QObject
{
  Q_OBJECT
public:
  AbstractTemplateResource(QObject* parent = 0);
  virtual ~AbstractTemplateResource();

  virtual Template* loadByName(const QString &name) const = 0;

};

class FileSystemTemplateResource : public AbstractTemplateResource
{
  Q_OBJECT
public:
  FileSystemTemplateResource(QObject* parent = 0);
  Template* loadByName(const QString &name) const;

  void setTheme(const QString &themeName);
  void setTemplateDirs(const QStringList &dirs);

private:
  QString m_themeName;
  QStringList m_templateDirs;
};

class InMemoryTemplateResource : public AbstractTemplateResource
{
  Q_OBJECT
public:
  InMemoryTemplateResource(QObject* parent = 0);
  Template* loadByName(const QString &name) const;

  void setTemplate(const QString &name, const QString &content);

private:
  QHash<QString, QString> m_namedTemplates;
};

class GRANTLEE_EXPORT TemplateLoader
{
public:
  static TemplateLoader* instance();

  QList<AbstractTemplateResource*> templateResources();

  void addTemplateResource(AbstractTemplateResource *resource);

  void setPluginDirs(const QStringList &dirs);

  QStringList pluginDirs();

  Template* loadByName(const QString &name, QObject *parent ) const;

  QStringList defaultLibraries() const;
  void setDefaultLibraries(const QStringList &list);
  void addDefaultLibrary(const QString &libName);
  void removeDefaultLibrary(const QString &libName);

private:
  TemplateLoader();

  QList<AbstractTemplateResource*> m_resources;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;
  static TemplateLoader* m_instance;
};

}

#endif
