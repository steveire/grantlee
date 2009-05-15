/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "extends.h"

#include "block.h"

#include <QStringList>

#include "parser.h"
#include "template.h"
#include "grantlee.h"

#include <QDebug>

#include <QMutableListIterator>

typedef QMutableListIterator<Node*> MutableNodeListIterator;

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory()
{

}

Node* ExtendsNodeFactory::getNode(const QString &tagContent, Parser *p)
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
    fe = FilterExpression(parentName);
    parentName = QString();
  }

  NodeList nodeList = p->parse();

  return new ExtendsNode(nodeList, parentName, fe);
}

ExtendsNode::ExtendsNode(NodeList list, const QString &name, FilterExpression fe)
  : m_filterExpression(fe),
    m_name(name)
{
  m_list = list;
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

  Template* t = loader->getTemplate();

  connect(t, SIGNAL(error(int, QString)), SIGNAL(error(int, QString)));

  bool success = loader->loadByName(t, parentName);

  if (!success)
  {
      return 0;
  }
  return t;
}

QString ExtendsNode::render(Context *c)
{
  Template *parent = getParent(c);
  if (!parent)
  {
    error(TagSyntaxError, "TODO: Fix message");
    return QString();
  }

  NodeList nodeList = parent->nodeList();
  QHash<QString, int> parentBlocks;

  MutableNodeListIterator i(nodeList);
  int idx = 0;
  while (i.hasNext())
  {
    Node* n = i.next();
    BlockNode *bn = qobject_cast<BlockNode*>(n);
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
    BlockNode *bn = qobject_cast<BlockNode*>(n);
    if (bn)
    {
      if (parentBlocks.contains(bn->name()))
      {
        int ii = parentBlocks.value(bn->name());
        BlockNode *pbn = qobject_cast<BlockNode *>(nodeList[ii]);
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
            break;
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
