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

#include "strings.h"

#include <QtCore/QVariant>

#include "util.h"

QVariant AddSlashesFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = getSafeString( input );
  safeString.get().replace( '\\', "\\\\" ).get().replace( '\"', "\\\"" ).get().replace( '\'', "\\\'" );
  return safeString;
}

QVariant CapFirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = getSafeString( input );
  if ( safeString.get().isEmpty() )
    return QString();

  return safeString.get().at( 0 ).toUpper() + safeString.get().right( safeString.get().size() - 1 );
}


EscapeJsFilter::EscapeJsFilter()
{
  m_jsEscapes << QPair<QString, QString>( "\\", "\\x5C" )
              << QPair<QString, QString>( "\'",  "\\x27" )
              << QPair<QString, QString>( "\"",  "\\x22" )
              << QPair<QString, QString>( ">", "\\x3E" )
              << QPair<QString, QString>( "<", "\\x3C" )
              << QPair<QString, QString>( "&", "\\x26" )
              << QPair<QString, QString>( "=", "\\x3D" )
              << QPair<QString, QString>( "-", "\\x2D" )
              << QPair<QString, QString>( ";", "\\x3B" )
              << QPair<QString, QString>( QChar( 0x2028 ), "\\u2028" )
              << QPair<QString, QString>( QChar( 0x2029 ), "\\u2029" );

  for( int i = 0; i < 32; ++i )
  {
    m_jsEscapes << QPair<QString, QString>( QChar( i ), "\\x" + QString( "%1" ).arg( i, 2, 16, QChar('0') ).toUpper() );
  }
}

QVariant EscapeJsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QString retString = getSafeString( input );

  QListIterator<QPair<QString, QString> > it( m_jsEscapes );

  while ( it.hasNext() ) {
    QPair<QString, QString> escape = it.next();
    retString = retString.replace( escape.first, escape.second );
  }
  return retString;
}

QVariant FixAmpersandsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = getSafeString( input );

  static const QRegExp fixAmpersandsRegexp( "&(?!(\\w+|#\\d+);)" );

  safeString.get().replace( fixAmpersandsRegexp, "&amp;" );

  return safeString;
}

QVariant CutFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString retString = getSafeString( input );
  SafeString argString = getSafeString( argument );

  bool inputSafe = retString.isSafe();

  retString.get().remove( argString );

  if ( inputSafe && argString.get() != ";" )
    return markSafe( retString );
  else
    return retString;
}

QVariant SafeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return markSafe( getSafeString( input ) );
}

QVariant LineNumbersFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString safeString = getSafeString( input );
  QStringList lines = safeString.get().split( '\n' );
  int width = QString::number( lines.size() ).size();

  const bool shouldEscape = ( autoescape && !safeString.isSafe() );
  for ( int i = 0; i < lines.size(); ++i ) {
    lines[ i ] = QString( "%1. %2" ).arg( i + 1, width ).arg(
                   shouldEscape ? QString( escape( lines.at( i ) ) ) : lines.at( i )
                 );
  }

  return markSafe( lines.join( QString( '\n' ) ) );
}

QVariant LowerFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return getSafeString( input ).get().toLower();
}

QVariant StringFormatFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString a;
  if ( isSafeString( input ) )
    a = getSafeString( input );
  else if ( input.type() == QVariant::List ) {
    a = toString( input.toList() );
  }

  return SafeString( getSafeString( argument ).get().arg( a ), getSafeString( input ).isSafe() );
}

QVariant TitleFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
//   QRegExp re( "\b([a-z])" );

  static const QRegExp re( "([a-z])'([A-Z])" );

  QString str = getSafeString( input );

  str.replace( re, QString( "\\1" ).toUpper() );

  return str;
}

QVariant TruncateWordsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString s = getSafeString( argument );

  bool ok;
  int numWords = s.get().toInt( &ok );

  if ( !ok ) {
    return input.toString();
  }

  QString inputString = getSafeString( input );
  QStringList words = inputString.split( ' ', QString::SkipEmptyParts );

  if ( words.size() > numWords ) {
    words = words.mid( 0, numWords );
    if ( !words.at( words.size() - 1 ).endsWith( QLatin1String( "..." ) ) ) {
      words << "...";
    }
  }
  return words.join( QChar( ' ' ) );

}

QVariant UpperFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return getSafeString( input ).get().toUpper();
}

QVariant WordCountFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return QString::number( getSafeString( input ).get().split( ' ' ).size() );
}

QVariant LJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return getSafeString( input ).get().leftJustified( getSafeString( argument ).get().toInt() );
}

QVariant RJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return getSafeString( input ).get().rightJustified( getSafeString( argument ).get().toInt() );
}

QVariant CenterFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QString value = getSafeString( input );
  const int valueWidth = value.size();
  const int width = getSafeString( argument ).get().toInt();
  const int totalPadding = width - valueWidth;
  const int rightPadding = totalPadding >> 1;

  return value.leftJustified( valueWidth + rightPadding ).rightJustified( width );
}

QVariant EscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return markForEscaping( getSafeString( input ) );
}

QVariant ForceEscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return markSafe( escape( getSafeString( input ) ) );
}

QVariant RemoveTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QStringList tags = getSafeString( argument ).get().split( ' ' );
  QString tagRe = QString( "(%1)" ).arg( tags.join( "|" ) );
  QRegExp startTag( QString( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  QRegExp endTag( QString( "</%1>" ).arg( tagRe ) );

  SafeString value = getSafeString( input );
  const bool safeInput = value.isSafe();
  value.get().remove( startTag );
  value.get().remove( endTag );
  if ( safeInput )
    return markSafe( value );
  return value;
}

QVariant StripTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  static QRegExp tagRe( "<[^>]*>" );
  tagRe.setMinimal( true );

  QString value = getSafeString( input );
  value.remove( tagRe );
  return value;
}

QVariant WordWrapFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QString _input = getSafeString( input );
  int width = argument.toInt();
  QStringList partList = _input.split( ' ', QString::SkipEmptyParts );
  QString output = partList.takeFirst();
  int pos = output.size() - output.lastIndexOf( '\n' ) - 1;
  Q_FOREACH( const QString &part, partList ) {
    QStringList lines;
    if ( part.contains( '\n' ) ) {
      lines = part.split( '\n' );
    } else {
      lines.append( part );
    }
    pos += lines.first().size() + 1;
    if ( pos > width ) {
      output.append( '\n' );
      pos += lines.last().size();
    } else {
      output.append( ' ' );
      if ( lines.size() > 1 )
        pos += lines.last().size();
    }
    output.append( part );
  }
  return output;
}

QVariant FloatFormatFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  double _input = getSafeString( input ).get().toDouble();
  int precision;
  if ( argument.isValid() )
    precision = getSafeString( argument ).get().toInt();
  else
    precision = 1;

  return QString::number( _input, 'f', precision );
}

QVariant SafeSequenceFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QVariantList list;
  if ( input.type() == QVariant::List )
    Q_FOREACH( const QVariant &item, input.toList() )
      list << markSafe( getSafeString( item ) );
  return list;
}

QVariant LineBreaksFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString inputString = getSafeString( input );
  static const QRegExp re( "\n{2,}" );
  QStringList output;

  Q_FOREACH( const QString &bit, inputString.get().split( re ) ) {
    SafeString _bit = SafeString( bit, inputString.isSafe() );
    if ( autoescape )
      _bit = conditionalEscape( _bit );
    _bit.get().replace( '\n', "<br />" );
    output.append( QString( "<p>%1</p>" ).arg( _bit ) );
  }
  return markSafe( output.join( "\n\n" ) );
}

QVariant LineBreaksBrFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString inputString = getSafeString( input );
  if ( autoescape && isSafeString( input ) ) {
    inputString = conditionalEscape( inputString );
  }
  return markSafe( inputString.get().replace( '\n', "<br />" ) );
}

QVariant SlugifyFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QString inputString = getSafeString( input );
  inputString = inputString.normalized( QString::NormalizationForm_KD ).toAscii();
  inputString = inputString.remove( QRegExp( "[^\\w\\s-]" ) ).trimmed().toLower();
  return markSafe( inputString.replace( QRegExp( "[-\\s]+" ), QChar( '-' ) ) );
}

