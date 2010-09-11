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


#include "lexer_p.h"

#include "grantlee_latin1literal_p.h"
#include "grantlee_tags_p.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

using namespace Grantlee;

static QRegExp getTagRe()
{

  // In python regex, '.' matches any character except newline, whereas it QRegExp,
  // it matches any character including newline. We match 'not newline' instead.

  static const QLatin1Literal notEndOfLine( "[^\\n]*" );

  static const QString tagString = QLatin1Char( '(' )
                                 + QRegExp::escape( BLOCK_TAG_START )
                                 + notEndOfLine
                                 + QRegExp::escape( BLOCK_TAG_END )
                                 + QLatin1Char( '|' )
                                 + QRegExp::escape( VARIABLE_TAG_START )
                                 + notEndOfLine
                                 + QRegExp::escape( VARIABLE_TAG_END )
                                 + QLatin1Char( '|' )
                                 + QRegExp::escape( COMMENT_TAG_START )
                                 + notEndOfLine
                                 + QRegExp::escape( COMMENT_TAG_END )
                                 + QLatin1Char( ')' );

  static QRegExp sTagRe( tagString );

  sTagRe.setMinimal( true ); // Can't use '?', eg '.*?', Have to setMinimal instead
  return sTagRe;
}

Lexer::Lexer( const QString &templateString )
  : m_templateString( templateString )
{

}

Lexer::~Lexer()
{
}

QList<Token> Lexer::tokenize() const
{
  const QRegExp tagRe = getTagRe();

  QList<Token> tokenList;

  int pos = 0;
  int oldPosition = 0;
  int linecount = 0;
  while ( ( pos = tagRe.indexIn( m_templateString, pos ) ) != -1 ) {
    tokenList << createToken( m_templateString.mid( oldPosition, pos - oldPosition ), NotInTag, &linecount );
    tokenList << createToken( tagRe.cap( 1 ), InTag, &linecount );
    pos += tagRe.matchedLength();
    oldPosition = pos;
  }
  tokenList << createToken( m_templateString.right( m_templateString.size() - oldPosition ), NotInTag, &linecount );

  return tokenList;
}

Token Lexer::createToken( const QString &fragment, State inTag, int *linecount ) const
{
  Token token;
  token.linenumber = ( *linecount ) + 1;
  if ( inTag == NotInTag ) {
    ( *linecount ) += fragment.count( QLatin1Char( '\n' ) );
    token.content = fragment;
    token.tokenType = TextToken;
  } else {
    static const int startTagSize = 2;
    static const int endTagSize = 2;
    QString content = fragment.mid( startTagSize, fragment.size() - startTagSize - endTagSize );
    if ( fragment.startsWith( VARIABLE_TAG_START ) ) {
      token.tokenType = VariableToken;
      token.content = content.trimmed();
    } else if ( fragment.startsWith( BLOCK_TAG_START ) ) {
      token.tokenType = BlockToken;
      token.content = content.trimmed();
    } else if ( fragment.startsWith( COMMENT_TAG_START ) ) {
      token.tokenType = CommentToken;
      return token;
    }
  }
  return token;
}
