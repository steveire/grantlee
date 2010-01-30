/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "chatitem.h"

ChatItem::ChatItem(const QString &sender, const QString &content, Type type, QObject *parent)
    :  QObject(parent), m_sender(sender), m_content(content), m_type(type)
{

}

QString ChatItem::sender() const
{
  return m_sender;
}

QString ChatItem::content() const
{
  return m_content;
}

bool ChatItem::isMessage() const
{
  return m_type == Message;
}

bool ChatItem::isAction() const
{
  return m_type == Action;
}

