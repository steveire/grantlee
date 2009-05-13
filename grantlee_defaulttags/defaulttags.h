/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef DEFAULTTAGS_H
#define DEFAULTTAGS_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"

#include "if.h"
#include "for.h"
#include "with.h"
#include "comment.h"
#include "ifequal.h"
#include "firstof.h"
#include "cycle.h"
#include "templatetag.h"
#include "widthratio.h"
#include "filtertag.h"
#include "load.h"
#include "now.h"
#include "ssi.h"

#include "grantlee_export.h"

namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class IfChangedNodeFactory : public AbstractNodeFactory
{
  public:
    IfChangedNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p);

};

class DefaultTagLibrary : public QObject, public TagLibraryInterface
{
  Q_OBJECT
  Q_INTERFACES( Grantlee::TagLibraryInterface )
public:
  DefaultTagLibrary()
  {
    m_nodeFactories.insert("if", new IfNodeFactory());
    m_nodeFactories.insert("for", new ForNodeFactory());
    m_nodeFactories.insert("with", new WithNodeFactory());
    m_nodeFactories.insert("comment", new CommentNodeFactory());
    m_nodeFactories.insert("ifequal", new IfEqualNodeFactory());
    m_nodeFactories.insert("ifnotequal", new IfNotEqualNodeFactory());
    m_nodeFactories.insert("firstof", new FirstOfNodeFactory());
    m_nodeFactories.insert("cycle", new CycleNodeFactory());
    m_nodeFactories.insert("templatetag", new TemplateTagNodeFactory());
    m_nodeFactories.insert("widthratio", new WidthRatioNodeFactory());
    m_nodeFactories.insert("filter", new FilterNodeFactory());
    m_nodeFactories.insert("load", new LoadNodeFactory());
    m_nodeFactories.insert("now", new NowNodeFactory());
    m_nodeFactories.insert("ssi", new SsiNodeFactory());
  }

  QHash<QString, AbstractNodeFactory*> nodeFactories()
  {
    return m_nodeFactories;
  }

private:
  QHash<QString, AbstractNodeFactory*> m_nodeFactories;

};

#endif
