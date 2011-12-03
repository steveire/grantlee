/*
  This file is part of the Grantlee template system.

  Copyright (c) 2011 Stephen Kelly <steveire@gmail.com>

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

#ifndef GRANTLEE_TEXTPROCESSINGMACHINE_P_H
#define GRANTLEE_TEXTPROCESSINGMACHINE_P_H

#include "statemachine_p.h"

namespace Grantlee
{

struct CharTransitionInterface
{
  virtual bool characterTest( QString::const_iterator )
  {
    return false;
  }

  virtual void onTransition() {}

protected:
  virtual ~CharTransitionInterface() {}
};

class TextProcessingMachine : public StateMachine<CharTransitionInterface>
{
public:
  void processCharacter( QString::const_iterator character );
protected:
  bool doProcessCharacter( QString::const_iterator character, State<CharTransitionInterface> *state );
};

typedef CharTransitionInterface NullTest;

template<typename T, typename U>
struct AndTest
{
  static bool characterTest( QString::const_iterator ch )
  {
    return T::characterTest( ch ) && U::characterTest( ch );
  }
};

template<typename T, typename U>
struct OrTest
{
  static bool characterTest( QString::const_iterator ch )
  {
    return T::characterTest( ch ) || U::characterTest( ch );
  }
};

template<typename T>
struct Negate
{
  static bool characterTest( QString::const_iterator ch )
  {
    return !T::characterTest( ch );
  }
};

struct IsSpace
{
  static bool characterTest( QString::const_iterator ch )
  {
    return ch->isSpace();
  }
};

template<char c>
struct CharacterTest
{
  static bool characterTest( QString::const_iterator ch )
  {
    return *ch == QLatin1Char( c );
  }
};

}

#endif
