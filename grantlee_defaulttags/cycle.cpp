/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "cycle.h"

#include <QStringList>
#include "parser.h"


static const char * _namedCycleNodes = "_namedCycleNodes";

CycleNodeFactory::CycleNodeFactory()
{

}

Node* CycleNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = smartSplit(tagContent);

  if (expr.size() < 1 )
  {
    setError(TagSyntaxError, QString("%1 expects at least one argument").arg(expr.at(0)));
    return 0;
  }

  if (expr.at(1).contains(","))
  {
    QStringList csvlist = expr.at(1).split(",");
    expr.removeAt(1);
    for (int i = 0; i < csvlist.size() ; ++i )
    {
      expr.insert(i+1, "\"" + csvlist.at(i) + "\"");
    }
  }

  if (expr.size() == 2)
  {
    // {% cycle var %}
    QString name = expr.at(1);
    QVariant cycleNodes = p->property(_namedCycleNodes);
    if (!cycleNodes.isValid() || cycleNodes.type() != QVariant::Hash)
    {
      setError(TagSyntaxError, QString("No named cycles in template. '%1' is not defined").arg(name));
      return 0;
    }
    QVariantHash hash = cycleNodes.toHash();
    if (!hash.contains(name))
    {
      setError(TagSyntaxError, QString("Node not found: %1").arg(name));
      return 0;
    }
    QVariant nodeVariant = hash.value(name);
    if (nodeVariant.userType() != QMetaType::QObjectStar)
    {
      setError(TagSyntaxError, "Invalid object in node cycle list");
      return 0;
    }
    QObject *obj = nodeVariant.value<QObject*>();
    Node *node = qobject_cast<Node*>(obj);
    if (!node)
    {
      setError(TagSyntaxError, "Invalid object in node cycle list");
      return 0;
    }
    return node;
  }

  int exprSize = expr.size();
  if ( exprSize > 4 && expr.at( exprSize - 2 ) == "as")
  {
    // {% cycle "foo" "bar" "bat" as var %}
    QString name = expr.at(exprSize - 1);
    QStringList list = expr.mid(1, exprSize - 3);
    Node *node = new CycleNode(getFilterExpressionList(list, p), name, parent);
    QVariant hashVariant = p->property(_namedCycleNodes);
    QVariantHash hash;
    if (hashVariant.isValid() && hashVariant.type() == QVariant::Hash)
    {
      hash = hashVariant.toHash();
    }
    QObject *nodeObject = node;
    QVariant nodeVariant = QVariant::fromValue(nodeObject);
    hash.insert(name, nodeVariant);
    p->setProperty(_namedCycleNodes, QVariant(hash));
    return node;
  } else {
    QStringList list = expr.mid(1, exprSize - 1);
    return new CycleNode(getFilterExpressionList(list, p), QString(), parent);
  }
}

CycleNode::CycleNode(QList<FilterExpression> list, const QString &name, QObject *parent)
  : Node(parent), m_variableIterator(list)
{
  m_name = name;
}

QString CycleNode::render(Context *c)
{
  QString value = m_variableIterator.next().resolve(c).toString();
  if (!m_name.isEmpty())
  {
    c->insert(m_name, value);
  }
  return value;
}

