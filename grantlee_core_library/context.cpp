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

#include "context.h"

#include "util_p.h"

using namespace Grantlee;

namespace Grantlee
{
class ContextPrivate
{
  ContextPrivate( Context *context, const QVariantHash &variantHash )
      : q_ptr( context ), m_autoescape( true ), m_mutating( false ) {
    m_variantHashStack.append( variantHash );
  }

  QList<QVariantHash> m_variantHashStack;
  bool m_autoescape;
  bool m_mutating;

  Q_DECLARE_PUBLIC( Context )
  Context *q_ptr;

};

}

Context::Context( const QVariantHash &variantHash )
    : d_ptr( new ContextPrivate( this, variantHash ) )
{
}

Context::~Context()
{
  delete d_ptr;
}

bool Context::autoescape() const
{
  Q_D( const Context );
  return d->m_autoescape;
}

void Context::setAutoescape( bool autoescape )
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
        var = QVariant::fromValue<Grantlee::SafeString>( Util::getSafeString( var.toString() ) );
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
  Q_D(const Context);
  return d->m_mutating;
}

void Context::setMutating(bool mutating)
{
  Q_D(Context);
  d->m_mutating = mutating;
}

