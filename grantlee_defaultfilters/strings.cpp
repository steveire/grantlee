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

#include "strings.h"

#include <QVariant>


AddSlashesFilter::AddSlashesFilter( QObject* parent )
    : Filter( parent )
{

}


Grantlee::SafeString AddSlashesFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input ).rawString();

  retString.replace( "\\", "\\\\" ).replace( '"', "\\\"" ).replace( '\'', "\\'" );

  return retString;
}

CapFirstFilter::CapFirstFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString CapFirstFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input ).rawString();
  if ( retString.isEmpty() )
    return QString();

  return retString.at( 0 ).toUpper() + retString.right( retString.size() - 1 );

}


EscapeJsFilter::EscapeJsFilter( QObject* parent )
    : Filter( parent )
{

}



Grantlee::SafeString EscapeJsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input ).rawString();

  QList<QPair<QString, QString> > jsEscapes;

  jsEscapes << QPair<QString, QString>( "\\", "\\x5C" );

  QListIterator<QPair<QString, QString> > it( jsEscapes );

  while ( it.hasNext() ) {
    QPair<QString, QString> escape = it.next();
    retString.replace( escape.first, escape.second );
  }
  return retString;
}

FixAmpersandsFilter::FixAmpersandsFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString FixAmpersandsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input ).rawString();

  QRegExp fixAmpersandsRegexp( "&(?!(\\w+|#\\d+);)" );

  retString.replace( fixAmpersandsRegexp, "&amp;" );

  return retString;
}

CutFilter::CutFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString CutFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input ).rawString();

  retString.remove( argument );

  return retString;
}

SafeFilter::SafeFilter( QObject* parent ): Filter( parent )
{

}

Grantlee::SafeString SafeFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::markSafe( Util::getSafeString( input ) );
}

LineNumbersFilter::LineNumbersFilter( QObject* parent )
    : Filter( parent )
{

}


SafeString LineNumbersFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  SafeString safeString = Util::getSafeString( input );
  QStringList lines = safeString.rawString().split( '\n' );
  int width = QString::number( lines.size() ).size();

  const bool shouldEscape = ( !autoescape || safeString.isSafe() );

  for ( int i = 0; i < lines.size(); ++i ) {
    lines[ i ] = QString( "%0" + QString::number( width ) + "d. %s" ).arg( i + 1 ).arg(
                  shouldEscape ? Util::escape( lines.at( i ) ).rawString() : lines.at( i )
                  );
  }

  return Util::markSafe( lines.join( QString( '\n' ) ) );
}

LowerFilter::LowerFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString LowerFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rawString().toLower();
}

//TODO: consider making doFilter return a QVariant.
// MakeListFilter::MakeListFilter(QObject* parent)
//     : Filter(parent)
// {
//
// }
//
// SafeString MakeListFilter::doFilter(const QVariant& input, const Grantlee::SafeString& argument, bool autoescape) const
// {
//   return Util::getSafeString( input ).rawString().split();
// }


StringFormatFilter::StringFormatFilter( QObject* parent )
    : Filter( parent )
{

}


SafeString StringFormatFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  return argument.rawString().arg( Util::getSafeString( input ).rawString() );
}

TitleFilter::TitleFilter( QObject* parent ): Filter( parent )
{

}

SafeString TitleFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
//   QRegExp re( "\b([a-z])" );

  QRegExp re( "([a-z])'([A-Z])" );

  QString str = Util::getSafeString( input ).rawString();

  str.replace( re, QString( "\\1" ).toUpper() );

  return str;
}

TruncateWordsFilter::TruncateWordsFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString TruncateWordsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QVariant v( argument );
  if ( !v.convert( QVariant::Int ) ) {
    return input.toString();
  }
  int numWords = v.toInt();

  QString inputString = Util::getSafeString( input ).rawString();
  QStringList words = inputString.split( " ", QString::SkipEmptyParts );

  if ( words.size() > numWords ) {
    words = words.mid( 0, numWords );
    if ( !words.at( words.size() - 1 ).endsWith( "..." ) ) {
      words << "...";
    }
  }
  return words.join( " " );

}

UpperFilter::UpperFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString UpperFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rawString().toUpper();
}

WordCountFilter::WordCountFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString WordCountFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return QString::number( Util::getSafeString( input ).rawString().split( " " ).size() );
}

LJustFilter::LJustFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString LJustFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rawString().leftJustified( QVariant( argument ).toInt() );
}


RJustFilter::RJustFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString RJustFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rawString().rightJustified( QVariant( argument ).toInt() );
}

CenterFilter::CenterFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString CenterFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  QString value = Util::getSafeString( input ).rawString();
  const int valueWidth = value.size();
  const int width = QVariant( argument ).toInt();
  const int totalPadding = valueWidth - width;
  const int leftPadding = totalPadding >> 1;
  const int rightPadding = totalPadding - leftPadding;

  return value.leftJustified( rightPadding ).rightJustified( leftPadding );
}


EscapeFilter::EscapeFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString EscapeFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::markForEscaping( Util::getSafeString( input ) );
}

ForceEscapeFilter::ForceEscapeFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString ForceEscapeFilter::doFilter( const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::markSafe( Util::escape( Util::getSafeString( input ) ) );
}

RemoveTagsFilter::RemoveTagsFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString RemoveTagsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QStringList tags = argument.rawString().split( " " );
  QString tagRe = QString( "(%1)" ).arg( tags.join( "|" ) );
  QRegExp startTag( QString( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  QRegExp endTag( QString( "</%1>" ).arg( tagRe ) );

  QString value = Util::getSafeString( input );
  value.replace( startTag, "" );
  value.replace( endTag, "" );
  return value;
}

StripTagsFilter::StripTagsFilter( QObject* parent )
    : Filter( parent )
{

}

Grantlee::SafeString StripTagsFilter::doFilter( const QVariant& input, const Grantlee::SafeString& argument, bool autoescape ) const
{
  QRegExp tagRe( "<[^>]*?>" );

  QString value = Util::getSafeString( input );
  value.replace( tagRe, "" );;
  return value;
}


