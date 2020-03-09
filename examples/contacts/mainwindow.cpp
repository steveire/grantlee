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

#include "mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

#include "contact.h"
#include <QComboBox>
#include <QDir>
#include <QTimer>

#include "grantlee_paths.h"
#include "grantlee_templates.h"

static const struct {
  const char *name;
  const char *email;
  const char *phone;
  int houseNumber;
  const char *street;
  const char *city;
  const char *nickname;
  const char *salaryCurrency;
  double salary;
  double rating;
  int daysOld;
} contacts[]
    = {{"Alice Murphy", "alice@wonderland.com", "+12345", 123, "Fake St",
        "New York", "dreamer", "EUR", 45000.45, 234.4, 7500},
       {"Bob Murphy", "bob@murphy.com", "+34567", 456, "Fake St", "Paris",
        "bobby", "EUR", 560000.56, 2340.4, 8500},
       {"Carly Carlson", "carly@carlson.com", "+56789", 123, "No St", "London",
        "carlie", "USD", 67000.67, 23400.4, 8000},
       {"David Doyle", "david@doyle.com", "+77894", 456, "No St", "Berlin",
        "doyler", "USD", 78000.78, 234000.4, 7575},
       {"Emma Stevenson", "emma@stevenson.com", "+65432", 89, "Fake St",
        "Syndey", "emms", "EUR", 5400000.54, 2340000.4, 8080},
       {"Frank Fischer", "frank@fischer.com", "+54321", 321, "Fake St", "Tokyo",
        "fish", "EUR", 89000.89, 234000.4, 8754},
       {"Gina Jameson", "gina@jameson.com", "+98765", 123, "Other St",
        "Beijing", "genes", "EUR", 430000, 23400000.4, 9000}};

MainWindow::MainWindow(const QString &templateDir, QWidget *parent,
                       Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  QSplitter *splitter = new QSplitter(this);
  layout->addWidget(splitter);

  m_list = new QListWidget(splitter);
  m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);

  const int numContacts = sizeof(contacts) / sizeof(*contacts);

  QList<Contact *> contactList;
  for (int i = 0; i < numContacts; ++i) {
    Contact *c = new Contact(this);
    c->setName(contacts[i].name);
    c->setEmail(contacts[i].email);
    c->setPhone(contacts[i].phone);
    Address *address = new Address(c);
    address->setHouseNumber(contacts[i].houseNumber);
    address->setStreetName(contacts[i].street);
    address->setCity(contacts[i].city);
    c->setAddress(address);
    c->setNickname(contacts[i].nickname);
    c->setSalaryCurrency(contacts[i].salaryCurrency);
    c->setSalary(contacts[i].salary);
    c->setRating(contacts[i].rating);
    c->setBirthday(QDate::currentDate().addDays(-contacts[i].daysOld));
    contactList.append(c);
  }

  for (int i = 0; i < numContacts; ++i) {
    Contact *friend1 = contactList[(i + 1) % numContacts];
    Contact *friend2 = contactList[(i + 2) % numContacts];
    Contact *friend3 = contactList[(i + 3) % numContacts];
    QList<QObject *> friends = QList<QObject *>() << friend1 << friend2
                                                  << friend3;
    contactList[i]->setFriends(friends);
  }

  Q_FOREACH (Contact *contact, contactList) {
    m_list->addItem(contact);
  }

  connect(m_list->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(render()));

  QWidget *widget = new QWidget(splitter);
  QVBoxLayout *wLayout = new QVBoxLayout();
  widget->setLayout(wLayout);

  m_combo = new QComboBox;
  wLayout->addWidget(m_combo);

  m_templateDir = templateDir;
  QDir themeDir(templateDir);
  m_combo->insertItems(0,
                       themeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));

  connect(m_combo, SIGNAL(currentIndexChanged(int)), SLOT(render()));

  m_webView = new QWebView;
  wLayout->addWidget(m_webView);
  QTimer::singleShot(0, this, SLOT(delayedInit()));
}

void MainWindow::delayedInit()
{
  m_engine = new Grantlee::Engine(this);
  m_engine->addDefaultLibrary("grantlee_i18n");
  m_engine->addDefaultLibrary("grantlee_scriptabletags");

  initLocalizer();

  m_templateLoader = QSharedPointer<Grantlee::FileSystemTemplateLoader>(
      new Grantlee::FileSystemTemplateLoader(m_localizer));
  m_templateLoader->setTemplateDirs(QStringList() << m_templateDir);

  m_engine->addTemplateLoader(m_templateLoader);
}

void MainWindow::initLocalizer() {}

void MainWindow::render() const
{
  const QList<QListWidgetItem *> list = m_list->selectedItems();
  QVariantList contacts;

  Q_FOREACH (QListWidgetItem *item, list)
    contacts << QVariant::fromValue(
        static_cast<QObject *>(static_cast<Contact *>(item)));

  m_templateLoader->setTheme(m_combo->currentText());
  Grantlee::Template t = m_engine->loadByName("main.html");

  if (t->error()) {
    qDebug() << t->errorString();
    return;
  }

  Grantlee::Context c;
  c.setLocalizer(m_localizer);
  c.insert("contacts", contacts);

  const QString result = t->render(&c);

  if (t->error()) {
    qDebug() << t->errorString();
    return;
  }

  m_webView->setHtml(result);
}
