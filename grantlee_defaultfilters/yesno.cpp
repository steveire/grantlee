/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "yesno.h"


YesNoFilter::YesNoFilter(QObject* parent): Filter(parent)
{

}

Grantlee::SafeString YesNoFilter::doFilter(const QVariant& input, const Grantlee::SafeString &argument, bool autoescape) const
{
  QString arg = argument;
  QString yes;
  QString no;
  QString maybe;
  if (arg.isNull())
  {
    yes = "yes";
    no = "no";
    maybe = "maybe";
  } else {
    QStringList argList = arg.split(",");
    int numArgs = argList.size();
    if ((numArgs < 2) || (numArgs > 3))
    {
      return input.toString();
    } else if (numArgs == 2)
    {
      yes = argList.at(0);
      no = argList.at(1);
      maybe = argList.at(1);
    } else if (numArgs == 3)
    {
      yes = argList.at(0);
      no = argList.at(1);
      maybe = argList.at(2);
    }
  }
  if (!input.isValid())
    return maybe;
  if (!Util::getSafeString(input).rawString().isEmpty())
    return yes;
  return no;
}

#include "yesno.moc"
