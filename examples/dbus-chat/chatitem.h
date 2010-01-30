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

#ifndef CHATITEM_H
#define CHATITEM_H

#include <QObject>

class ChatItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(QString content READ content)
    Q_PROPERTY(bool is_message READ isMessage)
    Q_PROPERTY(bool is_action READ isAction)
public:
    enum Type {
        Message,
        Action
    };

    ChatItem(const QString &sender, const QString &content, Type type, QObject *parent = 0);

    QString sender() const;
    QString content() const;
    bool isMessage() const;
    bool isAction() const;

private:
    QString m_sender;
    QString m_content;
    Type m_type;
};



#endif
