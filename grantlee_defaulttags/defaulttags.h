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

#ifndef DEFAULTTAGS_H
#define DEFAULTTAGS_H

#include "node.h"
#include "taglibraryinterface.h"

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
// #include "ssi.h"
#include "templatetag.h"
#include "widthratio.h"
#include "with.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class DefaultTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  DefaultTagLibrary( QObject *parent = 0 )
      : QObject( parent ) {
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );

    QHash<QString, AbstractNodeFactory*> nodeFactories;

    nodeFactories.insert( "autoescape", new AutoescapeNodeFactory() );
    nodeFactories.insert( "comment", new CommentNodeFactory() );
    nodeFactories.insert( "cycle", new CycleNodeFactory() );
    nodeFactories.insert( "debug", new DebugNodeFactory() );
    nodeFactories.insert( "filter", new FilterNodeFactory() );
    nodeFactories.insert( "firstof", new FirstOfNodeFactory() );
    nodeFactories.insert( "for", new ForNodeFactory() );
    nodeFactories.insert( "if", new IfNodeFactory() );
    nodeFactories.insert( "ifchanged", new IfChangedNodeFactory() );
    nodeFactories.insert( "ifequal", new IfEqualNodeFactory() );
    nodeFactories.insert( "ifnotequal", new IfNotEqualNodeFactory() );
    nodeFactories.insert( "load", new LoadNodeFactory() );
    nodeFactories.insert( "media_finder", new MediaFinderNodeFactory() );
    nodeFactories.insert( "now", new NowNodeFactory() );
    nodeFactories.insert( "range", new RangeNodeFactory() );
    nodeFactories.insert( "regroup", new RegroupNodeFactory() );
    nodeFactories.insert( "spaceless", new SpacelessNodeFactory() );
    // Disabled for security reasons.
//     nodeFactories.insert( "ssi", new SsiNodeFactory() );
    nodeFactories.insert( "templatetag", new TemplateTagNodeFactory() );
    nodeFactories.insert( "widthratio", new WidthRatioNodeFactory() );
    nodeFactories.insert( "with", new WithNodeFactory() );

    return nodeFactories;
  }
};

#endif

