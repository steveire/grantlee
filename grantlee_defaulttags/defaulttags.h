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
#include "interfaces/taglibraryinterface.h"

#include "autoescape.h"
#include "regroup.h"
#include "if.h"
#include "for.h"
#include "with.h"
#include "comment.h"
#include "ifequal.h"
#include "firstof.h"
#include "cycle.h"
#include "templatetag.h"
#include "widthratio.h"
#include "filtertag.h"
#include "load.h"
#include "now.h"
#include "ssi.h"
#include "ifchanged.h"
#include "spaceless.h"
#include "debug.h"

#include "grantlee_export.h"

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
    m_nodeFactories.insert( "autoescape", new AutoescapeNodeFactory() );
    m_nodeFactories.insert( "regroup", new RegroupNodeFactory() );
    m_nodeFactories.insert( "if", new IfNodeFactory() );
    m_nodeFactories.insert( "for", new ForNodeFactory() );
    m_nodeFactories.insert( "with", new WithNodeFactory() );
    m_nodeFactories.insert( "comment", new CommentNodeFactory() );
    m_nodeFactories.insert( "ifequal", new IfEqualNodeFactory() );
    m_nodeFactories.insert( "ifnotequal", new IfNotEqualNodeFactory() );
    m_nodeFactories.insert( "firstof", new FirstOfNodeFactory() );
    m_nodeFactories.insert( "cycle", new CycleNodeFactory() );
    m_nodeFactories.insert( "templatetag", new TemplateTagNodeFactory() );
    m_nodeFactories.insert( "widthratio", new WidthRatioNodeFactory() );
    m_nodeFactories.insert( "filter", new FilterNodeFactory() );
    m_nodeFactories.insert( "load", new LoadNodeFactory() );
    m_nodeFactories.insert( "now", new NowNodeFactory() );
    m_nodeFactories.insert( "ssi", new SsiNodeFactory() );
    m_nodeFactories.insert( "ifchanged", new IfChangedNodeFactory() );
    m_nodeFactories.insert( "spaceless", new SpacelessNodeFactory() );
    m_nodeFactories.insert( "debug", new DebugNodeFactory() );
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );
    return m_nodeFactories;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;

};

#endif

