/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "default_if_none.h"
#include "util_p.h"

DefaultIfNoneFilter::DefaultIfNoneFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString DefaultIfNoneFilter::doFilter(const QVariant& input, const Grantlee::SafeString &argument, bool autoescape) const
{
  if (!input.isValid())
    return argument;
  return Util::getSafeString(input);
}

#include "default_if_none.moc"
