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

#include "audiotextedit.h"

#include <QFileDialog>
#include <QDebug>

#include "phonon/mediaobject.h"

#include "audioobject.h"

AudioTextEdit::AudioTextEdit(QWidget* parent)
  : QTextEdit(parent)
{
  viewport()->installEventFilter( this );
}

bool AudioTextEdit::eventFilter(QObject* obj, QEvent* event)
{
  if ( event->type() != QEvent::MouseButtonPress )
    return QObject::eventFilter(obj, event);

  QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );
  QTextCursor textCursor = cursorForPosition( mouseEvent->pos() );

  QTextCharFormat currentFormat = textCursor.charFormat();
  textCursor.movePosition(QTextCursor::Right);
  QTextCharFormat previousFormat = textCursor.charFormat();

  QTextCharFormat audioFormat;
  if ( currentFormat.objectType() != AudioType )
  {
    if ( previousFormat.objectType() != AudioType )
      return QObject::eventFilter(obj, event);
    else
      audioFormat = previousFormat;
  } else
    audioFormat = currentFormat;

  Phonon::MediaObject *music =
         Phonon::createPlayer(Phonon::MusicCategory,
                              Phonon::MediaSource(audioFormat.property( AudioProperty ).toString()));
  music->play();

  return true;
}

#include "audiotextedit.moc"
