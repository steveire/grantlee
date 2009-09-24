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

#include "util_p.h"


AddSlashesFilter::AddSlashesFilter()
{
}


QVariant AddSlashesFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString safeString = Util::getSafeString( input );
  safeString.replace( "\\", "\\\\" ).replace( "\"", "\\\"" ).replace( "\'", "\\\'" );
  return safeString;
}

CapFirstFilter::CapFirstFilter()
{
}

QVariant CapFirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString safeString = Util::getSafeString( input );
  if ( safeString.isEmpty() )
    return QString();

  return safeString.at( 0 ).toUpper() + safeString.right( safeString.size() - 1 );
}


EscapeJsFilter::EscapeJsFilter()
{
}



QVariant EscapeJsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QString retString = Util::getSafeString( input );

  QList<QPair<QString, QString> > jsEscapes;

  jsEscapes << QPair<QString, QString>( "\\", "\\x5C" );

  QListIterator<QPair<QString, QString> > it( jsEscapes );

  while ( it.hasNext() ) {
    QPair<QString, QString> escape = it.next();
    retString.replace( escape.first, escape.second );
  }
  return retString;
}

FixAmpersandsFilter::FixAmpersandsFilter()
{
}

QVariant FixAmpersandsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString safeString = Util::getSafeString( input );

  QRegExp fixAmpersandsRegexp( "&(?!(\\w+|#\\d+);)" );

  safeString.replace( fixAmpersandsRegexp, "&amp;" );

  return safeString;
}

CutFilter::CutFilter()
{
}

QVariant CutFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString retString = Util::getSafeString( input );
  SafeString argString = Util::getSafeString( argument );

  retString.remove( argString );

  if ( argString == ";" )
    retString.setSafety( SafeString::IsNotSafe );

  return retString;
}

SafeFilter::SafeFilter()
{
}

QVariant SafeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::markSafe( Util::getSafeString( input ) );
}

LineNumbersFilter::LineNumbersFilter()
{
}


QVariant LineNumbersFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString safeString = Util::getSafeString( input );
  QStringList lines = safeString.split( '\n' );
  int width = QString::number( lines.size() ).size();

  const bool shouldEscape = ( autoescape && !safeString.isSafe() );
  for ( int i = 0; i < lines.size(); ++i ) {
    lines[ i ] = QString( "%1. %2" ).arg( i + 1, width ).arg(
                   shouldEscape ? Util::escape( lines.at( i ) ) : lines.at( i )
                 );
  }

  return Util::markSafe( lines.join( QString( '\n' ) ) );
}

LowerFilter::LowerFilter()
{
}

QVariant LowerFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).toLower();
}

//TODO: consider making doFilter return a QVariant.
// MakeListFilter::MakeListFilter()
//     : Filter(parent)
// {
//
// }
//
// SafeString MakeListFilter::doFilter(const QVariant& input, const QVariant &argument, bool autoescape) const
// {
//   return Util::getSafeString( input ).split();
// }


StringFormatFilter::StringFormatFilter()
{

}


QVariant StringFormatFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString a = Util::getSafeString( input );
  return SafeString( Util::getSafeString( argument ).arg( a ), Util::getSafeString( input ).isSafe() );
}

TitleFilter::TitleFilter()
{
}

QVariant TitleFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
//   QRegExp re( "\b([a-z])" );

  QRegExp re( "([a-z])'([A-Z])" );

  QString str = Util::getSafeString( input );

  str.replace( re, QString( "\\1" ).toUpper() );

  return str;
}

TruncateWordsFilter::TruncateWordsFilter()
{
}

QVariant TruncateWordsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QVariant v( Util::getSafeString( argument ) );
  if ( !v.convert( QVariant::Int ) ) {
    return input.toString();
  }
  int numWords = v.toInt();

  QString inputString = Util::getSafeString( input );
  QStringList words = inputString.split( " ", QString::SkipEmptyParts );

  if ( words.size() > numWords ) {
    words = words.mid( 0, numWords );
    if ( !words.at( words.size() - 1 ).endsWith( "..." ) ) {
      words << "...";
    }
  }
  return words.join( " " );

}

UpperFilter::UpperFilter()
{
}

QVariant UpperFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).toUpper();
}

WordCountFilter::WordCountFilter()
{
}

QVariant WordCountFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return QString::number( Util::getSafeString( input ).split( " " ).size() );
}

LJustFilter::LJustFilter()
{
}

QVariant LJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).leftJustified( QVariant( Util::getSafeString( argument ) ).toInt() );
}


RJustFilter::RJustFilter()
{
}

QVariant RJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::getSafeString( input ).rightJustified( QVariant( Util::getSafeString( argument ) ).toInt() );
}

CenterFilter::CenterFilter()
{
}

QVariant CenterFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QString value = Util::getSafeString( input );
  const int valueWidth = value.size();
  const int width = QVariant( Util::getSafeString( argument ) ).toInt();
  const int totalPadding = width - valueWidth;
  const int rightPadding = totalPadding >> 1;

  return value.leftJustified( valueWidth + rightPadding ).rightJustified( width );
}


EscapeFilter::EscapeFilter()
{
}

QVariant EscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::markForEscaping( Util::getSafeString( input ) );
}

ForceEscapeFilter::ForceEscapeFilter()
{
}

QVariant ForceEscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  return Util::markSafe( Util::escape( Util::getSafeString( input ) ) );
}

RemoveTagsFilter::RemoveTagsFilter()
{
}

QVariant RemoveTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QStringList tags = Util::getSafeString( argument ).split( " " );
  QString tagRe = QString( "(%1)" ).arg( tags.join( "|" ) );
  QRegExp startTag( QString( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  QRegExp endTag( QString( "</%1>" ).arg( tagRe ) );

  SafeString value = Util::getSafeString( input );
  value.replace( startTag, "" );
  value.replace( endTag, "" );
  return value;
}

StripTagsFilter::StripTagsFilter()
{
}

QVariant StripTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QRegExp tagRe( "<[^>]*>" );
  tagRe.setMinimal( true );

  QString value = Util::getSafeString( input );
  value.replace( tagRe, "" );;
  return value;
}


QVariant WordWrapFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  QString _input = Util::getSafeString( input );
  int width = argument.toInt();
  QStringList partList = _input.split( " ", QString::SkipEmptyParts );
  QString output = partList.takeFirst();
  int pos = output.size() - output.lastIndexOf("\n") - 1;
  foreach( const QString &part, partList )
  {
    QStringList lines;
    if ( part.contains( "\n" ) )
    {
      lines = part.split( "\n" );
    } else {
      lines.append( part );
    }
    pos += lines.first().size() + 1;
    if ( pos > width )
    {
      output.append( "\n" );
      pos += lines.last().size();
    } else {
      output.append( " " );
      if ( lines.size() > 1 )
        pos += lines.last().size();
    }
    output.append( part );
  }
  return output;
}


