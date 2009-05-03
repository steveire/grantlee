/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "context.h"

#include "node.h"

#include "grantlee_export.h"

class GRANTLEE_EXPORT Template //: public QObject
{
//   Q_OBJECT
public:
  Template(const QString &templateString, QStringList dirs );

  QString render( Context *c );

  NodeList nodeList();

  // TODO: Remove this.  ??
  void setNodeList(const NodeList &list);

private:
  void parse();
  NodeList compileString(const QString &str);
  QStringList m_pluginDirs;
  NodeList m_nodelist;
};

class GRANTLEE_EXPORT TemplateLoader
{
public:
  static TemplateLoader* instance();

  void setTemplateDirs(const QStringList &dirs);
  void setPluginDirs(const QStringList &dirs);

  void setTheme(const QString &themeName);

  void injectTemplate(const QString &name, const QString &content);

  Template loadFromString(const QString &content);

  Template loadByName(const QString &name);

  TemplateLoader();

private:
  Template loadFromFile(const QString &fileName);

  QString m_themeName;
  QStringList m_templateDirs;
  QStringList m_pluginDirs;
  static TemplateLoader* m_instance;
  QHash<QString, QString> m_namedTemplates;
};

#endif
