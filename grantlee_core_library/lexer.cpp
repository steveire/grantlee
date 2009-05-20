/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/


#include "lexer_p.h"

#include <QRegExp>
#include <QDebug>
#include <QStringList>

#include "grantlee.h"

using namespace Grantlee;

static QRegExp tagRe(QString("(%1.*%2|%3.*%4|%5.*%6)")
  .arg(QRegExp::escape(BLOCK_TAG_START))
  .arg(QRegExp::escape(BLOCK_TAG_END))
  .arg(QRegExp::escape(VARIABLE_TAG_START))
  .arg(QRegExp::escape(VARIABLE_TAG_END))
  .arg(QRegExp::escape(COMMENT_TAG_START))
  .arg(QRegExp::escape(COMMENT_TAG_END))
);

Lexer::Lexer(const QString &templateString)
  : m_templateString(templateString)
{

}

Lexer::~Lexer()
{
}


QList<Token> Lexer::tokenize() const
{
  tagRe.setMinimal(true); // Can't use '?', eg '.*?', Have to setMinimal instead

  QList<Token> tokenList;

  int pos = 0;
  int oldPosition = 0;
  while ((pos = tagRe.indexIn(m_templateString, pos)) != -1) {
    tokenList << createToken(m_templateString.mid(oldPosition, pos - oldPosition), NotInTag);
    tokenList << createToken(tagRe.cap(1), InTag);
    pos += tagRe.matchedLength();
    oldPosition = pos;
  }
  tokenList << createToken(m_templateString.right(m_templateString.size() - oldPosition), NotInTag);

  return tokenList;

}

Token Lexer::createToken( const QString &fragment, int inTag) const
{
  Token token;
  if (inTag == NotInTag)
  {
    token.content = fragment;
    token.tokenType = TextToken;
  } else {
    int startTagSize = 2;
    int endTagSize = 2;
    QString content = fragment.mid(startTagSize, fragment.size() - startTagSize - endTagSize );
    if (fragment.startsWith(VARIABLE_TAG_START))
    {
      token.tokenType = VariableToken;
      token.content = content.trimmed();
    } else if (fragment.startsWith(BLOCK_TAG_START))
    {
      token.tokenType = BlockToken;
      token.content = content.trimmed();
    } else if (fragment.startsWith(COMMENT_TAG_START))
    {
      token.tokenType = CommentToken;
      return token;
    }
  }
  return token;
}

