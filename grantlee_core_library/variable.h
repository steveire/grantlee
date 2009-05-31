/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>

#include "grantlee.h"

namespace Grantlee
{
class Context;
}

namespace Grantlee
{

class VariablePrivate;

class Variable
{
public:
  Variable();
  explicit Variable(const QString &var);
  Variable(const Variable &other);
  ~Variable();

  Variable &operator=(const Variable &other);

  QString toString() const;
  bool isTrue(Context *c) const;

  QVariant resolve(Context *c) const;

  Error error() const;
  QString errorString() const;

protected:
  void setError(Error type, const QString &message);

private:
  Q_DECLARE_PRIVATE(Variable);
  VariablePrivate *d_ptr;
};

}

#endif
