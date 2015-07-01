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

#include "stringfilters.h"

#include "util.h"

#include <QtCore/QVariant>
#include <QtCore/QRegularExpression>

QVariant AddSlashesFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = getSafeString( input );
  safeString.get().replace( QLatin1Char( '\\' ), QStringLiteral( "\\\\" ) ).get().replace( QLatin1Char( '\"' ), QStringLiteral( "\\\"" ) ).get().replace( QLatin1Char( '\'' ), QStringLiteral( "\\\'" ) );
  return safeString;
}

QVariant CapFirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = getSafeString( input );
  if ( safeString.get().isEmpty() )
    return QString();

  return QVariant(safeString.get().at( 0 ).toUpper() + static_cast<QString>(safeString.get().right( safeString.get().size() - 1 )));
}


EscapeJsFilter::EscapeJsFilter()
{
}

static QList<QPair<QString, QString> > getJsEscapes()
{
  QList<QPair<QString, QString> > jsEscapes;
  jsEscapes << QPair<QString, QString>( QChar::fromLatin1( '\\' ), QStringLiteral( "\\u005C" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '\'' ),  QStringLiteral( "\\u0027" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '\"' ),  QStringLiteral( "\\u0022" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '>' ), QStringLiteral( "\\u003E" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '<' ), QStringLiteral( "\\u003C" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '&' ), QStringLiteral( "\\u0026" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '=' ), QStringLiteral( "\\u003D" ) )
            << QPair<QString, QString>( QChar::fromLatin1( '-' ), QStringLiteral( "\\u002D" ) )
            << QPair<QString, QString>( QChar::fromLatin1( ';' ), QStringLiteral( "\\u003B" ) )
            << QPair<QString, QString>( QChar( 0x2028 ), QStringLiteral( "\\u2028" ) )
            << QPair<QString, QString>( QChar( 0x2029 ), QStringLiteral( "\\u2029" ) );

  for( int i = 0; i < 32; ++i )
  {
    jsEscapes << QPair<QString, QString>( QChar( i ), QStringLiteral( "\\u00" ) + QString::fromLatin1( "%1" ).arg( i, 2, 16, QLatin1Char( '0' ) ).toUpper() );
  }
  return jsEscapes;
}

QVariant EscapeJsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QString retString = getSafeString( input );

  static const QList<QPair<QString, QString> > jsEscapes = getJsEscapes();

  QListIterator<QPair<QString, QString> > it( jsEscapes );

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

  const QRegularExpression fixAmpersandsRegexp( QStringLiteral( "&(?!(\\w+|#\\d+);)" ) );

  safeString.get().replace( fixAmpersandsRegexp, QStringLiteral( "&amp;" ) );

  return safeString;
}

QVariant CutFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString retString = getSafeString( input );
  SafeString argString = getSafeString( argument );

  bool inputSafe = retString.isSafe();

  retString.get().remove( argString );

  if ( inputSafe && argString.get() != QChar::fromLatin1( ';' ) )
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
  QStringList lines = safeString.get().split( QLatin1Char( '\n' ) );
  int width = QString::number( lines.size() ).size();

  const bool shouldEscape = ( autoescape && !safeString.isSafe() );
  for ( int i = 0; i < lines.size(); ++i ) {
    lines[ i ] = QString::fromLatin1( "%1. %2" ).arg( i + 1, width ).arg(
                   shouldEscape ? QString( escape( lines.at( i ) ) ) : lines.at( i )
                 );
  }

  return markSafe( lines.join( QChar::fromLatin1( '\n' ) ) );
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

  QString str = getSafeString( input );

  QString::iterator it = str.begin();
  const QString::iterator end = str.end();

  bool toUpper = true;
  for ( ; it != end; ++it ) {
    if ( toUpper )
      *it = it->toUpper();
    else
      *it = it->toLower();
    toUpper = it->isSpace();
  }

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
  QStringList words = inputString.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );

  if ( words.size() > numWords ) {
    words = words.mid( 0, numWords );
    if ( !words.at( words.size() - 1 ).endsWith( QStringLiteral( "..." ) ) ) {
      words << QStringLiteral( "..." );
    }
  }
  return words.join( QChar::fromLatin1( ' ' ) );

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
  return QString::number( getSafeString( input ).get().split( QLatin1Char( ' ' ) ).size() );
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
  const QStringList tags = getSafeString( argument ).get().split( QLatin1Char( ' ' ) );
  const QString tagRe = QString::fromLatin1( "(%1)" ).arg( tags.join( QChar::fromLatin1( '|' ) ) );
  const QRegularExpression startTag( QString::fromLatin1( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  const QRegularExpression endTag( QString::fromLatin1( "</%1>" ).arg( tagRe ) );

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
  static QRegularExpression tagRe( QStringLiteral( "<[^>]*>" ), QRegularExpression::InvertedGreedinessOption );

  QString value = getSafeString( input );
  value.remove( tagRe );
  return value;
}

QVariant WordWrapFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QString _input = getSafeString( input );
  int width = argument.toInt();
  QStringList partList = _input.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );
  QString output = partList.takeFirst();
  int pos = output.size() - output.lastIndexOf( QLatin1Char( '\n' ) ) - 1;
  Q_FOREACH( const QString &part, partList ) {
    QStringList lines;
    if ( part.contains( QLatin1Char( '\n' ) ) ) {
      lines = part.split( QLatin1Char( '\n' ) );
    } else {
      lines.append( part );
    }
    pos += lines.first().size() + 1;
    if ( pos > width ) {
      output.append( QLatin1Char( '\n' ) );
      pos += lines.last().size();
    } else {
      output.append( QLatin1Char( ' ' ) );
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
  static const QRegularExpression re( QStringLiteral( "\n{2,}" ) );
  QStringList output;

  Q_FOREACH( const QString &bit, inputString.get().split( re ) ) {
    SafeString _bit = SafeString( bit, inputString.isSafe() );
    if ( autoescape )
      _bit = conditionalEscape( _bit );
    _bit.get().replace( QLatin1Char( '\n' ), QStringLiteral( "<br />" ) );
    output.append( QString::fromLatin1( "<p>%1</p>" ).arg( _bit ) );
  }
  return markSafe( output.join( QStringLiteral( "\n\n" ) ) );
}

QVariant LineBreaksBrFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString inputString = getSafeString( input );
  if ( autoescape && isSafeString( input ) ) {
    inputString = conditionalEscape( inputString );
  }
  return markSafe( inputString.get().replace( QLatin1Char( '\n' ), QStringLiteral( "<br />" ) ) );
}

static QString nofailStringToAscii( const QString &input )
{
  QString output;
  output.reserve( input.size() );

  QString::const_iterator it = input.constBegin();
  const QString::const_iterator end = input.constEnd();
  static const QChar asciiEndPoint( 128 );
  for ( ; it != end; ++it )
    if ( *it < asciiEndPoint )
      output.append( *it );

  return output;
}

QVariant SlugifyFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QString inputString = getSafeString( input );
  inputString = inputString.normalized( QString::NormalizationForm_KD );
  inputString = nofailStringToAscii( inputString );
  inputString = inputString.remove( QRegularExpression( QStringLiteral( "[^\\w\\s-]" ) ) ).trimmed().toLower();
  return markSafe( inputString.replace( QRegularExpression( QStringLiteral( "[-\\s]+" ) ), QChar::fromLatin1( '-' ) ) );
}
