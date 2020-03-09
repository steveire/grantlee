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

#include "block.h"

#include "blockcontext.h"
#include "exception.h"
#include "parser.h"
#include "rendercontext.h"
#include "template.h"
#include "util.h"

static const char *const __loadedBlocks = "__loadedBlocks";

// Terrible hack warning.
#define BLOCK_CONTEXT_KEY 0

BlockNodeFactory::BlockNodeFactory(QObject *parent)
    : AbstractNodeFactory(parent)
{
}

Node *BlockNodeFactory::getNode(const QString &tagContent, Parser *p) const
{
  const auto expr = tagContent.split(QLatin1Char(' '), QString::SkipEmptyParts);

  if (expr.size() != 2) {
    throw Grantlee::Exception(TagSyntaxError,
                              QStringLiteral("block tag takes one argument"));
  }

  const auto blockName = expr.at(1);

  auto loadedBlocksVariant = p->property(__loadedBlocks);
  QVariantList blockVariantList;

  if (loadedBlocksVariant.isValid()
      && loadedBlocksVariant.userType() == qMetaTypeId<QVariantList>()) {
    blockVariantList = loadedBlocksVariant.value<QVariantList>();
    for (auto &item : blockVariantList) {
      const auto blockNodeName = item.value<QString>();

      if (blockNodeName == blockName) {
        throw Grantlee::Exception(
            TagSyntaxError,
            QStringLiteral("'block' tag with name '%1' appears more than once.")
                .arg(blockName));
      }
    }
  }
  // Block not already in list.
  blockVariantList.append(blockName);
  loadedBlocksVariant = QVariant(blockVariantList);

  p->setProperty(__loadedBlocks, loadedBlocksVariant);

  auto n = new BlockNode(blockName, p);
  const auto list = p->parse(n, QStringLiteral("endblock"));

  auto endBlock = p->takeNextToken();
  const QStringList acceptableBlocks{QStringLiteral("endblock"),
                                     QStringLiteral("endblock ") + blockName};
  if (!acceptableBlocks.contains(endBlock.content)) {
    p->invalidBlockTag(endBlock, QStringLiteral("endblock"), acceptableBlocks);
  }

  n->setNodeList(list);

  return n;
}

BlockNode::BlockNode(const QString &name, QObject *parent)
    : Node(parent), m_name(name), m_stream(0)
{
  qRegisterMetaType<Grantlee::SafeString>("Grantlee::SafeString");
}

BlockNode::~BlockNode() {}

void BlockNode::setNodeList(const NodeList &list) const { m_list = list; }

void BlockNode::render(OutputStream *stream, Context *c) const
{
  QVariant &variant = c->renderContext()->data(BLOCK_CONTEXT_KEY);
  auto blockContext = variant.value<BlockContext>();

  c->push();

  if (blockContext.isEmpty()) {
    m_context = c;
    m_stream = stream;
    c->insert(QStringLiteral("block"),
              QVariant::fromValue(
                  const_cast<QObject *>(static_cast<const QObject *>(this))));
    m_list.render(stream, c);
    m_stream = 0;
  } else {
    auto block = static_cast<const BlockNode *>(blockContext.pop(m_name));
    variant.setValue(blockContext);
    auto push = block;
    if (!block)
      block = this;

    const auto list = block->m_list;

    block = new BlockNode(block->m_name, 0);
    block->setNodeList(list);
    block->m_context = c;
    block->m_stream = stream;
    c->insert(QStringLiteral("block"),
              QVariant::fromValue(
                  const_cast<QObject *>(static_cast<const QObject *>(block))));
    list.render(stream, c);

    delete block;
    if (push) {
      blockContext.push(m_name, push);
      variant.setValue(blockContext);
    }
  }
  c->pop();
}

SafeString BlockNode::getSuper() const
{
  if (m_context->renderContext()->contains(BLOCK_CONTEXT_KEY)) {
    QVariant &variant = m_context->renderContext()->data(BLOCK_CONTEXT_KEY);
    const auto blockContext = variant.value<BlockContext>();
    auto block = blockContext.getBlock(m_name);
    if (block) {
      QString superContent;
      QTextStream superTextStream(&superContent);
      auto superStream = m_stream->clone(&superTextStream);
      const_cast<BlockNode *>(this)->render(superStream.data(), m_context);
      return markSafe(superContent);
    }
  }
  return SafeString();
}

NodeList BlockNode::nodeList() const { return m_list; }

QString BlockNode::name() const { return m_name; }
