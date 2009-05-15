/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"

#include <QDebug>

const char * __loadedBlocks = "__loadedBlocks";

BlockNodeFactory::BlockNodeFactory()
{

}

Node* BlockNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = smartSplit(tagContent);

  if (expr.size() != 2)
  {
    error(TagSyntaxError, "block tag takes one argument");
    return 0;
  }

  QString blockName = expr.at(1);

  QVariant loadedBlocksVariant = p->property(__loadedBlocks);
  QVariantList blockVariantList;

  if (loadedBlocksVariant.isValid() && loadedBlocksVariant.type() == QVariant::List)
  {
    blockVariantList = loadedBlocksVariant.toList();
    QListIterator<QVariant> it(blockVariantList);
    while (it.hasNext())
    {
      QString blockNodeName = it.next().toString();

      if (blockNodeName == blockName)
      {
        error(TagSyntaxError, QString("%1 appears more than once.").arg(blockName));
        return 0;
      }
    }
  }
  // Block not already in list.
  blockVariantList.append(blockName);
  loadedBlocksVariant = QVariant(blockVariantList);

  p->setProperty(__loadedBlocks, loadedBlocksVariant);

  NodeList list = p->parse(QStringList() << "endblock" << "endblock " + blockName );

  p->nextToken();

  return new BlockNode(blockName, list);
}

BlockNode::BlockNode(const QString &name, const NodeList &list)
  : m_parent(0)
{
//   m_filterExpression = FilterExpression(name);
  m_name = name;
  m_list = list;
}

QString BlockNode::render(Context *c)
{
  c->push();
  m_context = c;
  c->insert("block", QVariant::fromValue(static_cast<QObject *>(this)));
  QString result = m_list.render(c);
  c->pop();
  return result;
}

QString BlockNode::getSuper() const
{
  return m_parent->render(m_context);
}

QString BlockNode::blockName()
{
  return m_name;
//   return m_filterExpression.variable().toString();
}

BlockNode* BlockNode::parent() const
{
  return m_parent;
}

void BlockNode::addParent(NodeList nodeList)
{
  if (m_parent)
    m_parent->addParent(nodeList);
  else
  {
//     NodeList l;
//     foreach (BlockNode *block, nodeList)
//     {
//       l << block;
//     }
    m_parent = new BlockNode(m_name, nodeList);
  }
}

void BlockNode::setParent(BlockNode* node)
{
  m_parent = node;
}

void BlockNode::setNodeList(NodeList list)
{
  m_list = list;
}

NodeList BlockNode::nodeList()
{
  return m_list;
}

QString BlockNode::name()
{
  return m_name;
}
