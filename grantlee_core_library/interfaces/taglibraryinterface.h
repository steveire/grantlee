/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef TAGLIBRARYINTERFACE_H
#define TAGLIBRARYINTERFACE_H

#include <QHash>

namespace Grantlee
{
class AbstractNodeFactory;
class Filter;
}

// class QString;

namespace Grantlee
{

class TagLibraryInterface
{
public:
  virtual ~TagLibraryInterface() {}

  virtual QHash<QString, AbstractNodeFactory*> nodeFactories() {
    QHash<QString, AbstractNodeFactory*> h;
    return h;
  };

  virtual QHash<QString, Filter*> filters() {
    QHash<QString, Filter*> h;
    return h;
  };
};

}

Q_DECLARE_INTERFACE(Grantlee::TagLibraryInterface, "org.kde.grantlee.TagLibraryInterface/1.0")

#endif
