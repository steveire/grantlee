/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef PARSER_H
#define PARSER_H

#include <QStringList>

#include "token.h"
#include "node.h"

namespace Grantlee
{
class Filter;
}

namespace Grantlee
{

class ParserPrivate;

class GRANTLEE_EXPORT Parser : public QObject
{
  Q_OBJECT
public:
  Parser( const QList<Token> &tokenList, const QStringList &pluginDirs, QObject *parent );
  ~Parser();

  NodeList parse(const QStringList &stopAt, QObject *parent);

  NodeList parse(QObject *parent);

  Filter *getFilter(const QString &name) const;

  void skipPast(const QString &tag);

  Token nextToken();
  bool hasNextToken() const;
  void deleteNextToken();

  void loadLib(const QString &name);

  void emitError(int errorNumber, const QString &message);

protected:
  void prependToken(const Token &token);

protected slots:
  void errorSlot(int type, const QString &message);

signals:
  void error(int type, const QString &message);

private:
  Q_DECLARE_PRIVATE(Parser);
  ParserPrivate *d_ptr;

};

}

#endif
