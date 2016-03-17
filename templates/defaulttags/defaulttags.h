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

#ifndef DEFAULTTAGS_H
#define DEFAULTTAGS_H

#include "autoescape.h"
#include "comment.h"
#include "cycle.h"
#include "debug.h"
#include "filtertag.h"
#include "firstof.h"
#include "for.h"
#include "if.h"
#include "ifchanged.h"
#include "ifequal.h"
#include "load.h"
#include "mediafinder.h"
#include "now.h"
#include "range.h"
#include "regroup.h"
#include "spaceless.h"
#include "templatetag.h"
#include "widthratio.h"
#include "with.h"

#include "node.h"
#include "taglibraryinterface.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class DefaultTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES(Grantlee::TagLibraryInterface)
  Q_PLUGIN_METADATA(IID "org.grantlee.TagLibraryInterface")
public:
  DefaultTagLibrary(QObject *parent = 0) : QObject(parent) {}

  QHash<QString, AbstractNodeFactory *> nodeFactories(const QString &name
                                                      = QString()) override
  {
    Q_UNUSED(name);

    QHash<QString, AbstractNodeFactory *> nodeFactories;

    nodeFactories.insert(QStringLiteral("autoescape"),
                         new AutoescapeNodeFactory());
    nodeFactories.insert(QStringLiteral("comment"), new CommentNodeFactory());
    nodeFactories.insert(QStringLiteral("cycle"), new CycleNodeFactory());
    nodeFactories.insert(QStringLiteral("debug"), new DebugNodeFactory());
    nodeFactories.insert(QStringLiteral("filter"), new FilterNodeFactory());
    nodeFactories.insert(QStringLiteral("firstof"), new FirstOfNodeFactory());
    nodeFactories.insert(QStringLiteral("for"), new ForNodeFactory());
    nodeFactories.insert(QStringLiteral("if"), new IfNodeFactory());
    nodeFactories.insert(QStringLiteral("ifchanged"),
                         new IfChangedNodeFactory());
    nodeFactories.insert(QStringLiteral("ifequal"), new IfEqualNodeFactory());
    nodeFactories.insert(QStringLiteral("ifnotequal"),
                         new IfNotEqualNodeFactory());
    nodeFactories.insert(QStringLiteral("load"), new LoadNodeFactory());
    nodeFactories.insert(QStringLiteral("media_finder"),
                         new MediaFinderNodeFactory());
    nodeFactories.insert(QStringLiteral("now"), new NowNodeFactory());
    nodeFactories.insert(QStringLiteral("range"), new RangeNodeFactory());
    nodeFactories.insert(QStringLiteral("regroup"), new RegroupNodeFactory());
    nodeFactories.insert(QStringLiteral("spaceless"),
                         new SpacelessNodeFactory());
    nodeFactories.insert(QStringLiteral("templatetag"),
                         new TemplateTagNodeFactory());
    nodeFactories.insert(QStringLiteral("widthratio"),
                         new WidthRatioNodeFactory());
    nodeFactories.insert(QStringLiteral("with"), new WithNodeFactory());

    return nodeFactories;
  }
};

#endif
