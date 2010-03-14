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

#include <QRegExp>
#include <QStringList>

#include "grantlee_tags_p.h"

using namespace Grantlee;

// In python regex, '.' matches any character except newline, whereas it QRegExp,
// it matches any character including newline. We match 'not newline' instead.
static QRegExp tagRe( QString( "(%1[^\\n]*%2|%3[^\\n]*%4|%5[^\\n]*%6)" )
                      .arg( QRegExp::escape( BLOCK_TAG_START ) )
                      .arg( QRegExp::escape( BLOCK_TAG_END ) )
                      .arg( QRegExp::escape( VARIABLE_TAG_START ) )
                      .arg( QRegExp::escape( VARIABLE_TAG_END ) )
                      .arg( QRegExp::escape( COMMENT_TAG_START ) )
                      .arg( QRegExp::escape( COMMENT_TAG_END ) )
                    );

Lexer::Lexer( const QString &templateString )
    : m_templateString( templateString )
{

}

Lexer::~Lexer()
{
}


QList<Token> Lexer::tokenize() const
{
  tagRe.setMinimal( true ); // Can't use '?', eg '.*?', Have to setMinimal instead

  QList<Token> tokenList;

  int pos = 0;
  int oldPosition = 0;
  while (( pos = tagRe.indexIn( m_templateString, pos ) ) != -1 ) {
    tokenList << createToken( m_templateString.mid( oldPosition, pos - oldPosition ), NotInTag );
    tokenList << createToken( tagRe.cap( 1 ), InTag );
    pos += tagRe.matchedLength();
    oldPosition = pos;
  }
  tokenList << createToken( m_templateString.right( m_templateString.size() - oldPosition ), NotInTag );

  return tokenList;

}

Token Lexer::createToken( const QString &fragment, int inTag ) const
{
  Token token;
  if ( inTag == NotInTag ) {
    token.content = fragment;
    token.tokenType = TextToken;
  } else {
    int startTagSize = 2;
    int endTagSize = 2;
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

