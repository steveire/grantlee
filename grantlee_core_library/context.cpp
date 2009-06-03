/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "context.h"

using namespace Grantlee;

namespace Grantlee
{
class ContextPrivate
{
  ContextPrivate(Context *context, const QVariantHash &variantHash)
    : q_ptr(context), m_autoescape(true)
  {
    m_variantHashStack.append(variantHash);
  }

  QList<QVariantHash> m_variantHashStack;
  bool m_autoescape;

  Q_DECLARE_PUBLIC(Context)
  Context *q_ptr;

};

}

Context::Context(const QVariantHash &variantHash)
  : d_ptr(new ContextPrivate(this, variantHash))
{
}

Context::~Context()
{
  delete d_ptr;
}

bool Context::autoescape() const
{
  Q_D(const Context);
  return d->m_autoescape;
}

void Context::setAutoescape(bool autoescape)
{
  Q_D(Context);
  d->m_autoescape = autoescape;
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

void Context::insert(const QString &name, const QVariant &variant)
{
  Q_D(Context);

  d->m_variantHashStack[0].insert(name, variant);
}

QHash<QString, QVariant> Context::stackHash(int depth) const
{
  Q_D(const Context);

  return d->m_variantHashStack.value(depth);
}
