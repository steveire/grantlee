/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "removetags.h"
#include <QRegExp>


RemoveTagsFilter::RemoveTagsFilter(QObject* parent): Filter(parent)
{

}


QString RemoveTagsFilter::doFilter(const QVariant& input, const QString &argument) const
{
  QStringList tags = argument.split(" ");
  QString tagRe = QString("(%1)").arg(tags.join("|"));
  QRegExp startTag( QString("<%1(/?>|(\\s+[^>]*>))").arg(tagRe) );
  QRegExp endTag( QString("</%1>").arg(tagRe) );

  QString value = input.toString();
  value.replace(startTag, "");
  value.replace(endTag, "");
  return value;
}

#include "removetags.moc"
