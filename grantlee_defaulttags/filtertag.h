
#ifndef FILTERTAG_H
#define FILTERTAG_H

#include "node.h"

using namespace Grantlee;

class FilterNodeFactory : public AbstractNodeFactory
{
  public:
    FilterNodeFactory();

    Node* getNode(const QString &tagContent, Parser *p, QObject *parent);

};

class FilterNode : public Node
{
  Q_OBJECT
public:
  FilterNode(FilterExpression fe, NodeList filterList, QObject *parent);

  QString render(Context *c);

private:
  FilterExpression m_fe;
  NodeList m_filterList;
};

#endif
