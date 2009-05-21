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

class FilterExpressionPrivate;

class GRANTLEE_EXPORT FilterExpression
{
public:
  enum Reversed
  {
    IsNotReversed,
    IsReversed
  };

  explicit FilterExpression();
  explicit FilterExpression(const QString &varString, Grantlee::Parser *parser = 0);
  FilterExpression(const FilterExpression &other);
  ~FilterExpression();

  FilterExpression &operator=(const FilterExpression &other);

  int error() const;

  Variable variable() const;

  QVariant resolve(Context *c) const;

  bool isTrue(Context *c) const;

  QVariantList toList(Context *c) const;

private:
  Q_DECLARE_PRIVATE(FilterExpression);
  FilterExpressionPrivate *d_ptr;
};

}

#endif
