
#define QT_DISABLE_DEPRECATED_BEFORE 0

#include "localrequestmanager.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include "templatereply.h"

LocalRequestManager::LocalRequestManager(Grantlee::Engine *engine, QObject* parent)
  : QNetworkAccessManager(parent), m_engine( engine )
{

}

QNetworkReply* LocalRequestManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
{
  QUrl requestUrl = request.url();
  if (requestUrl.scheme() != "template") {
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
  }

  Grantlee::Template t = m_engine->loadByName(requestUrl.path());

  if (t->error()) {
    qDebug() << t->errorString();
  }

  QList<QPair<QString, QString> > query = requestUrl.queryItems();
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
