/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/


#include "lexer.h"

#include <QRegExp>
#include <QDebug>
#include <QStringList>

#include "grantlee.h"

using namespace Grantlee;

QRegExp tagRe(QString("(%1.*%2|%3.*%4|%5.*%6)")
  .arg(QRegExp::escape(BLOCK_TAG_START))
  .arg(QRegExp::escape(BLOCK_TAG_END))
  .arg(QRegExp::escape(VARIABLE_TAG_START))
  .arg(QRegExp::escape(VARIABLE_TAG_END))
  .arg(QRegExp::escape(COMMENT_TAG_START))
  .arg(QRegExp::escape(COMMENT_TAG_END))
);

enum State
{
  InTag,
  NotInTag
};

class LexerPrivate
{
public:
  LexerPrivate(Lexer *lexer)
    : q_ptr(lexer)
  {

  }
  Q_DECLARE_PUBLIC(Lexer);
  Lexer *q_ptr;

  Token createToken( const QString &fragment, int inTag);

  QString m_templateString;
};

Lexer::Lexer(const QString &templateString)
  : d_ptr(new LexerPrivate(this))
{
  Q_D(Lexer);
  d->m_templateString = templateString;
}

QList<Token> Lexer::tokenize()
{
  Q_D(Lexer);

  tagRe.setMinimal(true); // Can't use '?', eg '.*?', Have to setMinimal instead

  QList<Token> tokenList;

  int pos = 0;
  int oldPosition = 0;
  while ((pos = tagRe.indexIn(d->m_templateString, pos)) != -1) {
    tokenList << d->createToken(d->m_templateString.mid(oldPosition, pos - oldPosition), NotInTag);
    tokenList << d->createToken(tagRe.cap(1), InTag);
    pos += tagRe.matchedLength();
    oldPosition = pos;
  }
  tokenList << d->createToken(d->m_templateString.right(d->m_templateString.size() - oldPosition), NotInTag);

  return tokenList;

}

Token LexerPrivate::createToken( const QString &fragment, int inTag)
{
  Token token;
  if (inTag == NotInTag)
  {
    token.content = fragment;
    token.tokenType = TextToken;
  } else {
    int startTagSize = 2;
    int endTagSize = 2;
    if (fragment.startsWith(VARIABLE_TAG_START))
    {
//       startTagSize = VARIABLE_TAG_START.size();
//       endTagSize = VARIABLE_TAG_END.size();

      token.tokenType = VariableToken;
    } else if (fragment.startsWith(BLOCK_TAG_START))
    {
//       startTagSize = BLOCK_TAG_START.size();
//       endTagSize = BLOCK_TAG_END.size();

      token.tokenType = BlockToken;
    } else if (fragment.startsWith(COMMENT_TAG_START))
    {
      token.tokenType = CommentToken;
      return token;
    }
    // trim this?
    token.content = fragment.mid(startTagSize, fragment.size() - startTagSize - endTagSize );
  }
  return token;
}

