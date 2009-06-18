/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

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

  void setTokens( const QList<Token> &tokenList );

  NodeList parse( const QStringList &stopAt = QStringList() );
  NodeList parse( const QString &stopAt );

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

