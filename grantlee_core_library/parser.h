/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef PARSER_H
#define PARSER_H

#include <QStringList>

#include "token.h"
#include "node.h"
#include "grantlee.h"

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
  Parser( const QList<Token> &tokenList, QObject *parent );
  ~Parser();

  NodeList parse( const QString &stopAt, QObject *parent );
  NodeList parse( const QStringList &stopAt, QObject *parent );

  NodeList parse( QObject *parent );

  Filter *getFilter( const QString &name ) const;

  void skipPast( const QString &tag );

  Token nextToken();
  bool hasNextToken() const;
  void deleteNextToken();

  void loadLib( const QString &name );


  Error error() const;
  QString errorString() const;

protected:
  void prependToken( const Token &token );

  void setError( Error errorNumber, const QString &message );

private:
  Q_DECLARE_PRIVATE( Parser );
  ParserPrivate *d_ptr;

};

}

#endif
