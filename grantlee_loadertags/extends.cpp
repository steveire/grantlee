/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "extends.h"

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"

#include <QDebug>

#include <QMutableListIterator>

typedef QMutableListIterator<Node*> MutableNodeListIterator;

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory()
{

}

Node* ExtendsNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  QString parentName = expr.at(1);
  int size = parentName.size();

  if (parentName.at(0) == QChar('\"') && parentName.at(size-1) == QChar('\"'))
  {
    parentName = parentName.mid(1, size -2);
  }

  NodeList nodeList = p->parse();

  return new ExtendsNode(nodeList, parentName);
}

ExtendsNode::ExtendsNode(NodeList list, const QString &name)
{
  m_list = list;
  m_filterExpression = FilterExpression(name);
}

QString ExtendsNode::render(Context *c)
{
  QString filename = m_filterExpression.variable().toString();

  TemplateLoader *loader = TemplateLoader::instance();

  Template t = loader->loadByName(filename);

  NodeList nodeList = t.nodeList();
  QHash<QString, int> parentBlocks;

  MutableNodeListIterator i(nodeList);
  int idx = 0;
  while (i.hasNext())
  {
    Node* n = i.next();
    BlockNode *bn = dynamic_cast<BlockNode*>(n);
    if (bn)
    {
      parentBlocks.insert(bn->name(), idx);
//       i.remove();
    }
    idx++;
  }

  MutableNodeListIterator j(m_list);

  while (j.hasNext())
  {
    Node* n = j.next();
    BlockNode *bn = dynamic_cast<BlockNode*>(n);
    if (bn)
    {
      if (parentBlocks.contains(bn->name()))
      {
        int ii = parentBlocks.value(bn->name());
        BlockNode *pbn = dynamic_cast<BlockNode *>(nodeList[ii]);
        pbn->setParent(bn->parent());
        pbn->addParent(pbn->nodeList());
        pbn->setNodeList(bn->nodeList());
        nodeList[ii] = pbn;
      } else {
        foreach(Node *node, nodeList)
        {
          TextNode *tn = dynamic_cast<TextNode*>(node);
          if (!tn)
          {
            ExtendsNode *en = dynamic_cast<ExtendsNode*>(node);
            if (en)
            {
              en->appendNode(bn);
            }
          }
        }
      }
    }
  }

  return nodeList.render(c);
}

void ExtendsNode::appendNode(Node *node)
{
  m_list.append(node);
}
