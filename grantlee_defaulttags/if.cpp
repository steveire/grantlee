/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "if.h"

#include <QStringList>

#include "parser.h"

#include <QDebug>

IfNodeFactory::IfNodeFactory()
{

}


Node* IfNodeFactory::getNode(const QString &tagContent, Parser *p)
{
  QStringList expr = tagContent.split(" ", QString::SkipEmptyParts);
  expr.takeAt(0);
//   if (expr.size() <= 0)
//     error(Parser::TagSyntaxError, "'if' statement requires at least one argument");

  bool isAnd = false;
  if (expr.contains("and"))
  {
    isAnd = true;
  }
  if (expr.contains("or"))
  {
    if (isAnd == true)
    {
//       error(Parser::TagSyntaxError, "'if' tags can't mix 'and' and 'or'");
    }
  }


  NodeList trueList = p->parse(QStringList() << "else" << "endif");
  NodeList falseList;
  if (p->nextToken().content.trimmed() == "else")
  {
    falseList = p->parse(QStringList() << "endif");
    // skip past the endif tag
    p->nextToken();
  } // else empty falseList.

  return new IfNode(expr.join(" "), trueList, falseList);
}


IfNode::IfNode(const QString &booleanExpression, NodeList trueList, NodeList falseList)
              : m_linkType(OrLink)
{
  m_booleanExpression = booleanExpression;
  m_trueList = trueList;
  m_falseList = falseList;

  QStringList boolpairs = booleanExpression.split(" and " );
  if ( boolpairs.size() == 1 )
  {
    // the expression does not contain 'and'
    boolpairs = booleanExpression.split( " or " );

  } else {
    if (booleanExpression.contains(" or "))
    {
      // Error. Can't mix these.
    }
  }

//   if (m_booleanExpression.contains("and"))
//   {
//     m_linkType = AndLink;
//     boolpairs = simplified.split("and");
//   }
//   if (m_booleanExpression.contains("or"))
//   {
//     if ( m_linkType == AndLink )
//     {
//       // Error. can't mix these.
//     }
//     m_linkType = OrLink;
//     boolpairs = simplified.split("or");
//   }


  foreach (const QString &boolStr, boolpairs)
  {
    QPair<bool, FilterExpression> pair;
    QStringList splitted = boolStr.split("not");
    if (splitted.size() == 2)
    {
      // whether to negate the result of the string.
      pair.first = true;
      splitted.removeFirst();
    } else {
      pair.first = false;
    }
    pair.second = FilterExpression(splitted[0].trimmed());
    m_boolVars.append(pair);
  }

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

      if ( ( isTrue && !negate )
        || ( !isTrue && negate ) )
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

