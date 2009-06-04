/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "join.h"
#include <util_p.h>

JoinFilter::JoinFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString JoinFilter::doFilter(const QVariant& input, const Grantlee::SafeString &argument, bool autoescape) const
{
  QVariantList varList = Util::variantToList(input);
  QListIterator<QVariant> i(varList);
  QString ret;
  while (i.hasNext())
  {
    QVariant var = i.next();
    Grantlee::SafeString s = Util::getSafeString(var);
    if (autoescape)
      s = Util::conditionalEscape(s);

    ret.append(s.rawString());
    if (i.hasNext())
      ret.append(argument);
  }
  return Util::markSafe(ret);
}

#include "join.moc"

