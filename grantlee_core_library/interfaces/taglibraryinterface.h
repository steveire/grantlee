/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TAGLIBRARYINTERFACE_H
#define TAGLIBRARYINTERFACE_H

namespace Grantlee
{
class AbstractNodeFactory;
}

class QString;

namespace Grantlee
{

class TagLibraryInterface
{
public:
  virtual ~TagLibraryInterface() {}

  virtual QHash<QString, AbstractNodeFactory*> nodeFactories() = 0;

};

}

Q_DECLARE_INTERFACE(Grantlee::TagLibraryInterface, "org.kde.grantlee.TagLibraryInterface/1.0")

#endif
