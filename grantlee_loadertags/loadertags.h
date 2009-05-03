/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef LOADERTAGS_H
#define LOADERTAGS_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "include.h"
#include "extends.h"
#include "block.h"

class Parser;

class LoaderTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( TagLibraryInterface )
public:
  LoaderTagLibrary()
  {
    m_nodeFactories.insert("include", new IncludeNodeFactory());
    m_nodeFactories.insert("extends", new ExtendsNodeFactory());
    m_nodeFactories.insert("block", new BlockNodeFactory());
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories()
  {
    return m_nodeFactories;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;

};


#endif
