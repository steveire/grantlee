/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QVariantHash>

#include "grantlee_export.h"

namespace Grantlee
{

class ContextPrivate;

class GRANTLEE_EXPORT Context
{

public:

  /**
  Sets every key in the hash as a property name with the variant as the value.
  */
  Context(const QVariantHash &hash);

  ~Context();

  /**
  Returns a QVariant, even in the case of it containing a list which can be iterated over.
  Have to think about whether QMap should work too, or would its keys already be resolved.
  */
  QVariant lookup(const QString &str) const;
  void insert(const QString &name, const QVariant &variant);
  void push();
  void pop();

  QVariantHash stackHash(int depth) const;

private:
  Q_DECLARE_PRIVATE(Context)
  ContextPrivate *d_ptr;
};

}

#endif
