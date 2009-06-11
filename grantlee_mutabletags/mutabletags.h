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

#ifndef MUTABLE_TAGS_H
#define MUTABLE_TAGS_H

#include "interfaces/taglibraryinterface.h"

#include "grantlee_export.h"

#include "raw.h"
#include "repeater.h"

using namespace Grantlee;

class GRANTLEE_EXPORT MutableTagsLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  MutableTagsLibrary( QObject *parent = 0 )
      : QObject( parent ) {
    m_nodeFactories.insert( "raw", new RawNodeFactory() );
    m_nodeFactories.insert( "repeater", new RepeaterNodeFactory() );
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );
    return m_nodeFactories;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;
};

#endif

