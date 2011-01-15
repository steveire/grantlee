/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010,2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_LEXER_P_H
#define GRANTLEE_LEXER_P_H

#include "token.h"
#include "textprocessingmachine_p.h"

template <typename T> class QList;

namespace Grantlee
{

class Lexer
{
public:
  Lexer( const QString &templateString );
  ~Lexer();

  QList<Token> tokenize();

  void markStartSyntax();
  void markEndSyntax();
  void clearMarkers();
  void finalizeToken();

private:
  void reset();

private:
  QString m_templateString;

  QList<Token> m_tokenList;
  int m_upto;
  int m_processedUpto;
  int m_startSyntaxPosition;
  int m_endSyntaxPosition;

};

struct NullLexerAction
{
  static void doAction( Lexer * ) {}
};

template<typename Type, typename Test, typename Action1 = NullLexerAction, typename Action2 = NullLexerAction>
class LexerObject : public Type
{
public:
  LexerObject( Lexer* lexer, State<typename Type::Type>* sourceState = 0 )
    : Type( sourceState ), m_lexer( lexer )
  {

  }

  bool characterTest( QString::const_iterator character )
  {
    return Test::characterTest( character );
  }

  void onTransition()
  {
    return Action1::doAction( m_lexer );
  }

  void onEntry()
  {
    return Action1::doAction( m_lexer );
  }

  void onExit()
  {
    return Action2::doAction( m_lexer );
  }
private:
  Lexer * const m_lexer;
};

struct TokenFinalizer
{
  static void doAction( Lexer *lexer ) {
    lexer->finalizeToken();
  }
};

struct MarkStartSyntax
{
  static void doAction( Lexer *lexer ) {
    lexer->markStartSyntax();
  }
};

struct MarksClearer
{
  static void doAction( Lexer *lexer ) {
    lexer->clearMarkers();
  }
};

struct MarkEndSyntax
{
  static void doAction( Lexer *lexer ) {
    lexer->markEndSyntax();
  }
};

template<char c, typename Action = NullLexerAction>
class CharacterTransition : public LexerObject<State<CharTransitionInterface>::Transition, CharacterTest<c>, Action>
{
public:
  CharacterTransition( Lexer* lexer, State<CharTransitionInterface>* sourceState = 0 )
    : LexerObject<State<CharTransitionInterface>::Transition, CharacterTest<c>, Action>( lexer, sourceState )
  {

  }
};

template<char c, typename Action = NullLexerAction>
class NegateCharacterTransition : public LexerObject<State<CharTransitionInterface>::Transition, Negate<CharacterTest<c> >, Action>
{
public:
  NegateCharacterTransition( Lexer* lexer, State<CharTransitionInterface>* sourceState = 0 )
    : LexerObject<State<CharTransitionInterface>::Transition, Negate<CharacterTest<c> >, Action>( lexer, sourceState )
  {

  }
};

}

#endif
