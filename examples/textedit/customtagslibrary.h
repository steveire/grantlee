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


#ifndef CUSTOM_TAGS_H
#define CUSTOM_TAGS_H

#include <grantlee/taglibraryinterface.h>

#include "filters.h"

using namespace Grantlee;

class CustomTagsLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  CustomTagsLibrary( QObject *parent = 0 )
      : QObject( parent ) {
  }

  QHash<QString, Filter*> filters( const QString &name = QString() ) {
    Q_UNUSED( name );
    QHash<QString, Filter*> _filters;
    _filters.insert( "to_clean_html", new CleanHtmlFilter() );
    _filters.insert( "to_plain_text", new PlainTextFilter() );
    return _filters;
  }
};

#endif
