/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "context.h"
#include <QStringList>
#include <QMetaEnum>
#include <QThread>

#include <QDebug>

using namespace Grantlee;


Context::Context(QHash<QString, QVariant> variantHash)
{
  m_variantHashStack.append(variantHash);
}

Context::~Context()
{

}

QVariant Context::lookup(const QString &str) const
{
  // return a variant from the stack.

  QListIterator<QHash<QString, QVariant> > i(m_variantHashStack);
  while (i.hasNext())
  {
    QHash<QString, QVariant> h = i.next();
    if (h.contains(str))
      return h[str];
  }

  return QVariant();
}

void Context::push()
{
  QHash<QString, QVariant> hash;
  m_variantHashStack.prepend(hash);
}

void Context::pop()
{
  m_variantHashStack.removeFirst();
}

void Context::insert(const QString &name, QVariant var)
{
  m_variantHashStack[0].insert(name, var);
}

QHash<QString, QVariant> Context::stackHash(int depth)
{
  return m_variantHashStack.value(depth);
}
