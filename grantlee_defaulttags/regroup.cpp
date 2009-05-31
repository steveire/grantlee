/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "regroup.h"

#include <QStringList>
#include "parser.h"



RegroupNodeFactory::RegroupNodeFactory()
{

}

Node* RegroupNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent) const
{
  QStringList expr = tagContent.split(" ");

  if (expr.size() != 6)
  {
    setError(TagSyntaxError, "widthratio takes five arguments");
    return 0;
  }
  FilterExpression target(expr.at(1), p);
  if (expr.at(2) != "by")
  {
    setError(TagSyntaxError, "second argument must be 'by'");
    return 0;
  }

  if (expr.at(4) != "as")
  {
    setError(TagSyntaxError, "fourth argument must be 'as'");
    return 0;
  }

  FilterExpression expression("\"" + expr.at(3) + "\"", p);

  QString name = expr.at(5);

  return new RegroupNode(target, expression, name, parent);
}

RegroupNode::RegroupNode(FilterExpression target, FilterExpression expression, const QString &varName, QObject *parent)
  : Node(parent), m_target(target), m_expression(expression), m_varName(varName)
{

}

QString RegroupNode::render(Context *c)
{
  QVariantList objList = m_target.resolve(c).toList();
  if (objList.isEmpty())
  {
    c->insert(m_varName, QVariantMap());
    return QString("");
  }

  // What's going on?
  //
  // objList is a flat list of objects with a common parameter. For example, Person objects with
  // a name parameter. The list is already sorted.
  // Say the objList contains ["David Beckham", "David Blain", "Keira Nightly"] etc.
  // We want to regroup the list into separate lists of people with the same first name.
  // ie objMap should be: {"David": ["David Beckham", "David Blain"], "Keira": ["Keira Nightly"]}
  //
  // We then insert the objMap into the Context ready for rendering later in a for loop.

  QVariantList list;
  QVariantMap objMap;
  QString mapKey;
  QString lastKey;
  QString keyName = m_expression.resolve(c).toString();
  QListIterator<QVariant> i(objList);
  while (i.hasNext())
  {
    QVariant var = i.next();
    c->push();
    c->insert("var", var);
    QString key = FilterExpression("var." + keyName, 0).resolve(c).toString();
    c->pop();
    if (key != mapKey)
    {
      lastKey = mapKey;
      mapKey = key;
      if (!list.isEmpty())
      {
        // list is now a list of Person objects with the same first name. lastKey is that first name.
        objMap.insert(lastKey, list);
        list.clear();
      }
    }
    list.append(var);
  }
  if (!list.isEmpty())
  {
    objMap.insert(mapKey, list);
  }
  if (!objMap.isEmpty())
  {
    QVariantList contextList;
    QMapIterator<QString, QVariant> i(objMap);
    while (i.hasNext())
    {
      i.next();
      QVariantMap contextMap;
      contextMap.insert("grouper", i.key());
      contextMap.insert("list", i.value());
      contextList.append(contextMap);
    }
    c->insert(m_varName, contextList);
  }
  return QString();
}

