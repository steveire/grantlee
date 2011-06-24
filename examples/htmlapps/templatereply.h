
#ifndef TEMPLATEREPLY_H
#define TEMPLATEREPLY_H

#include <QtCore/QBuffer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <grantlee/template.h>
#include <grantlee/context.h>

class TemplateReply : public QNetworkReply
{
  Q_OBJECT
public:
  TemplateReply(const QNetworkRequest &req, const QNetworkAccessManager::Operation op, Grantlee::Template t, Grantlee::Context c, QObject *parent = 0);

  virtual void abort();
  virtual qint64 readData(char* data, qint64 maxlen);
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
