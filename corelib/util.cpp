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

#include "util.h"
#include "metaenumvariable_p.h"

#include <QStringList>

QString Grantlee::unescapeStringLiteral( const QString &input )
{
  return input.mid( 1, input.size() - 2 )
         .replace( "\\\'", QChar( '\'' ) )
         .replace( "\\\"", QChar( '"' ) )
         .replace( "\\\\", QChar( '\\' ) );
}

bool Grantlee::variantIsTrue( const QVariant &variant )
{

  if ( !variant.isValid() )
    return false;
  switch ( variant.userType() ) {
  case QVariant::Bool: {
    return variant.toBool();
  }
  case QVariant::Int: {
    return ( variant.toInt() > 0 );
  }
  case QVariant::Double: {
    return ( variant.toDouble() > 0 );
  }
  case QMetaType::QObjectStar: {
    QObject *obj = variant.value<QObject *>();
    if ( !obj )
      return false;

    if ( obj->property( "__true__" ).isValid() ) {
      return obj->property( "__true__" ).toBool();
    }
    return true;
  }
  case QVariant::List: {
    return ( variant.toList().size() > 0 );
  }
  case QVariant::Hash: {
    return ( variant.toHash().size() > 0 );
  }
  }

  return !getSafeString( variant ).get().isEmpty();
}

QVariantList Grantlee::variantToList( const QVariant &var )
{

  if ( !var.isValid() )
    return QVariantList();

  if ( var.type() == QVariant::List ) {
    return var.toList();
  }

  if ( var.type() == QVariant::StringList ) {
    QVariantList list;
    foreach(const QString &_string, var.toStringList())
      list << _string;
    return list;
  }

  if ( var.type() == QVariant::Hash ) {
    const QVariantHash varHash = var.toHash();
    QVariantList list;

    QVariantHash::const_iterator it = varHash.constBegin();
    for ( ; it != varHash.constEnd(); ++it )
      list << it.key();

    return list;
  }

  if ( var.userType() == qMetaTypeId<MetaEnumVariable>() ) {
    const MetaEnumVariable mev = var.value<MetaEnumVariable>();
    if (mev.value != -1)
      return QVariantList();

    QVariantList list;
    for (int row = 0; row < mev.enumerator.keyCount(); ++row) {
      list << QVariant::fromValue( MetaEnumVariable( mev.enumerator, row ) );
    }
    return list;
  }

  if ( var.userType() == QMetaType::QObjectStar ) {
    QObject *obj = var.value<QObject*>();
    if ( obj->property( "__list__" ).isValid() ) {
      return obj->property( "__list__" ).toList();
    }
  } else {
    return QVariantList() << var;
  }
  return QVariantList();
}

Grantlee::SafeString Grantlee::markSafe( const Grantlee::SafeString &input )
{
  Grantlee::SafeString sret = input;
  sret.setSafety( Grantlee::SafeString::IsSafe );
  return sret;
}

Grantlee::SafeString Grantlee::markForEscaping( const Grantlee::SafeString &input )
{
  Grantlee::SafeString temp = input;
  if ( input.isSafe() || input.needsEscape() )
    return input;

  temp.setNeedsEscape( true );
  return temp;
}

Grantlee::SafeString Grantlee::getSafeString( const QVariant &input )
{
  if ( input.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
    return input.value<Grantlee::SafeString>();
  } else {
    return input.toString();
  }
}

bool Grantlee::isSafeString( const QVariant &input )
{
  const int type = input.userType();
  return (( type == qMetaTypeId<Grantlee::SafeString>() )
          || type == QVariant::String );
}

static QList<int> getPrimitives()
{
  QList<int> primitives;
  primitives << qMetaTypeId<Grantlee::SafeString>()
             << QVariant::Bool
             << QVariant::Int
             << QVariant::Double
             << QVariant::DateTime;
  return primitives;
}

bool Grantlee::supportedOutputType( const QVariant &input )
{
  static const QList<int> primitives = getPrimitives();
  return primitives.contains( input.userType() );
}


bool Grantlee::equals( const QVariant &lhs, const QVariant &rhs )
{

  // TODO: Redesign...

  // QVariant doesn't use operator== to compare its held data, so we do it manually instead for SafeString.
  bool equal = false;
  if ( lhs.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
    if ( rhs.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
      equal = ( lhs.value<Grantlee::SafeString>() == rhs.value<Grantlee::SafeString>() );
    } else if ( rhs.userType() == QVariant::String ) {
      equal = ( lhs.value<Grantlee::SafeString>() == rhs.toString() );
    }
  } else if ( rhs.userType() == qMetaTypeId<Grantlee::SafeString>() && lhs.userType() == QVariant::String ) {
    equal = ( rhs.value<Grantlee::SafeString>() == lhs.toString() );
  } else if ( rhs.userType() == qMetaTypeId<MetaEnumVariable>() ) {
    if (lhs.userType() == qMetaTypeId<MetaEnumVariable>() ) {
      equal = ( rhs.value<MetaEnumVariable>() == lhs.value<MetaEnumVariable>() );
    } else if ( lhs.type() == QVariant::Int ) {
      equal = ( rhs.value<MetaEnumVariable>() == lhs.toInt() );
    }
  } else if ( lhs.userType() == qMetaTypeId<MetaEnumVariable>() ) {
    if ( rhs.type() == QVariant::Int ) {
      equal = ( lhs.value<MetaEnumVariable>() == rhs.toInt() );
    }
  } else {
    equal = (( lhs == rhs ) && ( lhs.userType() == rhs.userType() ) );
  }
  return equal;
}

Grantlee::SafeString Grantlee::toString( const QVariantList &list )
{
  QString output( '[' );
  QVariantList::const_iterator it = list.constBegin();
  const QVariantList::const_iterator end = list.constEnd();
  while ( it != end ) {
    const QVariant item = *it;
    if ( isSafeString( item ) ) {
      output.append( "u\'" );
      output.append( getSafeString( item ) );
      output.append( '\'' );
      if (( it + 1 ) != end )
        output.append( ", " );
    }
    if ( item.type() == QVariant::List ) {
      output.append( toString( item.toList() ) );
      output.append( ", " );
    }
    ++it;
  }

  return output.append( ']' );
}

