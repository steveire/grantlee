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

#include "lists.h"
#include <util_p.h>
#include <QDateTime>

JoinFilter::JoinFilter()
{
}

QVariant JoinFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QVariantList varList = Util::variantToList( input );
  QListIterator<QVariant> i( varList );
  QString ret;
  while ( i.hasNext() ) {
    QVariant var = i.next();
    Grantlee::SafeString s = Util::getSafeString( var );
    if ( autoescape )
      s = Util::conditionalEscape( s );

    ret.append( s );
    if ( i.hasNext() ) {
      SafeString argString = Util::getSafeString( argument );
      ret.append( argString );
    }
  }
  return Util::markSafe( ret );
}

LengthFilter::LengthFilter()
{
}

QVariant LengthFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  if ( input.type() == QVariant::List )
    return input.toList().size();

  if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String )
    return Util::getSafeString( input )->size();

  return QVariant();
}

LengthIsFilter::LengthIsFilter()
{
}

QVariant LengthIsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  if ( !input.isValid() || ( input.type() == QVariant::Int ) || ( input.type() == QVariant::DateTime ) )
    return QVariant();

  int size = 0;
  if ( input.type() == QVariant::List )
    size = input.toList().size();

  if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String )
    size = Util::getSafeString( input )->size();

  bool ok;
  int argInt = Util::getSafeString( argument )->toInt( &ok );

  if ( !ok )
    return QVariant();

  return size == argInt;
}

FirstFilter::FirstFilter()
{
}

QVariant FirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return varList.at( 0 );
}


LastFilter::LastFilter()
{
}

QVariant LastFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  QVariantList varList = Util::variantToList( input );

  if ( varList.isEmpty() )
    return QString();

  return varList.at( varList.size() - 1 );
}


RandomFilter::RandomFilter()
{
}

QVariant RandomFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  QVariantList varList = Util::variantToList( input );

  qsrand( QDateTime::currentDateTime().toTime_t() );
  int rnd = qrand() % varList.size();
  return varList.at( rnd );
}

SliceFilter::SliceFilter()
{
}

QVariant SliceFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString argString = Util::getSafeString( argument );
  int splitterIndex = argString->indexOf( ":" );
  QString inputString = Util::getSafeString( input );
  if ( splitterIndex >= 0 ) {
    int left = QVariant( argString->left( splitterIndex ) ).toInt();
    int right = QVariant( argString->right( splitterIndex ) ).toInt();
    if ( right < 0 ) {
      right = inputString.size() + right;
    }
    return inputString.mid( left, right );
  } else {
    return QString( inputString.at( argument.toInt() ) );
  }
}

QVariant MakeListFilter::doFilter( const QVariant& _input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  if ( _input.type() == QVariant::List )
    return _input;

  QVariant input = _input;

  if ( input.type() == QVariant::Int )
    input.convert( QVariant::String );

  if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String ) {
    QVariantList list;
    foreach ( const QVariant &var, Util::getSafeString( input )->split( QString(), QString::SkipEmptyParts ) )
      list << var;
    return list;
  }
  return QVariant();
}

QVariant UnorderedListFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  return Util::markSafe( processList( input.toList(), 1, autoescape ) );
}

SafeString UnorderedListFilter::processList( const QVariantList& list, int tabs, bool autoescape ) const
{
  QString indent;
  for ( int i = 0; i < tabs; ++i )
    indent.append( "\t" );
  QStringList output;

  int i = 0;
  int listSize = list.size();
  while ( i < listSize ) {
    QVariant titleObject = list.at( i );
    SafeString title = Util::getSafeString( titleObject );;
    QString sublist;
    QVariant sublistItem;

    if ( titleObject.type() == QVariant::List ) {
      sublistItem = titleObject;
      title->clear();
    } else if ( i < listSize - 1 ) {
      QVariant nextItem = list.at( i + 1 );
      if ( nextItem.type() == QVariant::List ) {
        sublistItem = nextItem;
      }
      ++i;
    }
    if ( sublistItem.isValid() ) {
      sublist = processList( sublistItem.toList(), tabs + 1, autoescape );
      sublist = QString( "\n%1<ul>\n%2\n%3</ul>\n%4" ).arg( indent ).arg( sublist ).arg( indent ).arg( indent );
    }
    output.append( QString( "%1<li>%2%3</li>" ).arg( indent )
                                               .arg( autoescape ? Util::conditionalEscape( title ) : title )
                                               .arg( sublist ) );
    ++i;
  }

  return output.join( "\n" );
}
