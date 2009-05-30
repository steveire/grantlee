/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QStringList>

#include "node.h"
#include "grantlee_export.h"

namespace Grantlee
{
class Context;
}

namespace Grantlee
{

class GRANTLEE_EXPORT Template : public QObject
{
  Q_OBJECT
public:
  Template(const QStringList &dirs, QObject *parent = 0  );

  void setContent(const QString &templateString);

  QString render( Context *c );

  NodeList getNodesByType(const char * className);

  NodeList nodeList() const;

  void setNodeList(const NodeList &list);
  
signals:
  void error(int type, const QString &message);

private:
  void parse();
  NodeList compileString(const QString &str);
  QStringList m_pluginDirs;
  NodeList m_nodeList;
};

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
