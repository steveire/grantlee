/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "context.h"

#include <QtCore/QStringList>

#include "util.h"

using namespace Grantlee;

namespace Grantlee
{
class ContextPrivate
{
  ContextPrivate( Context *context, const QVariantHash &variantHash )
      : q_ptr( context ), m_autoescape( true ), m_mutating( false ) {
    m_variantHashStack.append( variantHash );
  }

  Q_DECLARE_PUBLIC( Context )
  Context *q_ptr;

  QList<QVariantHash> m_variantHashStack;
  bool m_autoescape;
  bool m_mutating;
  QStringList m_externalMedia;
};

}

Context::Context()
    : d_ptr( new ContextPrivate( this, QVariantHash() ) )
{
}

Context::Context( const QVariantHash &variantHash )
    : d_ptr( new ContextPrivate( this, variantHash ) )
{
}

Context::Context( const Context &other )
    : d_ptr( new ContextPrivate( this, QVariantHash() ) )
{
  *this = other;
}


Context& Context::operator=( const Context &other )
{
  if ( &other == this )
    return *this;
  d_ptr->m_autoescape = other.d_ptr->m_autoescape;
  d_ptr->m_externalMedia = other.d_ptr->m_externalMedia;
  d_ptr->m_mutating = other.d_ptr->m_mutating;
  d_ptr->m_variantHashStack = other.d_ptr->m_variantHashStack;
  return *this;
}

Context::~Context()
{
  delete d_ptr;
}

bool Context::autoEscape() const
{
  Q_D( const Context );
  return d->m_autoescape;
}

void Context::setAutoEscape( bool autoescape )
{
  Q_D( Context );
  d->m_autoescape = autoescape;
}

QVariant Context::lookup( const QString &str ) const
{
  Q_D( const Context );

  // return a variant from the stack.
  QListIterator<QVariantHash> i( d->m_variantHashStack );
  while ( i.hasNext() ) {
    QVariantHash h = i.next();
    if ( h.contains( str ) ) {
      QVariant var = h.value( str );
      // If the user passed a string into the context, turn it into a Grantlee::SafeString.
      if ( var.type() == QVariant::String ) {
        var = QVariant::fromValue<Grantlee::SafeString>( getSafeString( var.toString() ) );
      }
      return var;
    }
  }

  return QVariant();
}

void Context::push()
{
  Q_D( Context );

  QHash<QString, QVariant> hash;
  d->m_variantHashStack.prepend( hash );
}

void Context::pop()
{
  Q_D( Context );

  d->m_variantHashStack.removeFirst();
}

void Context::insert( const QString &name, const QVariant &variant )
{
  Q_D( Context );

  d->m_variantHashStack[0].insert( name, variant );
}

QHash<QString, QVariant> Context::stackHash( int depth ) const
{
  Q_D( const Context );

  return d->m_variantHashStack.value( depth );
}

bool Context::isMutating() const
{
  Q_D( const Context );
  return d->m_mutating;
}

void Context::setMutating( bool mutating )
{
  Q_D( Context );
  d->m_mutating = mutating;
}

void Context::addExternalMedia( const QString& uri )
{
  Q_D( Context );
  d->m_externalMedia.append( uri );
}

QStringList Context::externalMedia() const
{
  Q_D( const Context );
  return d->m_externalMedia;
}

void Context::clearExternalMedia()
{
  Q_D( Context );
  d->m_externalMedia.clear();
}


