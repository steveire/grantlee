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

#include "variable.h"

#include "abstractlocalizer.h"
#include "context.h"
#include "exception.h"
#include "metaenumvariable_p.h"
#include "metatype.h"
#include "util.h"

#include <QtCore/QMetaEnum>
#include <QtCore/QStringList>

using namespace Grantlee;

namespace Grantlee
{

class VariablePrivate
{
public:
  VariablePrivate( Variable *variable )
      : q_ptr( variable ),
        m_localize( false )
  {

  }

  Q_DECLARE_PUBLIC( Variable )
  Variable * const q_ptr;

  QString m_varString;
  QVariant m_literal;
  QStringList m_lookups;
  bool m_localize;
};

}

Variable::Variable( const Variable &other )
    : d_ptr( new VariablePrivate( this ) )
{
  *this = other;
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
  if (&other == this)
    return *this;
  d_ptr->m_varString = other.d_ptr->m_varString;
  d_ptr->m_literal = other.d_ptr->m_literal;
  d_ptr->m_lookups = other.d_ptr->m_lookups;
  d_ptr->m_localize = other.d_ptr->m_localize;
  return *this;
}

Variable::Variable( const QString &var )
    : d_ptr( new VariablePrivate( this ) )
{
  Q_D( Variable );
  d->m_varString = var;

  QString localVar = var;
  if ( var.startsWith( QLatin1String( "_(" ) ) ) {
    // The FilterExpression parser ensures this:
    Q_ASSERT( var.endsWith( QLatin1Char( ')' ) ) );
    d->m_localize = true;
    localVar = var.mid( 2, var.size() - 3 );
  }
  if ( localVar.endsWith( QLatin1Char( '.' ) ) ) {
    throw Grantlee::Exception( TagSyntaxError, QString::fromLatin1( "Variable may not end with a dot: %1" ).arg( localVar ) );
  }

  bool processedNumber = false;
  {
    const int intResult = QLocale::c().toInt(localVar, &processedNumber);
    if (processedNumber) {
      d->m_literal = intResult;
    } else {
      const double doubleResult = QLocale::c().toDouble(localVar, &processedNumber);
      if (processedNumber) {
        d->m_literal = doubleResult;
      }
    }
  }
  if (!processedNumber) {
    if ( localVar.startsWith( QLatin1Char( '"' ) ) || localVar.startsWith( QLatin1Char( '\'' ) ) ) {
      // The FilterExpression parser ensures this:
      Q_ASSERT(localVar.endsWith( QLatin1Char( '\'' ) ) || localVar.endsWith( QLatin1Char( '"' ) ) );
      const QString unesc = unescapeStringLiteral( localVar );
      const Grantlee::SafeString ss = markSafe( unesc );
      d->m_literal = QVariant::fromValue<Grantlee::SafeString>( ss );
    } else {
      if ( localVar.contains( QLatin1String( "._" ) ) || ( localVar.startsWith( QLatin1Char( '_' ) ) ) ) {
        throw Grantlee::Exception( TagSyntaxError,
            QString::fromLatin1( "Variables and attributes may not begin with underscores: %1" ).arg( localVar ) );
      }
      d->m_lookups = localVar.split( QLatin1Char( '.' ) );
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

bool Variable::isTrue( Context *c ) const
{
  return variantIsTrue( resolve( c ) );
}

bool Variable::isLocalized() const
{
  Q_D( const Variable );
  return d->m_localize;
}

QVariant Variable::literal() const
{
  Q_D( const Variable );
  return d->m_literal;
}

QStringList Variable::lookups() const
{
  Q_D( const Variable );
  return d->m_lookups;
}

class StaticQtMetaObject : public QObject
{
public:
  static const QMetaObject* _smo() { return &QObject::staticQtMetaObject; }
};

QVariant Variable::resolve( Context *c ) const
{
  Q_D( const Variable );
  QVariant var;
  if ( !d->m_lookups.isEmpty() ) {
    int i = 0;
    if ( d->m_lookups.at( i ) == QLatin1String( "Qt" ) ) {
      ++i;
      const QString nextPart = d->m_lookups.at( i );
      ++i;

      static const QMetaObject *globalMetaObject = StaticQtMetaObject::_smo();

      bool breakout = false;
      for ( int j = 0; j < globalMetaObject->enumeratorCount(); ++j ) {
        const QMetaEnum me = globalMetaObject->enumerator( j );

        if ( QLatin1String( me.name() ) == nextPart ) {
          const MetaEnumVariable mev( me );
          var = QVariant::fromValue( mev );
          break;
        }

        for ( int k = 0; k < me.keyCount(); ++k ) {
          if ( QLatin1String( me.key( k ) ) == nextPart ) {
            const MetaEnumVariable mev( me, k );
            var = QVariant::fromValue( mev );
            breakout = true;
            break;
          }
        }
        if ( breakout )
          break;
      }
      if ( !var.isValid() )
        return QVariant();

    } else {
      var = c->lookup( d->m_lookups.at( i++ ) );
    }
    while ( i < d->m_lookups.size() ) {
      var = MetaType::lookup( var, d->m_lookups.at( i++ ) );
      if ( !var.isValid() )
        return QVariant();
    }
  } else {
    if ( isSafeString( d->m_literal ) )
      var = QVariant::fromValue( getSafeString( d->m_literal ) );
    else
      var = d->m_literal;
  }

  if ( d->m_localize ) {
    return c->localizer()->localize( var );
  }
  return var;
}
