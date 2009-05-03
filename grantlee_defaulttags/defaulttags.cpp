/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "defaulttags.h"

#include <QStringList>
#include <QtPlugin>

#include "parser.h"

#include <QDebug>

Q_EXPORT_PLUGIN2(grantlee_defaulttags_library, DefaultTagLibrary);

LoadNode::LoadNode()
{

}

QString LoadNode::render(Context *c)
{
  Q_UNUSED(c);
  // TODO: figure out if this should be a null or an empty QString.
  return QString();
  //   return QString('');
}


#include "defaulttags.moc"
