
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "script.h"
#include "commandtokenizer.h"

class CommandParser
{
public:
  CommandParser(const QList<Token> &tokens);
  Script parse();
};

#endif
