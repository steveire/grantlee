
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
