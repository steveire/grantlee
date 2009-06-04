/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "cut.h"

CutFilter::CutFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString CutFilter::doFilter(const QVariant& input, const Grantlee::SafeString &argument, bool autoescape) const
{
  QString retString = Util::getSafeString(input).rawString();

  retString.remove(argument);

  return retString;
}

#include "cut.moc"

