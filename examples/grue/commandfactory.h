
#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <QStringList>

class QIODevice;

class CommandFactory
{
public:
  virtual QIODevice* create() = 0;
  virtual QStringList completions() const = 0;
};

#endif
