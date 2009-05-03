/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TAGLIBRARYINTERFACE_H
#define TAGLIBRARYINTERFACE_H

class AbstractNodeFactory;
class QString;

class TagLibraryInterface
{
public:
  virtual ~TagLibraryInterface() {}

  virtual QHash<QString, AbstractNodeFactory*> nodeFactories() = 0;

};
Q_DECLARE_INTERFACE(TagLibraryInterface, "org.kde.grantlee.TagLibraryInterface/1.0")

#endif
