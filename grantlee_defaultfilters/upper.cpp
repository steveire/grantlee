/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "upper.h"

UpperFilter::UpperFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString UpperFilter::doFilter(const QVariant& input, const Grantlee::SafeString &argument, bool autoescape ) const
{
  return Util::getSafeString(input).rawString().toUpper();
}

#include "upper.moc"