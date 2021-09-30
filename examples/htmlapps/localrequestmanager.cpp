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

#define QT_DISABLE_DEPRECATED_BEFORE 0

#include "localrequestmanager.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include "templatereply.h"

LocalRequestManager::LocalRequestManager(Grantlee::Engine *engine,
                                         QObject *parent)
    : QNetworkAccessManager(parent), m_engine(engine)
{
}

QNetworkReply *
LocalRequestManager::createRequest(QNetworkAccessManager::Operation op,
                                   const QNetworkRequest &request,
                                   QIODevice *outgoingData)
{
  QUrl requestUrl = request.url();
  if (requestUrl.scheme() != "template") {
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
  }

  Grantlee::Template t = m_engine->loadByName(requestUrl.path());

  if (t->error()) {
    qDebug() << t->errorString();
  }

  QList<QPair<QString, QString>> query = requestUrl.queryItems();
  if (outgoingData) {
    QUrl postData;
    postData.setEncodedQuery(outgoingData->readAll());
  }

  Grantlee::Context c;

  TemplateReply *reply = new TemplateReply(request, op, t, c);

  if (t->error()) {
    qDebug() << t->errorString();
  }
  reply->open(QIODevice::ReadOnly);

  return reply;
}
