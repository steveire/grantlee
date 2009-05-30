

#ifndef SCRIPTABLE_PARSER_H
#define SCRIPTABLE_PARSER_H

#include <QObject>
#include <QScriptValue>
#include <QVariant>

#include "token.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class ScriptableParser : public QObject
{
  Q_OBJECT
public:
  ScriptableParser(Parser *p, QObject* parent = 0);

  Parser* parser() { return m_p; }

public slots:
  QObjectList parse(const QString &stopAt, QObject *parent);
  QObjectList parse(const QStringList &stopAt, QObject *parent);
  QObjectList parse(QObject *parent);

  void skipPast(const QString &tag);

  Token nextToken();
  bool hasNextToken() const;
  void deleteNextToken();

  void loadLib(const QString &name);

private:
  Parser *m_p;

};

#endif
