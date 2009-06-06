/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "for.h"

#include <QStringList>

#include "parser.h"


ForNodeFactory::ForNodeFactory()
{

}

Node* ForNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  expr.takeAt(0);
  QList<QString> vars;

  int reversed = ForNode::IsNotReversed;
  if (expr.last() == "reversed")
  {
    reversed = ForNode::IsReversed;
    expr.removeLast();
  }

  foreach(const QString &arg, expr.mid(0, expr.size() -2))
  {
    vars << arg.split(",", QString::SkipEmptyParts);
  }

  FilterExpression fe(expr.last(), p);

  NodeList loopNodes = p->parse(QStringList() << "empty" << "endfor", parent);
  NodeList emptyNodes;
  if (p->nextToken().content.trimmed() == "empty")
  {
    emptyNodes = p->parse(QStringList() << "endfor", parent);
    // skip past the endfor tag
    p->deleteNextToken();
  }

  return new ForNode(vars, fe, reversed, loopNodes, emptyNodes, parent);
}



ForNode::ForNode( QStringList loopVars,
                  FilterExpression fe,
                  int reversed,
                  NodeList loopNodeList,
                  NodeList emptyNodes,
                  QObject *parent )
  : Node(parent),
  m_loopVars(loopVars),
  m_filterExpression(fe),
  m_isReversed(reversed),
  m_loopNodeList(loopNodeList),
  m_emptyNodeList(emptyNodes)
{

}

// some magic variables injected into the context while rendering.
const QString forloop("forloop");
const QString parentloop("parentloop");
const QString counter0("counter0");
const QString counter("counter");
const QString revcounter0("revcounter0");
const QString revcounter("revcounter");
const QString first("first");
const QString last("last");

void ForNode::insertLoopVariables(Context *c, int listSize, int i)
{
  QVariantHash forloopHash = c->lookup("forloop").toHash();
  forloopHash.insert(counter0, i);
  forloopHash.insert(counter, i + 1);
  forloopHash.insert(revcounter, listSize - i );
  forloopHash.insert(revcounter0, listSize - i -1 );
  forloopHash.insert(first, ( i == 0 ) );
  forloopHash.insert(last, ( i == listSize - 1 ) );
  c->insert(forloop, forloopHash);
}

QString ForNode::renderLoop(Context *c)
{
  QString result;
  for (int j = 0; j < m_loopNodeList.size();j++)
  {
    result += m_loopNodeList[j]->render(c);
  }
  return result;
}

QString ForNode::handleHashItem(Context *c, QString key, QVariant value, int listSize, int i, bool unpack)
{
  QVariantList list;
  insertLoopVariables(c, listSize, i);

  if (!unpack)
  {
    // Iterating over a hash but not unpacking it.
    // convert each key-value pair to a list and insert it in the context.
    list << key << value;
    c->insert(m_loopVars.at(0), list);
    list.clear();
  } else {
    c->insert(m_loopVars.at(0), key);
    c->insert(m_loopVars.at(1), value);
  }
  return renderLoop(c);
}

QString ForNode::iterateHash(Context *c, QVariantHash varHash, bool unpack)
{
  QString result;

  int listSize = varHash.size();
  int i = 0;
  QVariantList list;

  QHashIterator<QString, QVariant> it(varHash);
  if (m_isReversed == IsReversed)
  {
    while (it.hasPrevious())
    {
      it.previous();
      result += handleHashItem(c, it.key(), it.value(), listSize, i, unpack);
      ++i;
    }
  } else {
    while (it.hasNext())
    {
      it.next();
      result += handleHashItem(c, it.key(), it.value(), listSize, i, unpack);
      ++i;
    }
  }
  return result;
}

QString ForNode::render(Context *c)
{
  QString result;
  QVariantHash forloopHash;

  QVariant parentLoopVariant = c->lookup(forloop);
  if (parentLoopVariant.isValid())
  {
    // This is a nested loop.
    forloopHash = parentLoopVariant.toHash();
    forloopHash.insert(parentloop, parentLoopVariant.toHash());
    c->insert(forloop, forloopHash);
  }

  bool unpack = m_loopVars.size() > 1;

  QVariant var = m_filterExpression.resolve(c);

  c->push();

  if (var.type() == QVariant::Hash)
  {
    QVariantHash varHash = var.toHash();
    result = iterateHash(c, varHash, unpack);
    c->pop();
    return result;
  }

  // If it's an iterable type, iterate, otherwise it's a list of one.
  QVariantList varList = m_filterExpression.toList(c);
  NodeList nodeList;
  int listSize = varList.size();

  if (listSize < 1)
  {
    c->pop();
    return m_emptyNodeList.render(c);
  }

  for (int i = 0; i < listSize; i++)
  {
    insertLoopVariables(c, listSize, i);

    int index = i;
    if (m_isReversed == IsReversed)
    {
      index = listSize - i - 1;
    }

    if (unpack)
    {
      if (varList[index].type() == QVariant::List)
      {
        QVariantList vList = varList[index].toList();
        int varsSize = qMin(m_loopVars.size(), vList.size());
        int j = 0;
        for ( ; j < varsSize; ++j)
        {
          c->insert(m_loopVars.at(j), vList.at(j));
        }
        // If any of the named vars don't have an item in the context,
        // insert an invalid object for them.
        for ( ; j < m_loopVars.size(); ++j)
        {
          c->insert(m_loopVars.at(j), QVariant());
        }

      } else {
        // We don't have a hash, but we have to unpack several values from each item
        // in the list. And each item in the list is not itself a list.
        // Probably have a list of objects that we're taking properties from.
        foreach(const QString &loopVar, m_loopVars)
        {
          c->push();
          c->insert("var", varList[index]);
          QVariant v = FilterExpression("var." + loopVar, 0).resolve(c);
          c->pop();
          c->insert(loopVar, v);
        }
      }
    } else {
      c->insert(m_loopVars[0], varList[index]);
    }
    result += renderLoop(c);
  }
  c->pop();
  return result;
}

NodeList ForNode::getNodesByType(const char* className)
{
  NodeList nodeList;
  nodeList << m_loopNodeList.getNodesByType(className);
  nodeList << m_emptyNodeList.getNodesByType(className);
  return nodeList;
}

