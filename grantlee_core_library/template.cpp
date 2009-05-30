/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "template.h"

#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QListIterator>

#include "lexer_p.h"
#include "parser.h"
#include "context.h"
#include "grantlee.h"

using namespace Grantlee;

typedef QListIterator<Node*> NodeListIterator;

NodeList Template::compileString(const QString &str)
{
  Lexer l(str);
  Parser p( l.tokenize(), m_pluginDirs, this );
  connect(&p, SIGNAL(error(int, QString)), SIGNAL(error(int, QString)));
  return p.parse(this);
}

Template::Template( const QStringList &pluginDirs, QObject *parent ) : QObject(parent)
{
  m_pluginDirs = pluginDirs;
}

void Template::setContent(const QString &templateString)
{
  if (!templateString.isEmpty())
    m_nodeList = compileString( templateString );
}

NodeList Template::getNodesByType(const char* className)
{
  return m_nodeList.getNodesByType(className);
}

QString Template::render(Context *c)
{
  QString ret = m_nodeList.render(c);
  if (ret.isNull())
  {
    error(TagSyntaxError, "someError");
    return QString();
  }
  return ret;
}

NodeList Template::nodeList() const
{
  return m_nodeList;
}

void Template::setNodeList(const NodeList &list)
{
  m_nodeList = list;
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

