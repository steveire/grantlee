/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "ifchanged.h"

#include <QStringList>
#include <QDateTime>

#include "parser.h"



IfChangedNodeFactory::IfChangedNodeFactory()
{

}

Node* IfChangedNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);

  NodeList trueList = p->parse(QStringList() << "else" << "endifchanged", parent );
  NodeList falseList;

  if (p->nextToken().content.trimmed() == "else")
  {
    falseList = p->parse(QStringList() << "endifchanged", parent );
    p->deleteNextToken();
  }
  expr.takeAt(0);

  return new IfChangedNode(trueList, falseList, getFilterExpressionList(expr, p), parent);
}

IfChangedNode::IfChangedNode(NodeList trueList, NodeList falseList, QList<FilterExpression> feList, QObject *parent)
  : Node(parent), m_trueList(trueList), m_falseList(falseList), m_filterExpressions(feList)
{
  m_lastSeen = QVariant();
  m_id = QString::number(reinterpret_cast<qint64>(this));
}

QString IfChangedNode::render(Context *c)
{
  if (c->lookup("forloop").isValid() && (!c->lookup("forloop").toHash().contains(m_id)))
  {
    m_lastSeen = QVariant();
    QVariantHash hash = c->lookup("forloop").toHash();
    hash.insert(m_id, 1);
    c->insert("forloop", hash);
  }

  QString watchedString;
  if (m_filterExpressions.size() == 0)
  {
    watchedString = m_trueList.render(c);
  }
  QListIterator<FilterExpression> i(m_filterExpressions);
  QVariantList watchedVars;
  while (i.hasNext())
  {
    QVariant var = i.next().resolve(c);
    if (!var.isValid())
    {
      // silent error
      return QString();
    }
    watchedVars << var;
  }

  // At first glance it looks like m_last_seen will always be invalid,
  // But it will change because render is called multiple times by the parent
  // for loop.
  if ( (watchedVars != m_lastSeen.toList() )
      || ( !watchedString.isNull() && ( watchedString != m_lastSeen.toString() ) ) )
  {
    bool firstLoop = !m_lastSeen.isValid();
    if (!watchedString.isNull())
      m_lastSeen = watchedString;
    else
      m_lastSeen = watchedVars;
    c->push();
    QVariantHash hash;
    // TODO: Document this.
    hash.insert("firstloop", firstLoop);
    c->insert("ifchanged", hash);
    QString content = m_trueList.render(c);
    c->pop();
    return content;
  } else if (!m_falseList.isEmpty())
  {
    return m_falseList.render(c);
  }


  return QString();
}

NodeList IfChangedNode::getNodesByType(const char* className)
{
  NodeList list;
  list << m_trueList.getNodesByType(className);
  list << m_falseList.getNodesByType(className);
  return list;
}

