/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef FILTEREXPRESSION_H
#define FILTEREXPRESSION_H

#include "variable.h"

#include "grantlee_export.h"

namespace Grantlee
{
class Parser;
class Filter;
}

using namespace Grantlee;

typedef QPair<Filter*, Variable > ArgFilter;

class Token;

namespace Grantlee
{

class GRANTLEE_EXPORT FilterExpression
{
public:
  enum Reversed
  {
    IsNotReversed,
    IsReversed
  };

  FilterExpression();
  FilterExpression(const QString &varString, Grantlee::Parser *parser = 0);

  int error() const;

//   QList<QPair<QString, QString> > filters();
  Variable variable() const;

  QVariant resolve(Context *c) const;

  bool isTrue(Context *c) const;

  QVariantList toList(Context *c) const;

private:
  Variable m_variable;
  QList<ArgFilter> m_filters;
  int m_error;

};

}

#endif
