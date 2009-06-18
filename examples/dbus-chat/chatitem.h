
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
