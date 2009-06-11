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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QVariantHash>

#include "grantlee_export.h"

namespace Grantlee
{

class ContextPrivate;

class GRANTLEE_EXPORT Context
{

public:

  /**
  Sets every key in the hash as a property name with the variant as the value.
  */
  Context( const QVariantHash &hash );

  bool autoescape() const;
  void setAutoescape( bool autoescape );

  ~Context();

  /**
  Returns a QVariant, even in the case of it containing a list which can be iterated over.
  Have to think about whether QMap should work too, or would its keys already be resolved.
  */
  QVariant lookup( const QString &str ) const;
  void insert( const QString &name, const QVariant &variant );
  void push();
  void pop();

  QVariantHash stackHash( int depth ) const;
  bool isMutating() const;
  void setMutating( bool mutating );

private:
  Q_DECLARE_PRIVATE( Context )
  ContextPrivate *d_ptr;
};

}

#endif

