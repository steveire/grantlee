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

#ifndef TAGLIBRARYINTERFACE_H
#define TAGLIBRARYINTERFACE_H

#include "outputstream.h"

#include <QtCore/QHash>

namespace Grantlee
{
class AbstractNodeFactory;
class Engine;
class Filter;

/// @headerfile taglibraryinterface.h grantlee/taglibraryinterface.h

/**
  @brief The TagLibraryInterface returns available tags and filters from libraries.

  This interface must be implemented in tag and filter libraries.

  The implementation will usually be very simple.

  @code
    class MyTagLibrary : public QObject, public TagLibraryInterface
    {
      Q_OBJECT
      Q_INTERFACES( Grantlee::TagLibraryInterface )
    public:
      MyTagLibrary( QObject *parent = 0 )
          : QObject( parent ) {
      }

      QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
        Q_UNUSED( name );
        QHash<QString, AbstractNodeFactory*> nodeFactories;
        nodeFactories.insert( "mytag1", new MyTag1() );
        nodeFactories.insert( "mytag2", new MyTag2() );
        return nodeFactories;
      }

      QHash<QString, Filter*> filters( const QString &name = QString() ) {
        Q_UNUSED( name );

        QHash<QString, Filter*> filters;

        filters.insert( "myfilter1", new MyFilter1() );
        filters.insert( "myfilter2", new MyFilter2() );

        return filters;
      }
    };
  @endcode

  @author Stephen Kelly <steveire@gmail.com>
*/
class TagLibraryInterface
{
public:
  virtual ~TagLibraryInterface() {}

  /**
    Returns the AbstractNodeFactory implementations available in this library.
  */
  virtual QHash<QString, AbstractNodeFactory*> nodeFactories( const QString &name = QString() ) {
    Q_UNUSED( name );
    static const QHash<QString, AbstractNodeFactory*> h;
    return h;
  };

  /**
    Returns the Filter implementations available in this library.
  */
  virtual QHash<QString, Filter*> filters( const QString &name = QString() ) {
    Q_UNUSED( name );
    static const QHash<QString, Filter*> h;
    return h;
  };
};

}

Q_DECLARE_INTERFACE( Grantlee::TagLibraryInterface, "org.kde.grantlee.TagLibraryInterface/1.0" )

#endif
