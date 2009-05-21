/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "yesno.h"


#include <QDebug>

YesNoFilter::YesNoFilter(QObject* parent): Filter(parent)
{

}

QString YesNoFilter::doFilter(const QVariant& input, const QString &argument) const
{
  QString arg = argument;
  QString yes;
  QString no;
  QString maybe;
  if (argument.isNull())
  {
    yes = "yes";
    no = "no";
    maybe = "maybe";
  } else {
    QStringList argList = argument.split(",");
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
  if (!input.toString().isEmpty())
    return yes;
  return no;
}

#include "yesno.moc"