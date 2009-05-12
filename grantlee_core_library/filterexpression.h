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
}

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

  int error();

//   QList<QPair<QString, QString> > filters();
  Variable variable();

  QVariant resolve(Context *c);

  bool isTrue(Context *c);

  QVariantList toList(Context *c);

private:
  Variable m_variable;
  int m_error;

};

}

#endif
