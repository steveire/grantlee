/*
  This file is part of the Grantlee template system.

  Copyright (c) 2016 Stephen Kelly <steveire@gmail.com>

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

#ifndef IF_P_H
#define IF_P_H

#include <QtCore/QPair>
#include <QtCore/QStringList>
#include <QtCore/QVector>

#include "../lib/exception.h"
#include "filterexpression.h"
#include "node.h"
#include "util.h"

namespace Grantlee
{
class Parser;
}

class IfToken;

class IfParser
{
public:
  IfParser(Grantlee::Parser *parser, const QStringList &args);

  QSharedPointer<IfToken> parse();

  QSharedPointer<IfToken> expression(int rbp = 0);

private:
  QSharedPointer<IfToken> createNode(const QString &content) const;

  QSharedPointer<IfToken> consumeToken();

private:
  Grantlee::Parser *mParser;
  QVector<QSharedPointer<IfToken>> mParseNodes;
  int mPos = 0;
  QSharedPointer<IfToken> mCurrentToken;
};

class IfToken
{
public:
  enum OpCode { Invalid, Literal, OrCode, AndCode, NotCode, Sentinal };

  static QSharedPointer<IfToken> makeSentinal()
  {
    return QSharedPointer<IfToken>::create(0, QString(), Sentinal);
  }

  using ArgsType
      = std::pair<QSharedPointer<IfToken>, QSharedPointer<IfToken>>;

  IfToken(int lbp, const QString &tokenName, OpCode opCode) : mArgs()
  {
    mLbp = lbp;
    mTokenName = tokenName;
    mOpCode = opCode;
  }

  IfToken(const Grantlee::FilterExpression &fe) : mFe(fe)
  {
    mLbp = 0;
    mTokenName = QStringLiteral("literal");
    mOpCode = Literal;
  }

  void nud(IfParser *parser);
  void led(QSharedPointer<IfToken> left, IfParser *parser);

  QVariant evaluate(Grantlee::Context *c) const;

  int lbp() const { return mLbp; }

  int mLbp;
  QString mTokenName;

  Grantlee::FilterExpression mFe;
  ArgsType mArgs;

  OpCode mOpCode;

  QString tokenName() const { return mTokenName; }
};

void IfToken::nud(IfParser *parser)
{
  switch (mOpCode) {
  default:
    break;
  case IfToken::Literal:
    return;
  case IfToken::NotCode:
    mArgs.first = parser->expression(mLbp);
    mArgs.second.clear();
    return;
  }

  throw Grantlee::Exception(
      Grantlee::TagSyntaxError,
      QStringLiteral("Not expecting '%1' in this position in if tag.")
          .arg(mTokenName));
}

void IfToken::led(QSharedPointer<IfToken> left, IfParser *parser)
{
  switch (mOpCode) {
  default:
    break;
  case IfToken::OrCode:
  case IfToken::AndCode:
    mArgs.first = left;
    mArgs.second = parser->expression(mLbp);
    return;
  }

  throw Grantlee::Exception(
      Grantlee::TagSyntaxError,
      QStringLiteral("Not expecting '%1' as infix operator in if tag.")
          .arg(mTokenName));
}

IfParser::IfParser(Grantlee::Parser *parser, const QStringList &args)
    : mParser(parser)
{
  mParseNodes.reserve(args.size());
  if (args.size() > 1) {
    auto it = args.begin() + 1, end = args.end() - 1;
    for (; it != end; ++it) {
      if (*it == QLatin1String("not")
          && *std::next(it) == QLatin1String("in")) {
        mParseNodes.push_back(createNode(QStringLiteral("not in")));
        ++it;
        if (it == end) {
          break;
        }
      } else {
        mParseNodes.push_back(createNode(*it));
      }
    }
    mParseNodes.push_back(createNode(*it));
  }
  mPos = 0;
  mCurrentToken = consumeToken();
}

QSharedPointer<IfToken> IfParser::consumeToken()
{
  if (mPos >= mParseNodes.size()) {
    return IfToken::makeSentinal();
  }
  auto t = mParseNodes[mPos];
  ++mPos;
  return t;
}

QSharedPointer<IfToken> IfParser::expression(int rbp)
{
  auto t = mCurrentToken;
  mCurrentToken = consumeToken();
  t->nud(this);
  auto left = t;
  while (rbp < mCurrentToken->lbp()) {
    t = mCurrentToken;
    mCurrentToken = consumeToken();
    t->led(left, this);
    left = t;
  }
  return left;
}

QSharedPointer<IfToken> IfParser::parse()
{
  auto r = expression();

  if (mCurrentToken->mOpCode != IfToken::Sentinal) {
    throw Grantlee::Exception(
        Grantlee::TagSyntaxError,
        QStringLiteral("Unused '%1' at end of if expression.")
            .arg(mCurrentToken->tokenName()));
  }

  return r;
}

QSharedPointer<IfToken> IfParser::createNode(const QString &content) const
{
  if (content == QLatin1String("or")) {
    return QSharedPointer<IfToken>::create(6, content, IfToken::OrCode);
  }
  if (content == QLatin1String("and")) {
    return QSharedPointer<IfToken>::create(7, content, IfToken::AndCode);
  }
  if (content == QStringLiteral("not")) {
    return QSharedPointer<IfToken>::create(8, content, IfToken::NotCode);
  }
  return QSharedPointer<IfToken>::create(
      Grantlee::FilterExpression(content, mParser));
}

QVariant IfToken::evaluate(Context *c) const
{
  try {
    switch (mOpCode) {
    case Literal:
      return mFe.resolve(c);
    case OrCode:
      return Grantlee::variantIsTrue(mArgs.first->evaluate(c))
             || Grantlee::variantIsTrue(mArgs.second->evaluate(c));
    case AndCode:
      return Grantlee::variantIsTrue(mArgs.first->evaluate(c))
             && Grantlee::variantIsTrue(mArgs.second->evaluate(c));
    case NotCode:
      return !Grantlee::variantIsTrue(mArgs.first->evaluate(c));
    default:
      Q_ASSERT(!"Invalid OpCode");
      return QVariant();
    }
  } catch (Grantlee::Exception) {
    return false;
  }
}

#endif
