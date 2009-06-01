/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "extends.h"

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"
#include "templateloader.h"
#include "grantlee.h"

#include <QDebug>

#include <QMutableListIterator>

typedef QMutableListIterator<Node*> MutableNodeListIterator;

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory(QObject *parent)
  : AbstractNodeFactory(parent)
{

}

Node* ExtendsNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  QString parentName = expr.at(1);
  FilterExpression fe;
  int size = parentName.size();

  if ( ( parentName.startsWith( "\"" ) && parentName.endsWith( "\"" ) )
    || ( parentName.startsWith( "'" ) && parentName.endsWith( "'" ) ) )
  {
    parentName = parentName.mid(1, size -2);
  } else {
    fe = FilterExpression(parentName, p);
    parentName = QString();
  }

  NodeList nodeList = p->parse(parent);

  if (nodeList.getNodesByType(ExtendsNode::staticMetaObject.className()).size() > 0)
  {
    setError(TagSyntaxError, "Extends tag may only appear once in a template.");
    return 0;
  }

  return new ExtendsNode(nodeList, parentName, fe, parent);
}

ExtendsNode::ExtendsNode(NodeList list, const QString &name, FilterExpression fe, QObject *parent)
  : Node(parent),
    m_filterExpression(fe),
    m_name(name)
{
  m_list = list;

}

NodeList ExtendsNode::getNodesByType(const char* className)
{
  return m_list.getNodesByType(className);
}


Template *ExtendsNode::getParent(Context *c)
{
  QString parentName;
  if (m_name.isEmpty())
  {
    QVariant parentVar = m_filterExpression.resolve(c);
    if (parentVar.userType() == QMetaType::QObjectStar)
    {
      QObject *parentObject = parentVar.value<QObject*>();
      Template *parentTemplate = qobject_cast<Template *>(parentObject);
      if (parentTemplate)
        return parentTemplate;
    } else {
      parentName = parentVar.toString();
    }
  } else {
    parentName = m_name;
  }

  TemplateLoader *loader = TemplateLoader::instance();

  Template* t = loader->loadByName(parentName, this);

  return t;
}

QString ExtendsNode::render(Context *c)
{
  Template *parent = getParent(c);

  if (!parent)
  {
    setError(TagSyntaxError, "TODO: Fix message");
    return QString();
  }

  NodeList nodeList = parent->getNodesByType(BlockNode::staticMetaObject.className());
  QHash<QString, BlockNode *> parentBlocks;
  MutableNodeListIterator i(nodeList);
  int idx = 0;
  while (i.hasNext())
  {
    Node* n = i.next();
    BlockNode *bn = qobject_cast<BlockNode*>(n);
    if (bn)
    {
      parentBlocks.insert(bn->name(), bn);
    }
    idx++;
  }

  NodeList l = m_list.getNodesByType(BlockNode::staticMetaObject.className());
  MutableNodeListIterator j(l);

  while (j.hasNext())
  {
    Node* n = j.next();
    BlockNode *bn = qobject_cast<BlockNode*>(n);
    if (bn)
    {
      if (parentBlocks.contains(bn->name()))
      {
        BlockNode *pbn = parentBlocks.value(bn->name());
        pbn->setNodeParent(bn->nodeParent());
        pbn->addParent(pbn->nodeList());
        pbn->setNodeList(bn->nodeList());
      } else {
        foreach(Node *node, parent->nodeList())
        {
          TextNode *tn = dynamic_cast<TextNode*>(node);
          if (!tn)
          {
            ExtendsNode *en = dynamic_cast<ExtendsNode*>(node);
            if (en)
            {
              en->appendNode(bn);
            }
            break;
          }
        }
      }
    }
  }

  return parent->render(c);
}

void ExtendsNode::appendNode(Node *node)
{
  m_list.append(node);
}
