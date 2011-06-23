
#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>

class Script
{
public:

  QString command() const;
  void execute();

  int error() const;
  QString errorString() const;
  QString result();
};

#endif
