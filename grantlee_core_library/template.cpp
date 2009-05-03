/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "template.h"

#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QFile>

#include "lexer.h"
#include "node.h"
#include "parser.h"

#include <QDebug>

#include <QListIterator>

using namespace Grantlee;

typedef QListIterator<Node*> NodeListIterator;

NodeList Template::compileString(const QString &str)
{
  Lexer l(str);
  Parser p( l.tokenize(), m_pluginDirs );
  return p.parse();
}

Template::Template(const QString &str, QStringList pluginDirs )// : QObject(parent)
{
  m_pluginDirs = pluginDirs;
  m_nodelist = compileString( str );
}

QString Template::render(Context *c)
{
  return m_nodelist.render(c);
}

NodeList Template::nodeList()
{
  return m_nodelist;
}

void Template::setNodeList(const NodeList &list)
{
  m_nodelist = list;
}

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
}

void TemplateLoader::setTemplateDirs(const QStringList &dirs)
{
  m_templateDirs = dirs;
}

void TemplateLoader::setPluginDirs(const QStringList &dirs)
{
  m_pluginDirs = dirs;
}

void TemplateLoader::setTheme(const QString &themeName)
{
  m_themeName = themeName;
}

Template TemplateLoader::loadFromFile(const QString &fileName)
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
      return Template(QString(), QStringList());

  QTextStream in(&file);
  QString content;
  while (!in.atEnd()) {
      content += in.readLine();
  }

  return loadFromString(content);
}

Template TemplateLoader::loadFromString(const QString &content)
{
  return Template(content, m_pluginDirs);
}

void TemplateLoader::injectTemplate(const QString &name, const QString &content)
{
  m_namedTemplates.insert(name, content);
}

Template TemplateLoader::loadByName(const QString &name)
{
  if (m_namedTemplates.contains(name))
    return loadFromString(m_namedTemplates.value(name));
  return loadFromFile(name);
}

