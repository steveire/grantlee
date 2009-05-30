

#ifndef TEMPLATE_LOADER_H
#define TEMPLATE_LOADER_H

#include "template.h"

namespace Grantlee
{

class GRANTLEE_EXPORT TemplateLoader
{
public:
  static TemplateLoader* instance();

  void setTemplateDirs(const QStringList &dirs);
  void setPluginDirs(const QStringList &dirs);

  void setTheme(const QString &themeName);

  void injectTemplate(const QString &name, const QString &content);

  Template* getTemplate(QObject *parent = 0) const;

  bool loadByName(Template *t, const QString &name ) const;

  QStringList defaultLibraries() const;
  void setDefaultLibraries(const QStringList &list);
  void addDefaultLibrary(const QString &libName);
  void removeDefaultLibrary(const QString &libName);


private:
  TemplateLoader();
  bool loadFromFile(Template* t, const QString &fileName) const;

  QString m_themeName;
  QStringList m_templateDirs;
  QStringList m_pluginDirs;
  QStringList m_defaultLibraries;
  static TemplateLoader* m_instance;
  QHash<QString, QString> m_namedTemplates;
};



}

#endif
