/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010,2011 Stephen Kelly <steveire@gmail.com>

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

#include "lists.h"

#include "metatype.h"
#include "util.h"
#include "variable.h"

#include <QtCore/QDateTime>

QVariant JoinFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  if (!input.canConvert<QVariantList>())
    return QVariant();

  QSequentialIterable iter = input.value<QSequentialIterable>();

  QString ret;
  for (QSequentialIterable::const_iterator it = iter.begin();
       it != iter.end(); ++it) {
    const QVariant var = *it;
    Grantlee::SafeString s = getSafeString( var );
    if ( autoescape )
      s = conditionalEscape( s );

    ret.append( s );
    if ( (it + 1) != iter.end() ) {
      SafeString argString = getSafeString( argument );
      ret.append( conditionalEscape( argString ) );
    }
  }
  return markSafe( ret );
}

QVariant LengthFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )
  if ( input.canConvert<QVariantList>() )
    return input.value<QSequentialIterable>().size();

  if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String )
    return getSafeString( input ).get().size();

  return QVariant();
}

QVariant LengthIsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  if ( !input.isValid() || ( input.type() == QVariant::Int ) || ( input.type() == QVariant::DateTime ) )
    return QVariant();

  int size = 0;
  if ( input.canConvert<QVariantList>() )
    size = input.value<QSequentialIterable>().size();
  else if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String )
    size = getSafeString( input ).get().size();

  bool ok;
  int argInt = getSafeString( argument ).get().toInt( &ok );

  if ( !ok )
    return QVariant();

  return size == argInt;
}

QVariant FirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )

  if (!input.canConvert<QVariantList>())
    return QVariant();

  QSequentialIterable iter = input.value<QSequentialIterable>();

  if ( iter.size() == 0 )
    return QString();

  return *iter.begin();
}

QVariant LastFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )

  if (!input.canConvert<QVariantList>())
    return QVariant();

  QSequentialIterable iter = input.value<QSequentialIterable>();

  if ( iter.size() == 0 )
    return QString();

  return *(iter.end() - 1);
}

QVariant RandomFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  Q_UNUSED( argument )

  if (!input.canConvert<QVariantList>())
    return QVariant();

  QVariantList varList = input.value<QVariantList>();

  qsrand( QDateTime::currentDateTimeUtc().toTime_t() );
  int rnd = qrand() % varList.size();
  return varList.at( rnd );
}

QVariant SliceFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString argString = getSafeString( argument );
  int splitterIndex = argString.get().indexOf( QLatin1Char( ':' ) );
  QString inputString = getSafeString( input );
  if ( splitterIndex >= 0 ) {
    int left = argString.get().left( splitterIndex ).get().toInt();
    int right = argString.get().right( splitterIndex ).get().toInt();
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
  if ( _input.canConvert<QVariantList>())
    return _input.value<QVariantList>();

  QVariant input = _input;

  if ( input.type() == QVariant::Int )
    input.convert( QVariant::String );

  if ( input.userType() == qMetaTypeId<SafeString>() || input.type() == QVariant::String ) {
    QVariantList list;
    Q_FOREACH( const QVariant &var, getSafeString( input ).get().split( QString(), QString::SkipEmptyParts ) )
      list << var;
    return list;
  }
  return QVariant();
}

QVariant UnorderedListFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )

  if (!input.canConvert<QVariantList>())
    return QVariant();

  return markSafe( processList( input.value<QVariantList>(), 1, autoescape ) );
}

SafeString UnorderedListFilter::processList( const QVariantList& list, int tabs, bool autoescape ) const
{
  QString indent;
  for ( int i = 0; i < tabs; ++i )
    indent.append( QLatin1Char( '\t' ) );
  QStringList output;

  int i = 0;
  int listSize = list.size();
  while ( i < listSize ) {
    QVariant titleObject = list.at( i );
    SafeString title = getSafeString( titleObject );
    QString sublist;
    QVariant sublistItem;

    if ( titleObject.type() == QVariant::List ) {
      sublistItem = titleObject;
      title.get().clear();
    } else if ( i < listSize - 1 ) {
      QVariant nextItem = list.at( i + 1 );
      if ( nextItem.type() == QVariant::List ) {
        sublistItem = nextItem;
      }
      ++i;
    }
    if ( sublistItem.isValid() ) {
      sublist = processList( sublistItem.toList(), tabs + 1, autoescape );
      sublist = QStringLiteral( "\n%1<ul>\n%2\n%3</ul>\n%4" ).arg( indent, sublist, indent, indent );
    }
    output.append( QStringLiteral( "%1<li>%2%3</li>" ).arg( indent,
                                                                 autoescape ? conditionalEscape( title ) : title,
                                                                 sublist ) );
    ++i;
  }

  // Should be QLatin1Char() ?
  return output.join( QChar::fromLatin1( '\n' ) );
}

struct DictSortLessThan
{
  bool operator()( const QPair<QVariant, QVariant> &lp, const QPair<QVariant, QVariant> &rp ) const
  {
    const QVariant l = lp.first;
    const QVariant r = rp.first;
    switch ( l.userType() ) {
    case QVariant::Invalid:
        return (r.type() != QVariant::Invalid);
    case QVariant::Int:
        return l.toInt() < r.toInt();
    case QVariant::UInt:
        return l.toUInt() < r.toUInt();
    case QVariant::LongLong:
        return l.toLongLong() < r.toLongLong();
    case QVariant::ULongLong:
        return l.toULongLong() < r.toULongLong();
    case QMetaType::Float:
        return l.toFloat() < r.toFloat();
    case QVariant::Double:
        return l.toDouble() < r.toDouble();
    case QVariant::Char:
        return l.toChar() < r.toChar();
    case QVariant::Date:
        return l.toDate() < r.toDate();
    case QVariant::Time:
        return l.toTime() < r.toTime();
    case QVariant::DateTime:
        return l.toDateTime() < r.toDateTime();
    case QMetaType::QObjectStar:
        return l.value<QObject*>() < r.value<QObject*>();
    }
    if ( l.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
      if ( r.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
        return l.value<Grantlee::SafeString>().get() < r.value<Grantlee::SafeString>().get();
      } else if ( r.userType() == QVariant::String ) {
        return l.value<Grantlee::SafeString>().get() < r.toString();
      }
    } else if ( r.userType() == qMetaTypeId<Grantlee::SafeString>() ) {
      if ( l.userType() == QVariant::String ) {
        return l.toString() < r.value<Grantlee::SafeString>().get();
      }
    } else if ( l.userType() == QVariant::String ) {
      if ( r.userType() == QVariant::String ) {
        return l.toString() < r.toString();
      }
    }
    return false;
  }
};

QVariant DictSortFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )

  if (!input.canConvert<QVariantList>())
    return QVariant();

  QList<QPair<QVariant, QVariant> > keyList;
  const QSequentialIterable inList = input.value<QSequentialIterable>();
  Q_FOREACH( const QVariant &item, inList ) {
    QVariant var = item;

    const Variable v( getSafeString( argument ) );

    if ( v.literal().isValid() ) {
      var = MetaType::lookup( var, v.literal().toString() );
    } else {
      const QStringList lookups = v.lookups();
      Q_FOREACH( const QString &lookup, lookups ) {
        var = MetaType::lookup( var, lookup );
      }
    }
    keyList << qMakePair( var, item );
  }

  DictSortLessThan lt;
  qStableSort( keyList.begin(), keyList.end(), lt );

  QVariantList outList;
  QList<QPair<QVariant, QVariant> >::const_iterator it = keyList.constBegin();
  const QList<QPair<QVariant, QVariant> >::const_iterator end = keyList.constEnd();
  for ( ; it != end; ++it ) {
    outList << it->second;
  }
  return outList;
}
