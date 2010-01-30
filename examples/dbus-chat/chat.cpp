/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

// krazy:excludeall=qclasses

#include "chat.h"
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QScrollBar>

#include <grantlee/context.h>
#include <grantlee/engine.h>

#include "chat_adaptor.h"
#include "chat_interface.h"
#include "chatitem.h"
#include "grantlee_paths.h"

ChatMainWindow::ChatMainWindow()
    : m_nickname(QLatin1String("nickname"))
{
    m_availableColours << "#000066" << "#009966" << "#0000FF" << "#00CCFF" << "#FF0000";
    setupUi(this);
    sendButton->setEnabled(false);

    connect(messageLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(textChangedSlot(QString)));
    connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(sendClickedSlot()));
    connect(actionChangeNickname, SIGNAL(triggered(bool)), this, SLOT(changeNickname()));
    connect(actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQt()));
    connect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(exiting()));

    // add our D-Bus interface and connect to D-Bus
    new ChatAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/", this);

    com::trolltech::chat *iface;
    iface = new com::trolltech::chat(QString(), QString(), QDBusConnection::sessionBus(), this);
    //connect(iface, SIGNAL(message(QString,QString)), this, SLOT(messageSlot(QString,QString)));
    QDBusConnection::sessionBus().connect(QString(), QString(), "com.trolltech.chat", "message", this, SLOT(messageSlot(QString,QString)));
    connect(iface, SIGNAL(action(QString,QString)), this, SLOT(actionSlot(QString,QString)));

    NicknameDialog dialog;
    dialog.cancelButton->setVisible(false);
    dialog.exec();
    m_nickname = dialog.nickname->text().trimmed();

    themeChooser->insertItems(0, QStringList() << "plain" << "colored" << "boxes");
    connect(themeChooser, SIGNAL(activated(const QString &)), SLOT(changeTheme(const QString &)));

    m_engine = Grantlee::Engine::instance();

    Grantlee::FileSystemTemplateLoader::Ptr loader = FileSystemTemplateLoader::Ptr( new Grantlee::FileSystemTemplateLoader() );
    loader->setTemplateDirs(QStringList() << GRANTLEE_TEMPLATE_PATH );
    m_engine->addTemplateLoader(loader);
    m_engine->setPluginDirs(QStringList() << GRANTLEE_PLUGIN_PATH );

    m_engine->addDefaultLibrary( "grantlee_mutabletags_library" );

    changeTheme("plain");

    emit action(m_nickname, QLatin1String("joins the chat"));
}

ChatMainWindow::~ChatMainWindow()
{
    delete m_engine;
}

void ChatMainWindow::addColour(const QString &nickname, const QString &colour)
{
    QVariantHash h;
    QVariantList nameColours;
    QVariantList innerPair;
    innerPair << nickname << colour;
    nameColours.append(QVariant(innerPair));
    m_nameColours.insert( nickname, QVariant(innerPair));

    h.insert("name_colours", nameColours);
    Context c(h);
    QString content = m_template->render(&c);
}

void ChatMainWindow::addItem(ChatItem *item)
{
    QVariantHash h;
    QVariant chatVariant = QVariant::fromValue(static_cast<QObject*>(item));
    m_chatItems.append(chatVariant);
    h.insert("chat_items", QVariantList() << chatVariant);
    Context c(h);
    QString content = m_template->render(&c);
    chatHistory->setHtml(content);
    QScrollBar *sb = chatHistory->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ChatMainWindow::rebuildHistory()
{
    QVariantHash h;
    h.insert("chat_items", m_chatItems);
    h.insert("mynickname", m_nickname);
    h.insert("name_colours", m_nameColours.values());
    Context c(h);
    QString content = m_template->render(&c);
    chatHistory->setHtml(content);
    QScrollBar *sb = chatHistory->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ChatMainWindow::messageSlot(const QString &nickname, const QString &text)
{
    if (!m_nameColours.contains(nickname))
    {
      QString colour = m_availableColours.at(qrand() % m_availableColours.size());
      addColour(nickname, colour);
    }

    ChatItem *message = new ChatItem(nickname, text, ChatItem::Message, this);
    addItem(message);
}

void ChatMainWindow::actionSlot(const QString &nickname, const QString &text)
{
    ChatItem *message = new ChatItem(nickname, text, ChatItem::Action, this);
    addItem(message);
}

void ChatMainWindow::textChangedSlot(const QString &newText)
{
    sendButton->setEnabled(!newText.isEmpty());
}

void ChatMainWindow::sendClickedSlot()
{
    QString content = messageLineEdit->text();
    if (content.startsWith(QLatin1String("/me ")))
    {
      content.remove(0, 4);
      action(m_nickname, content);
    } else {
      //emit message(m_nickname, messageLineEdit->text());
      QDBusMessage msg = QDBusMessage::createSignal("/", "com.trolltech.chat", "message");
      msg << m_nickname << content;
      QDBusConnection::sessionBus().send(msg);
    }
    messageLineEdit->setText(QString());
}

void ChatMainWindow::changeNickname()
{
    NicknameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString old = m_nickname;
        m_nickname = dialog.nickname->text().trimmed();
        emit action(old, QString("is now known as %1").arg(m_nickname));
    }
}

void ChatMainWindow::changeTheme(const QString &themeName)
{
    m_template = m_engine->loadMutableByName(themeName + ".html");
    rebuildHistory();
}

void ChatMainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void ChatMainWindow::exiting()
{
    emit action(m_nickname, QLatin1String("leaves the chat"));
}

NicknameDialog::NicknameDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
        return 1;
    }

    ChatMainWindow chat;
    chat.show();
    return app.exec();
}
