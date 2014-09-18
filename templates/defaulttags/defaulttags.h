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
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  DefaultTagLibrary( QObject *parent = 0 )
      : QObject( parent ) {
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );

    QHash<QString, AbstractNodeFactory*> nodeFactories;

    nodeFactories.insert( QLatin1String( "autoescape" ), new AutoescapeNodeFactory() );
    nodeFactories.insert( QLatin1String( "comment" ), new CommentNodeFactory() );
    nodeFactories.insert( QLatin1String( "cycle" ), new CycleNodeFactory() );
    nodeFactories.insert( QLatin1String( "debug" ), new DebugNodeFactory() );
    nodeFactories.insert( QLatin1String( "filter" ), new FilterNodeFactory() );
    nodeFactories.insert( QLatin1String( "firstof" ), new FirstOfNodeFactory() );
    nodeFactories.insert( QLatin1String( "for" ), new ForNodeFactory() );
    nodeFactories.insert( QLatin1String( "if" ), new IfNodeFactory() );
    nodeFactories.insert( QLatin1String( "ifchanged" ), new IfChangedNodeFactory() );
    nodeFactories.insert( QLatin1String( "ifequal" ), new IfEqualNodeFactory() );
    nodeFactories.insert( QLatin1String( "ifnotequal" ), new IfNotEqualNodeFactory() );
    nodeFactories.insert( QLatin1String( "load" ), new LoadNodeFactory() );
    nodeFactories.insert( QLatin1String( "media_finder" ), new MediaFinderNodeFactory() );
    nodeFactories.insert( QLatin1String( "now" ), new NowNodeFactory() );
    nodeFactories.insert( QLatin1String( "range" ), new RangeNodeFactory() );
    nodeFactories.insert( QLatin1String( "regroup" ), new RegroupNodeFactory() );
    nodeFactories.insert( QLatin1String( "spaceless" ), new SpacelessNodeFactory() );
    nodeFactories.insert( QLatin1String( "templatetag" ), new TemplateTagNodeFactory() );
    nodeFactories.insert( QLatin1String( "widthratio" ), new WidthRatioNodeFactory() );
    nodeFactories.insert( QLatin1String( "with" ), new WithNodeFactory() );

    return nodeFactories;
  }
};

#endif
