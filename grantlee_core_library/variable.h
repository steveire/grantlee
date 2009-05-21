/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>


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
  explicit Variable();
  explicit Variable(const QString &var);
  Variable(const Variable &other);
  ~Variable();

  Variable &operator=(const Variable &other);

  QString toString() const;
  bool isTrue(Context *c) const;

  QVariant resolve(Context *c) const;

private:
  Q_DECLARE_PRIVATE(Variable);
  VariablePrivate *d_ptr;
};

}

#endif
