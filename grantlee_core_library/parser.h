/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include <QSet>
#include <QStringList>

// defines Token;
#include "lexer.h"

#include "node.h"

namespace Grantlee
{
class AbstractNodeFactory;

class TagLibraryInterface;
}

class ParserPrivate;


namespace Grantlee
{

class GRANTLEE_EXPORT Parser : public QObject
{
  Q_OBJECT
public:
  Parser( QList<Token> tokenList, QStringList pluginDirs );

  NodeList parse(QStringList stopAt = QStringList());

  Token nextToken();
  bool hasNextToken();

  enum ErrorType
  {
    UnknownError,
    TagSyntaxError,
    EmptyVariableError,
    EmptyBlockTagError,
    InvalidBlockTagError,
    UnclosedBlockTagError,
    CompileFunctionError
  };

protected:
  void addTag(QObject *);
  void getBuiltInLibrary();
  void getDefaultLibrary();

  void prependToken(Token token);

  void loadLib(const QString &name);

signals:
  void error(int type, const QString &message);

private:
  Q_DECLARE_PRIVATE(Parser);
  ParserPrivate *d_ptr;

};

}

#endif
