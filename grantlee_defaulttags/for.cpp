/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "for.h"

#include <QStringList>

#include "parser.h"

#include <QDebug>

ForNodeFactory::ForNodeFactory()
{

}

Node* ForNodeFactory::getNode(const QString &tagContent, Parser *p)
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

  // todo handle unpacking keys/values. Here??
  vars << expr.first();

  FilterExpression fe(expr.last(), p);

  NodeList loopNodes = p->parse(QStringList() << "empty" << "endfor");
  NodeList emptyNodes;
  if (p->nextToken().content.trimmed() == "empty")
  {
    emptyNodes = p->parse(QStringList() << "endfor");
    // skip past the endfor tag
    p->nextToken();
  }

  return new ForNode(vars, fe, reversed, loopNodes, emptyNodes);
}



ForNode::ForNode(QList<QString> loopVars, FilterExpression fe, int reversed, NodeList loopNodeList, NodeList emptyNodes)
{
  m_loopVars = loopVars;
  m_filterExpression = fe;
  m_isReversed = reversed;
  m_loopNodeList = loopNodeList;
}

QString ForNode::render(Context *c)
{
  // some magic variables injected into the context while rendering.
  const QString forloop("forloop");
  const QString parentloop("parentloop");
  const QString counter0("counter0");
  const QString counter("counter");
  const QString revcounter0("revcounter0");
  const QString revcounter("revcounter");
  const QString first("first");
  const QString last("last");


  QString result;
  QMap<QString, QVariant> forloopMap;
//   forloopMap.insert(parentloop, parentLoopMap);

  QVariant parentLoopVariant = c->lookup(forloop);
  QMap<QString, QVariant> parentLoopMap;
  if (parentLoopVariant.isValid())
  {
    // This is a nested loop.
    forloopMap.insert(parentloop, parentLoopVariant.toMap());
  }
  // If it's an iterable type, iterate, otherwise it's a list of one.
  QVariantList varList = m_filterExpression.toList(c);

//   QListIterator<QVariant> varIter(varList);
//   QVariant variant = m_filterExpression.next();

//   int loopCounter = 0;
//   while (variant.isValid())

  c->push();

  NodeList nodeList;

  int listSize = varList.size();
  
  for (int i = 0; i < listSize; i++)
  {
    forloopMap = c->lookup("forloop").toMap();
    forloopMap.insert(counter0, i);
    forloopMap.insert(counter, i + 1);
    forloopMap.insert(revcounter, listSize - i );
    forloopMap.insert(revcounter0, listSize - i -1 );
    forloopMap.insert(first, ( i == 0 ) );
    forloopMap.insert(last, ( i == listSize - 1 ) );
    c->insert(forloop, forloopMap);

// If here for map key, value style.

    if (m_isReversed == IsNotReversed)
    {
      c->insert(m_loopVars[0], varList[i]);
    } else {
      c->insert(m_loopVars[0], varList[listSize - i - 1]);
    }

    for (int j = 0; j < m_loopNodeList.size();j++)
    {
      result += m_loopNodeList[j]->render(c);
//       nodeList.append(m_loopNodeList[j].render(c));
    }

  }
  c->pop();
  return result;
}
