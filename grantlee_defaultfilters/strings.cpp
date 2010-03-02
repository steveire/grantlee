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

QVariant AddSlashesFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = Util::getSafeString( input );
  safeString->replace( '\\', "\\\\" ).replace( '\"', "\\\"" ).replace( '\'', "\\\'" );
  return safeString;
}

QVariant CapFirstFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  SafeString safeString = Util::getSafeString( input );
  if ( safeString->isEmpty() )
    return QString();

  return safeString->at( 0 ).toUpper() + safeString->right( safeString->size() - 1 );
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
  QString retString = Util::getSafeString( input );

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
  SafeString safeString = Util::getSafeString( input );

  QRegExp fixAmpersandsRegexp( "&(?!(\\w+|#\\d+);)" );

  safeString->replace( fixAmpersandsRegexp, "&amp;" );

  return safeString;
}

QVariant CutFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString retString = Util::getSafeString( input );
  SafeString argString = Util::getSafeString( argument );

  retString->remove( argString );

  if ( argString == ";" )
    retString.setSafety( SafeString::IsNotSafe );

  return retString;
}

QVariant SafeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return Util::markSafe( Util::getSafeString( input ) );
}

QVariant LineNumbersFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString safeString = Util::getSafeString( input );
  QStringList lines = safeString->split( '\n' );
  int width = QString::number( lines.size() ).size();

  const bool shouldEscape = ( autoescape && !safeString.isSafe() );
  for ( int i = 0; i < lines.size(); ++i ) {
    lines[ i ] = QString( "%1. %2" ).arg( i + 1, width ).arg(
                   shouldEscape ? QString( Util::escape( lines.at( i ) ) ) : lines.at( i )
                 );
  }

  return Util::markSafe( lines.join( QString( '\n' ) ) );
}

QVariant LowerFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return Util::getSafeString( input )->toLower();
}

QVariant StringFormatFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString a;
  if ( Util::isSafeString( input ) )
    a = Util::getSafeString( input );
  else if ( input.type() == QVariant::List ) {
    a = Util::toString( input.toList() );
  }

  return SafeString( Util::getSafeString( argument )->arg( a ), Util::getSafeString( input ).isSafe() );
}

QVariant TitleFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
//   QRegExp re( "\b([a-z])" );

  QRegExp re( "([a-z])'([A-Z])" );

  QString str = Util::getSafeString( input );

  str.replace( re, QString( "\\1" ).toUpper() );

  return str;
}

QVariant TruncateWordsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  SafeString s = Util::getSafeString( argument );

  bool ok;
  int numWords = s->toInt(&ok);

  if ( !ok ) {
    return input.toString();
  }

  QString inputString = Util::getSafeString( input );
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
  return Util::getSafeString( input )->toUpper();
}

QVariant WordCountFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return QString::number( Util::getSafeString( input )->split( ' ' ).size() );
}

QVariant LJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return Util::getSafeString( input )->leftJustified( Util::getSafeString( argument )->toInt() );
}

QVariant RJustFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  return Util::getSafeString( input )->rightJustified( Util::getSafeString( argument )->toInt() );
}

QVariant CenterFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QString value = Util::getSafeString( input );
  const int valueWidth = value.size();
  const int width = Util::getSafeString( argument )->toInt();
  const int totalPadding = width - valueWidth;
  const int rightPadding = totalPadding >> 1;

  return value.leftJustified( valueWidth + rightPadding ).rightJustified( width );
}

QVariant EscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return Util::markForEscaping( Util::getSafeString( input ) );
}

QVariant ForceEscapeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  return Util::markSafe( Util::escape( Util::getSafeString( input ) ) );
}

QVariant RemoveTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QStringList tags = Util::getSafeString( argument )->split( ' ' );
  QString tagRe = QString( "(%1)" ).arg( tags.join( "|" ) );
  QRegExp startTag( QString( "<%1(/?>|(\\s+[^>]*>))" ).arg( tagRe ) );
  QRegExp endTag( QString( "</%1>" ).arg( tagRe ) );

  SafeString value = Util::getSafeString( input );
  value->remove( startTag );
  value->remove( endTag );
  return value;
}

QVariant StripTagsFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QRegExp tagRe( "<[^>]*>" );
  tagRe.setMinimal( true );

  QString value = Util::getSafeString( input );
  value.remove( tagRe );
  return value;
}

QVariant WordWrapFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( autoescape )
  QString _input = Util::getSafeString( input );
  int width = argument.toInt();
  QStringList partList = _input.split( ' ', QString::SkipEmptyParts );
  QString output = partList.takeFirst();
  int pos = output.size() - output.lastIndexOf( '\n' ) - 1;
  foreach( const QString &part, partList ) {
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
  double _input = Util::getSafeString( input )->toDouble();
  int precision;
  if ( argument.isValid() )
    precision = Util::getSafeString( argument )->toInt();
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
    foreach(const QVariant &item, input.toList() )
      list << Util::markSafe( Util::getSafeString( item ) );
  return list;
}

QVariant LineBreaksFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString inputString = Util::getSafeString( input );
  QRegExp re( "\n{2,}" );
  QStringList output;

  foreach( const QString &bit, inputString->split( re ) ) {
    SafeString _bit = SafeString( bit, inputString.isSafe() );
    if ( autoescape )
      _bit = Util::conditionalEscape( _bit );
    _bit->replace( '\n', "<br />" );
    output.append( QString( "<p>%1</p>" ).arg( _bit ) );
  }
  return Util::markSafe( output.join( "\n\n" ) );
}

QVariant LineBreaksBrFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  SafeString inputString = Util::getSafeString( input );
  if ( autoescape && Util::isSafeString( input ) ) {
    inputString = Util::conditionalEscape( inputString );
  }
  return Util::markSafe( inputString->replace( '\n', "<br />" ) );
}

QVariant SlugifyFilter::doFilter( const QVariant& input, const QVariant& argument, bool autoescape ) const
{
  Q_UNUSED( argument )
  Q_UNUSED( autoescape )
  QString inputString = Util::getSafeString( input );
  inputString = inputString.normalized( QString::NormalizationForm_KD ).toAscii();
  inputString = inputString.remove( QRegExp( "[^\\w\\s-]" ) ).trimmed().toLower();
  return Util::markSafe( inputString.replace( QRegExp( "[-\\s]+" ), QChar( '-' ) ) );
}
