
#ifndef COMMANDTOKENIZER_H
#define COMMANDTOKENIZER_H

#include <QtCore/QList>
#include <QtCore/QString>

struct Token
{
  enum Type {
    Command,
    Argument,
    Pipe,
    Redirection,
    ProcessRedirectionStart,
    ProcessRedirectionEnd,
    SubProcessStart,
    SubProcessEnd
  };
  int type;
  QString command;
};

class CommandTokenizer
{
public:
  CommandTokenizer(const QString &command);

  QList<Token> tokenize();
};

#endif
