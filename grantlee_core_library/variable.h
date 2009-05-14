/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>

#include "context.h"

namespace Grantlee
{

class Variable
{
public:
  Variable();
  Variable(const QString &var);

  QString toString() const { return m_varString; }
  bool isTrue(Context *c) const;

  QVariant resolve(Context *c) const;

protected:
  QVariant resolvePart( QVariant, const QString &s ) const;

private:
  QString m_varString;
  QVariant m_literal;
  QStringList m_lookups;
  bool m_translate;

};

}

#endif
