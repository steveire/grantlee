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
class AbstractNodeFactory;

class TagLibraryInterface;
class Filter;
}

class ParserPrivate;

namespace Grantlee
{

class GRANTLEE_EXPORT Parser : public QObject
{
  Q_OBJECT
public:
  Parser( QList<Token> tokenList, QStringList pluginDirs, QObject *parent );
  ~Parser();

  NodeList parse(QStringList stopAt, QObject *parent);

  NodeList parse(QObject *parent);

  Filter *getFilter(const QString &name);

  void skipPast(const QString &tag);

  Token nextToken();
  bool hasNextToken();

  void loadLib(const QString &name);

  void emitError(int errorNumber, const QString &message);

protected:
  void addTag(QObject *);
  void getBuiltInLibrary();
  void getDefaultLibrary();

  void prependToken(Token token);

signals:
  void error(int type, const QString &message);

private:
  Q_DECLARE_PRIVATE(Parser);
  ParserPrivate *d_ptr;

};

}

#endif
