/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef LOADERTAGS_H
#define LOADERTAGS_H

#include "node.h"
#include "taglibraryinterface.h"

#include "block.h"
#include "extends.h"
#include "include.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class LoaderTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES(Grantlee::TagLibraryInterface)
  Q_PLUGIN_METADATA(IID "org.grantlee.TagLibraryInterface")
public:
  LoaderTagLibrary() {}

  QHash<QString, AbstractNodeFactory *> nodeFactories(const QString &name
                                                      = QString()) override
  {
    Q_UNUSED(name);

    QHash<QString, AbstractNodeFactory *> nodeFactories;
    nodeFactories.insert(QStringLiteral("block"), new BlockNodeFactory());
    nodeFactories.insert(QStringLiteral("extends"), new ExtendsNodeFactory());
    nodeFactories.insert(QStringLiteral("include"), new IncludeNodeFactory());
    return nodeFactories;
  }
};

#endif
