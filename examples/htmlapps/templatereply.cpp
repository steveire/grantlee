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

#include "templatereply.h"

#include <grantlee/outputstream.h>

#include <QtCore/QDebug>

TemplateReply::TemplateReply(const QNetworkRequest &req,
                             const QNetworkAccessManager::Operation op,
                             Grantlee::Template t, Grantlee::Context c,
                             QObject *parent)
    : QNetworkReply(parent), m_t(t), m_c(c), m_buffer(new QBuffer(this))
{
  setRequest(req);
  setUrl(req.url());
  setOperation(op);
}

bool TemplateReply::atEnd() const { return m_buffer->atEnd(); }

qint64 TemplateReply::bytesAvailable() const
{
  return m_buffer->bytesAvailable() + QNetworkReply::bytesAvailable();
}

bool TemplateReply::canReadLine() const { return m_buffer->canReadLine(); }

void TemplateReply::close() { QNetworkReply::close(); }

bool TemplateReply::isSequential() const { return m_buffer->isSequential(); }

bool TemplateReply::open(QIODevice::OpenMode mode)
{
  m_buffer->open(QIODevice::ReadWrite);
  QString s;
  QTextStream ts(&s);
  Grantlee::OutputStream stream(&ts);
  m_t->render(&stream, &m_c);
  setHeader(QNetworkRequest::ContentTypeHeader, "text/html;charset=utf-8");
  m_buffer->write(s.toUtf8());
  m_buffer->seek(0);
  bool b = QNetworkReply::open(mode);
  QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
  QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
  return b;
}

qint64 TemplateReply::pos() const { return m_buffer->pos(); }

bool TemplateReply::reset() { return m_buffer->reset(); }

bool TemplateReply::seek(qint64 pos) { return m_buffer->seek(pos); }

void TemplateReply::setReadBufferSize(qint64 size)
{
  QNetworkReply::setReadBufferSize(size);
}

qint64 TemplateReply::size() const
{
  return m_buffer->size() + QNetworkReply::size();
}

bool TemplateReply::waitForReadyRead(int msecs)
{
  return m_buffer->waitForReadyRead(msecs);
}

void TemplateReply::abort() { m_buffer->buffer().clear(); }

qint64 TemplateReply::readData(char *data, qint64 maxlen)
{
  return m_buffer->read(data, maxlen);
}
