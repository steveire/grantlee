/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>

#include "context.h"

class Variable
{
public:
  Variable();
  Variable(const QString &var);

  QString toString() { return m_varString; }

  QVariant resolve(Context *c);

protected:
  QVariant resolvePart( QVariant, const QString &s );

private:
  QString m_varString;

};

#endif
