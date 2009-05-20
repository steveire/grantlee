/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "cut.h"

CutFilter::CutFilter(QObject* parent): Filter(parent)
{

}

QString CutFilter::doFilter(const QVariant& input, const QString &argument) const
{
  QString retString = input.toString();

  retString.remove(argument);

  return retString;
}

#include "cut.moc"

