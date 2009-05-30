/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "slice.h"


SliceFilter::SliceFilter(QObject* parent): Filter(parent)
{

}

QString SliceFilter::doFilter(const QVariant& input, const QString &argument) const
{
  int splitterIndex = argument.indexOf(":");
  QString inputString = input.toString();
  if (splitterIndex >= 0)
  {
    int left = QVariant(argument.left(splitterIndex)).toInt();
    int right = QVariant(argument.right(splitterIndex)).toInt();
    if (right < 0)
    {
      right = inputString.size() + right;
    }
    return inputString.mid(left, right);
  } else {
    return inputString.at(QVariant(argument).toInt());
  }
}

#include "slice.moc"

