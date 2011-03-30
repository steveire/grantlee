/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010,2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_CAT_H
#define GRANTLEE_CAT_H

#include <QtCore/QIODevice>

#include "grantlee_tubes_export.h"

namespace Grantlee
{
class CatPrivate;

/// @headerfile cat.h grantlee/cat.h

/**
  @brief Cat reads from multiple sources and writes to a single target

  Cat reads from list of QIODevice targets in sequence and writes that resulting data to
  a target QIODevice.

  Cat reads from a source device until it closes, at which point it starts reading from the
  next source device in the list. This means that it is the responsibility of the
  caller to close each source device at the appropriate time to cause Cat to advance through
  its sources.

  @author Stephen Kelly <steveire@gmail.com>
 */
class GRANTLEE_TUBES_EXPORT Cat : public QObject
{
  Q_OBJECT
public:
  Cat( QObject *parent = 0 );

  /**
    Appends @p sourceDevice to the list of devices to read from.
  */
  void appendSource( QIODevice *sourceDevice );

  /**
    Returns the list of devices to read from.
  */
  QList<QIODevice*> sources() const;

  /**
    Sets the list of devices to read from to @p sources
  */
  void setSources( const QList<QIODevice*> &sources );

  /**
    Sets the device to write to to @p targetDevice
  */
  void setTarget( QIODevice *targetDevice );

  /**
    Returns the device being written to.
  */
  QIODevice* target() const;

private:
  Q_DECLARE_PRIVATE( Cat )
  CatPrivate * const d_ptr;
  Q_PRIVATE_SLOT( d_func(), void sourceReadFinished() )
};

}

#endif
