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

#include "variable.h"

#include "typeaccessors_p.h"

#include <QStringList>
#include <QMetaEnum>
#include <QDebug>

#include "context.h"
#include "util_p.h"

using namespace Grantlee;

namespace Grantlee
{

class VariablePrivate
{
public:
  VariablePrivate( Variable *variable )
      : q_ptr( variable ) {

  }

  QVariant resolvePart( const QVariant &variant, const QString &s ) const;

  QString m_varString;
  QVariant m_literal;
  QStringList m_lookups;
  bool m_translate;

  Q_DECLARE_PUBLIC( Variable )
  Variable *q_ptr;
};

}

Variable::Variable( const Variable &other )
    : d_ptr( new VariablePrivate( this ) )
{
  d_ptr->m_varString = other.d_ptr->m_varString;
  d_ptr->m_literal = other.d_ptr->m_literal;
  d_ptr->m_lookups = other.d_ptr->m_lookups;
  d_ptr->m_translate = other.d_ptr->m_translate;
}

Variable::Variable()
    : d_ptr( new VariablePrivate( this ) )
{
}

Variable::~Variable()
{
  delete d_ptr;
}

Variable &Variable::operator=( const Variable & other )
{
  d_ptr->m_varString = other.d_ptr->m_varString;
  d_ptr->m_literal = other.d_ptr->m_literal;
  d_ptr->m_lookups = other.d_ptr->m_lookups;
  d_ptr->m_translate = other.d_ptr->m_translate;
  return *this;
}

Variable::Variable( const QString &var )
    : d_ptr( new VariablePrivate( this ) )
{
  Q_D( Variable );
  d->m_varString = var;

  QVariant v( var );
  if ( v.convert( QVariant::Double ) ) {
    d->m_literal = v;
    if ( !var.contains( '.' ) && !var.contains( 'e' ) ) {
      if ( var.endsWith( '.' ) ) {
//         throw Grantlee::Exception( VariableSyntaxError, QString( "Variable may not end with a dot: %1" ).arg( v.toString() ) );
      }

      d->m_literal = v.toInt();
    }
  } else {
    QString localVar = var;
    if ( var.startsWith( QLatin1String( "_(" ) ) && var.endsWith( QLatin1Char( ')' ) ) ) {
      d->m_translate = true;
      localVar = var.mid( 2, var.size() - 3 );
    }
    if (( localVar.startsWith( '"' ) && localVar.endsWith( '"' ) )
        || ( localVar.startsWith( '\'' ) && localVar.endsWith( '\'' ) ) ) {
      QString unesc = Util::unescapeStringLiteral( localVar );
      Grantlee::SafeString ss = Util::markSafe( unesc );
      d->m_literal = QVariant::fromValue<Grantlee::SafeString>( ss );
    } else {
      d->m_lookups = localVar.split( '.' );
    }
  }
}

bool Variable::isValid() const
{
  Q_D( const Variable );
  return !d->m_varString.isEmpty();
}

bool Variable::isConstant() const
{
  Q_D( const Variable );
  return !d->m_literal.isNull();
}

QString Variable::toString() const
{
  Q_D( const Variable );
  return d->m_varString;
}

bool Variable::isTrue( Context *c ) const
{
  return Util::variantIsTrue( resolve( c ) );
}

QVariant Variable::resolve( Context *c ) const
{
  Q_D( const Variable );
  QVariant var;
  if ( !d->m_lookups.isEmpty() ) {
    int i = 0;
    var = c->lookup( d->m_lookups.at( i++ ) );
    while ( i < d->m_lookups.size() ) {
      var = d->resolvePart( var, d->m_lookups.at( i++ ) );
      if ( !var.isValid() )
        return var;
    }
  } else {
    if ( Util::isSafeString( var ) )
      var = QVariant::fromValue( Util::getSafeString( d->m_literal ) );
    else
      var = d->m_literal;
  }

  if ( d->m_translate ) {
//     return gettext(var.toString());
  }


  if ( Util::supportedOutputType( var ) ) {
    return var;
  }
  if ( var.canConvert( QVariant::String ) ) {
    if ( var.convert( QVariant::String ) )
      return QVariant::fromValue<Grantlee::SafeString>( var.toString() );
    return QVariant();
  }
  // Could be a list or a hash.
  return var;
}

QVariant VariablePrivate::resolvePart( const QVariant &var, const QString &nextPart ) const
{
  QVariant returnVar;

//   Should be
// * QVariantHash key lookup
// * Property? (member in django)
// * list index
  if ( QVariant::Hash == var.type() ) {
    QVariantHash hash = var.toHash();
    if ( hash.contains( nextPart ) )
      return hash.value( nextPart );
    return TypeAccessor<QVariantHash>::lookUp( hash, nextPart );
  } else if ( qMetaTypeId< Grantlee::SafeString >() == var.userType() ) {
    return TypeAccessor<SafeString>::lookUp( Util::getSafeString( var ), nextPart );
  } else if ( QMetaType::QObjectStar == var.userType() ) {
    // Can't be const because of invokeMethod.
    const QObject *obj = var.value<QObject *>();
    const QMetaObject *metaObj = obj->metaObject();

    QMetaProperty mp;
    for ( int i = 0; i < metaObj->propertyCount(); ++i ) {
      // TODO only read-only properties should be allowed here.
      // This might also handle the variant messing I hit before.
      mp = metaObj->property( i );

      if ( QString( mp.name() ) != nextPart )
        continue;

      return mp.read( obj );

    }
    return QVariant();
  } else {
    // List index test

    bool ok = false;
    int listIndex = nextPart.toInt( &ok );
    if ( !ok )
      return QVariant();
    QVariantList varList = Util::variantToList( var );

    if ( listIndex >= varList.size() )
      return QVariant();
    return varList.at( listIndex );
  }

  return QVariant();
}

