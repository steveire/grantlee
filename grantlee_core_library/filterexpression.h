/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef FILTEREXPRESSION_H
#define FILTEREXPRESSION_H

#include "variable.h"
#include "grantlee.h"

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

  FilterExpression();
  FilterExpression(const QString &varString, Grantlee::Parser *parser);
  FilterExpression(const FilterExpression &other);
  ~FilterExpression();

  FilterExpression &operator=(const FilterExpression &other);

  Error error() const;
  QString errorString() const;

  Variable variable() const;

  QVariant resolve(Context *c) const;

  bool isTrue(Context *c) const;

  QVariantList toList(Context *c) const;

  bool isConstant() const;

protected:
  void setError(Error type, const QString &message) const;

private:
  Q_DECLARE_PRIVATE(FilterExpression);
  FilterExpressionPrivate *d_ptr;
};

}

#endif
