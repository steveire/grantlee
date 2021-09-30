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

#ifndef TEMPLATEREPLY_H
#define TEMPLATEREPLY_H

#include <QtCore/QBuffer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <grantlee/context.h>
#include <grantlee/template.h>

class TemplateReply : public QNetworkReply
{
  Q_OBJECT
public:
  TemplateReply(const QNetworkRequest &req,
                const QNetworkAccessManager::Operation op, Grantlee::Template t,
                Grantlee::Context c, QObject *parent = 0);

  virtual void abort();
  virtual qint64 readData(char *data, qint64 maxlen);
  virtual bool atEnd() const;
  virtual qint64 bytesAvailable() const;
  virtual bool canReadLine() const;
  virtual void close();
  virtual bool isSequential() const;
  virtual qint64 pos() const;
  virtual bool reset();
  virtual bool seek(qint64 pos);
  virtual void setReadBufferSize(qint64 size);
  virtual qint64 size() const;
  virtual bool waitForReadyRead(int msecs);
  virtual bool open(OpenMode mode);

private:
  Grantlee::Template m_t;
  Grantlee::Context m_c;
  QBuffer *m_buffer;
};

#endif
