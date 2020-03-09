/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

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

#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QTextObjectInterface>
#include <QtCore/QObject>

enum CustomType {
  AudioType = QTextFormat::UserObject,

  NumTypes
};

enum CustomProperty {
  AudioProperty = QTextFormat::UserProperty,

  NumProperties
};

/**
  @brief A text object representing an audio snippet
*/
class AudioObject : public QObject, public QTextObjectInterface
{
  Q_OBJECT
  Q_INTERFACES(QTextObjectInterface)
public:
  AudioObject(QObject *parent = 0);

  virtual void drawObject(QPainter *painter, const QRectF &rect,
                          QTextDocument *doc, int posInDocument,
                          const QTextFormat &format);
  virtual QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                               const QTextFormat &format);
};

class AudioFormat : public QTextCharFormat
{
public:
  AudioFormat();
};

#endif // AUDIOOBJECT_H
