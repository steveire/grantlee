
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

