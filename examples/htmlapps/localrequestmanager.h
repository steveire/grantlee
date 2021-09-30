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

#ifndef LOCALREQUESTMANAGER_H
#define LOCALREQUESTMANAGER_H

#include <QtNetwork/QNetworkAccessManager>

#include <grantlee/engine.h>

class LocalRequestManager : public QNetworkAccessManager
{
  Q_OBJECT
public:
  LocalRequestManager(Grantlee::Engine *engine, QObject *parent = 0);

  virtual QNetworkReply *createRequest(Operation op,
                                       const QNetworkRequest &request,
                                       QIODevice *outgoingData = 0);

private:
  Grantlee::Engine *m_engine;
};

#endif
