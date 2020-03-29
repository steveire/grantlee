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

#include "cachingloaderdecorator.h"

namespace Grantlee
{

class CachingLoaderDecoratorPrivate
{
public:
  CachingLoaderDecoratorPrivate(QSharedPointer<AbstractTemplateLoader> loader,
                                CachingLoaderDecorator *qq)
      : q_ptr(qq), m_wrappedLoader(loader)
  {
  }

  Q_DECLARE_PUBLIC(CachingLoaderDecorator)
  CachingLoaderDecorator *const q_ptr;

  const QSharedPointer<AbstractTemplateLoader> m_wrappedLoader;

  mutable QHash<QString, Template> m_cache;
};
}

using namespace Grantlee;

CachingLoaderDecorator::CachingLoaderDecorator(
    QSharedPointer<AbstractTemplateLoader> loader)
    : d_ptr(new CachingLoaderDecoratorPrivate(loader, this))
{
}

CachingLoaderDecorator::~CachingLoaderDecorator() { delete d_ptr; }

bool CachingLoaderDecorator::canLoadTemplate(const QString &name) const
{
  Q_D(const CachingLoaderDecorator);
  return d->m_wrappedLoader->canLoadTemplate(name);
}

void CachingLoaderDecorator::clear()
{
  Q_D(CachingLoaderDecorator);
  return d->m_cache.clear();
}

int CachingLoaderDecorator::size() const
{
  Q_D(const CachingLoaderDecorator);
  return d->m_cache.size();
}

bool CachingLoaderDecorator::isEmpty() const
{
  Q_D(const CachingLoaderDecorator);
  return d->m_cache.isEmpty();
}

QPair<QString, QString>
CachingLoaderDecorator::getMediaUri(const QString &fileName) const
{
  Q_D(const CachingLoaderDecorator);
  return d->m_wrappedLoader->getMediaUri(fileName);
}

Template
CachingLoaderDecorator::loadByName(const QString &name,
                                   const Grantlee::Engine *engine) const
{
  Q_D(const CachingLoaderDecorator);
  const auto it = d->m_cache.constFind(name);
  if (it != d->m_cache.constEnd()) {
    return it.value();
  }

  const auto t = d->m_wrappedLoader->loadByName(name, engine);

  d->m_cache.insert(name, t);

  return t;
}
