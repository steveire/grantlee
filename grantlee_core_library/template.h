/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QStringList>

#include "node.h"
#include "grantlee_export.h"
#include "grantlee.h"

namespace Grantlee
{
class Context;
}

namespace Grantlee
{

class GRANTLEE_EXPORT Template : public QObject
{
  Q_OBJECT
public:
  Template(QObject *parent = 0  );

  void setContent(const QString &templateString);

  QString render( Context *c );

  NodeList getNodesByType(const char * className);

  NodeList nodeList() const;

  void setNodeList(const NodeList &list);

  Error error();
  QString errorString();

protected:
  void setError(Error type, const QString &message);

private:
  void parse();
  NodeList compileString(const QString &str);

  Error m_error;
  QString m_errorString;
  QStringList m_pluginDirs;
  NodeList m_nodeList;
};


}

#endif
