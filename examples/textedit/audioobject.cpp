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

#include "audioobject.h"

#include <QPainter>

#include <QDebug>

AudioObject::AudioObject(QObject *parent) : QObject(parent) {}

void AudioObject::drawObject(QPainter *painter, const QRectF &rect,
                             QTextDocument *doc, int posInDocument,
                             const QTextFormat &format)
{
  painter->setBrush(Qt::darkGreen);
  painter->drawRect(rect);

  painter->setBrush(Qt::white);
  QRect circleRect(rect.left() + 2.0, rect.top() + 2.0, rect.width() - 4.0,
                   rect.height() - 4.0);
  painter->drawEllipse(circleRect);

  painter->setBrush(Qt::green);
  const QPointF points[3]
      = {QPointF(circleRect.left() + 7.0, circleRect.top() + 4.0),
         QPointF(circleRect.right() - 4.0,
                 circleRect.top() + 4.0 + ((circleRect.height() - 8.0) / 2.0)),
         QPointF(circleRect.left() + 7.0, circleRect.bottom() - 4.0)};
  painter->drawPolygon(points, 3);
}

QSizeF AudioObject::intrinsicSize(QTextDocument *doc, int posInDocument,
                                  const QTextFormat &format)
{
  return QSizeF(30, 30);
}

AudioFormat::AudioFormat() : QTextCharFormat() { setObjectType(AudioType); }
