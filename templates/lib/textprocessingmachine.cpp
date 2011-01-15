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

#include "textprocessingmachine_p.h"

using namespace Grantlee;

void TextProcessingMachine::processCharacter( QChar character )
{
  State<CharTransitionInterface> *s = currentState();
  while ( s ) {
    if ( !doProcessCharacter( character, s ) ) {
      s = s->parent();
    } else {
      return;
    }
  }
}

bool TextProcessingMachine::doProcessCharacter( QChar character, State<CharTransitionInterface> *state )
{
  const QVector<Transition*> transitions = state->transitions();
  QVector<Transition*>::const_iterator it = transitions.constBegin();
  const QVector<Transition*>::const_iterator end = transitions.constEnd();
  for ( ; it != end; ++it ) {
    if ( ( *it )->characterTest( character ) ) {
      executeTransition( state, *it );
      return true;
    }
  }
  return false;
}
