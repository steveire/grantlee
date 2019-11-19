/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "extends.h"

#include "block.h"
#include "blockcontext.h"
#include "engine.h"
#include "exception.h"
#include "nodebuiltins_p.h"
#include "parser.h"
#include "rendercontext.h"
#include "template.h"
#include "util.h"

#include <QtCore/QListIterator>

using namespace Grantlee;

ExtendsNodeFactory::ExtendsNodeFactory(QObject *parent)
    : AbstractNodeFactory(parent)
{
}

Node *ExtendsNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  const auto expr = smartSplit(tagContent);

  if (expr.size() != 2)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Error: Include tag takes only one argument"));

  FilterExpression fe(expr.at(1), p);

  auto n = new ExtendsNode(fe, p);

  auto t = qobject_cast<TemplateImpl *>(p->parent());

  if (!t)
    throw Grantlee::Exception(
        TagSyntaxError, QStringLiteral("Extends tag is not in a template."));

  const auto nodeList = p->parse(t);
  n->setNodeList(nodeList);

  if (t->findChildren<ExtendsNode *>().size() > 1) {
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Extends tag may only appear once in a template."));
  }

  return n;
}

ExtendsNode::ExtendsNode(const FilterExpression &fe, QObject *parent)
    : Node(parent), m_filterExpression(fe)
{
}

ExtendsNode::~ExtendsNode() {}

static QHash<QString, BlockNode *> createNodeMap(const QList<BlockNode *> &list)
{
  QHash<QString, BlockNode *> map;

  auto it = list.constBegin();
  const auto end = list.constEnd();

  for (; it != end; ++it) {
    map.insert((*it)->name(), *it);
  }

  return map;
}

void ExtendsNode::setNodeList(const NodeList &list)
{
  m_list = list;

  const auto blockList = m_list.findChildren<BlockNode *>();
  m_blocks = createNodeMap(blockList);
}

Template ExtendsNode::getParent(Context *c) const
{
  const auto parentVar = m_filterExpression.resolve(c);
  if (parentVar.userType() == qMetaTypeId<Grantlee::Template>()) {
    return parentVar.value<Template>();
  }

  QString parentName = getSafeString(parentVar);

  auto ti = containerTemplate();

  const auto t = ti->engine()->loadByName(parentName);

  if (!t)
    throw Grantlee::Exception(
        TagSyntaxError,
        QStringLiteral("Template not found %1").arg(parentName));

  if (t->error())
    throw Grantlee::Exception(t->error(), t->errorString());

  return t;
}

void ExtendsNode::render(OutputStream *stream, Context *c) const
{
  const auto parentTemplate = getParent(c);

  if (!parentTemplate) {
    throw Grantlee::Exception(TagSyntaxError,
                              QStringLiteral("Cannot load template."));
  }

  QVariant &variant = c->renderContext()->data(0);
  auto blockContext = variant.value<BlockContext>();
  blockContext.addBlocks(m_blocks);
  variant.setValue(blockContext);

  const auto nodeList = parentTemplate->nodeList();

  const auto parentBlocks
      = createNodeMap(parentTemplate->findChildren<BlockNode *>());
  QListIterator<Node *> i(nodeList);

  while (i.hasNext()) {
    auto n = i.next();
    auto tn = qobject_cast<TextNode *>(n);
    if (!tn) {
      auto en = qobject_cast<ExtendsNode *>(n);
      if (!en) {
        blockContext.addBlocks(parentBlocks);
        variant.setValue(blockContext);
      }
      break;
    }
  }
  variant.setValue(blockContext);
  parentTemplate->nodeList().render(stream, c);

  auto nodes = parentTemplate->findChildren<BlockNode *>();
  blockContext.remove(nodes);
  variant.setValue(blockContext);
}

void ExtendsNode::appendNode(Node *node)
{
  m_list.append(node);
  node->setParent(parent());
}
