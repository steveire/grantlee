/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H

#include <QSharedPointer>

#include "template.h"
#include "mutabletemplate.h"


namespace Grantlee
{

class AbstractTemplateLoader
{
public:
  typedef QSharedPointer<AbstractTemplateLoader> Ptr;

  virtual ~AbstractTemplateLoader();

  virtual MutableTemplate loadMutableByName( const QString &name ) const = 0;
  virtual Template loadByName( const QString &name ) const = 0;
  virtual QString getMediaUri( const QString &fileName ) const = 0;
  virtual bool canLoadTemplate( const QString &name ) const = 0;

};

class FileSystemTemplateLoader : public AbstractTemplateLoader
{
public:
  typedef QSharedPointer<FileSystemTemplateLoader> Ptr;

  FileSystemTemplateLoader();
  virtual ~FileSystemTemplateLoader();
  MutableTemplate loadMutableByName( const QString &name ) const;
  Template loadByName( const QString &name ) const;

  virtual bool canLoadTemplate( const QString &name ) const;

  virtual QString getMediaUri( const QString& fileName ) const;

  void setTheme( const QString &themeName );
  QString themeName() const;
  void setTemplateDirs( const QStringList &dirs );

private:
  QString m_themeName;
  QStringList m_templateDirs;
};

class InMemoryTemplateLoader : public AbstractTemplateLoader
{
public:
  typedef QSharedPointer<InMemoryTemplateLoader> Ptr;

  InMemoryTemplateLoader();
  virtual ~InMemoryTemplateLoader();
  MutableTemplate loadMutableByName( const QString &name ) const;
  Template loadByName( const QString &name ) const;

  virtual bool canLoadTemplate( const QString &name ) const;

  virtual QString getMediaUri( const QString& fileName ) const;

  void setTemplate( const QString &name, const QString &content );

private:
  QHash<QString, QString> m_namedTemplates;
};

}

#endif
