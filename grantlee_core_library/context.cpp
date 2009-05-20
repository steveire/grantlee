/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "context.h"

#include <QDebug>

using namespace Grantlee;

namespace Grantlee
{
class ContextPrivate
{
  ContextPrivate(Context *context, QVariantHash variantHash)
    : q_ptr(context)
  {
    m_variantHashStack.append(variantHash);
  }

  QList<QVariantHash> m_variantHashStack;

  Q_DECLARE_PUBLIC(Context)
  Context *q_ptr;

};

}

Context::Context(QVariantHash variantHash)
  : d_ptr(new ContextPrivate(this, variantHash))
{
}

Context::~Context()
{
  delete d_ptr;
}

QVariant Context::lookup(const QString &str) const
{
  Q_D(const Context);

  // return a variant from the stack.
  QListIterator<QVariantHash> i(d->m_variantHashStack);
  while (i.hasNext())
  {
    QVariantHash h = i.next();
    if (h.contains(str))
      return h[str];
  }

  return QVariant();
}

void Context::push()
{
  Q_D(Context);

  QHash<QString, QVariant> hash;
  d->m_variantHashStack.prepend(hash);
}

void Context::pop()
{
  Q_D(Context);

  d->m_variantHashStack.removeFirst();
}

void Context::insert(const QString &name, QVariant var)
{
  Q_D(Context);

  d->m_variantHashStack[0].insert(name, var);
}

QHash<QString, QVariant> Context::stackHash(int depth)
{
  Q_D(Context);

  return d->m_variantHashStack.value(depth);
}
