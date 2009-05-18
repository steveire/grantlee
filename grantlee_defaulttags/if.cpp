/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "if.h"

#include <QStringList>

#include "parser.h"
#include "grantlee.h"

#include <QDebug>

IfNodeFactory::IfNodeFactory()
{

}


Node* IfNodeFactory::getNode(const QString &tagContent, Parser *p, QObject *parent)
{
  QStringList expr = smartSplit(tagContent);
  expr.takeAt(0);
  if (expr.size() <= 0)
  {
    error(TagSyntaxError, "'if' statement requires at least one argument");
    return 0;
  }

  int linkType = IfNode::OrLink;

  bool isAnd = false;
  QString exprString = expr.join(" ");

  QStringList boolPairs = exprString.split( " and ");

  if (boolPairs.size() == 1)
  {
    boolPairs = exprString.split( " or ");
  } else {
    linkType = IfNode::AndLink;
    if (exprString.contains(" or "))
    {
      error(TagSyntaxError, "'if' tags can't mix 'and' and 'or'");
      return 0;
    }
  }

  QList<QPair<bool, FilterExpression > > boolVars;
  foreach (const QString &boolStr, boolPairs)
  {
    QPair<bool, FilterExpression> pair;
    if (boolStr.contains(" "))
    {
      QStringList bits = boolStr.split( " " );
      if (bits.size() != 2)
      {
        error(TagSyntaxError, "'if' statement improperly formatted");
        return 0;
      }
      if (bits.at(0) != "not")
      {
        // TemplateSyntaxError, "Expected 'not' in if statement"
        error(TagSyntaxError, "Expected 'not' in if statement");
        return 0;
      }
      pair.first = true;
      pair.second = FilterExpression(bits.at(1).trimmed());
    } else {
      pair.first = false;
      pair.second = FilterExpression(boolStr.trimmed());
    }
    boolVars.append(pair);
  }


  NodeList trueList = p->parse(QStringList() << "else" << "endif", parent);
  NodeList falseList;
  if (p->nextToken().content.trimmed() == "else")
  {
    falseList = p->parse(QStringList() << "endif", parent);
    // skip past the endif tag
    p->nextToken();
  } // else empty falseList.

  return new IfNode(boolVars, trueList, falseList, linkType, parent);
}


IfNode::IfNode(QList<QPair<bool, FilterExpression > > boolVars, NodeList trueList, NodeList falseList, int linkType, QObject *parent)
              : Node(parent),
                m_boolVars(boolVars),
                m_trueList(trueList),
                m_falseList(falseList),
                m_linkType(linkType)
{

}

QString IfNode::render(Context *c)
{
  // Evaluate the expression. rendering variables with the context as needed. and processing nodes recursively
  // in either trueList or falseList as determined by booleanExpression.

  if (m_linkType == OrLink)
  {
    for (int i = 0; i < m_boolVars.size(); i++)
    {
      QPair<bool, FilterExpression> pair = m_boolVars.at(i);
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue(c);

      if ( isTrue != negate )
      {
        return renderTrueList(c);
      }
    }
//     return renderFalseList(c);
  } else {
    bool renderTrue = true;
    for (int i = 0; i < m_boolVars.size(); i++)
    {
      QPair<bool, FilterExpression> pair = m_boolVars.at(i);
      bool negate = pair.first;

      bool isTrue = pair.second.isTrue(c);

      // Karnaugh map:
      //          VariantIsTrue
      //          \ 0   1
      //         0| 0 | 1 |
      // negate  1| 1 | 0 |

      if ( ( !isTrue && !negate )
        || ( isTrue && negate ) )
      {
        renderTrue = false;
        break;
      }
    }
    if (renderTrue)
      return renderTrueList(c);
  }

  return renderFalseList(c);
}

QString IfNode::renderTrueList(Context *c)
{
  return m_trueList.render(c);
}

QString IfNode::renderFalseList(Context *c)
{
  return m_falseList.render(c);
}

