/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "filterexpression.h"
#include "parser.h"
#include "variable.h"
#include "grantlee.h"
#include "filter.h"

#include <QDebug>

using namespace Grantlee;


static const char * FILTER_SEPARATOR = "|";
static const char * FILTER_ARGUMENT_SEPARATOR = ":";
static const char * VARIABLE_ATTRIBUTE_SEPARATOR = ".";
static const char * ALLOWED_VARIABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\\.";


static const char * str = "[^\"\\\\]*(?:\\.[^\"\\\\]*)*";
static const char * varChars = "\\w\\." ;
static const QString filterSep( QRegExp::escape( FILTER_SEPARATOR ) );
static const QString argSep( QRegExp::escape( FILTER_ARGUMENT_SEPARATOR ) );
static const QString i18nOpen( QRegExp::escape( "_(" ) );
static const QString i18nClose( QRegExp::escape( ")" ) );

static const QString filterRawString = QString(
                           "^%1\"(%2)\"%3|"
                           "^\"(%2)\"|"
                           "^([%4]+)|"
                           "%5(\\w+)|"
                           "%6(%1\"%2\"%3|\"%2\"|[%4]+)"
                         )
              /* 1 */    .arg(i18nOpen)
              /* 2 */    .arg(str)
              /* 3 */    .arg(i18nClose)
              /* 4 */    .arg(varChars)
              /* 5 */    .arg(filterSep)
              /* 6 */    .arg(argSep)
                         ;

static const QRegExp sFilterRe(filterRawString);

FilterExpression::FilterExpression(const QString &varString, Parser *parser)
  : m_error(NoError)
{

  int pos = 0;
  int lastPos = 0;
  int len;
  QString subString;

  if (varString.contains("\n"))
  {
    // error.
  }
  
  
  QString vs = varString.trimmed();
  
  while ((pos = sFilterRe.indexIn(vs, pos)) != -1) {
    len = sFilterRe.matchedLength();
    subString = vs.mid(pos, len);
    int ssSize = subString.size();
    if (subString.startsWith(FILTER_SEPARATOR))
    {
      subString = subString.right(ssSize - 1);
      Filter *f = parser->getFilter(subString);
      if (f)
        m_filters << qMakePair<Filter*, Variable>(f, Variable());
      else
      {
        m_error = TagSyntaxError;
        return;
      }
    }
    else if (subString.startsWith(FILTER_ARGUMENT_SEPARATOR))
    {
      subString = subString.right(ssSize - 1);
      int lastFilter = m_filters.size();
      m_filters[lastFilter -1].second = Variable(subString);
    } else if (subString.startsWith("_(") && subString.endsWith(")"))
    {
      // Token is _("translated"): subString.mid(1, ssSize - 1 -2);
    } else if (subString.startsWith("\"") && subString.endsWith("\""))
    {
      // Token is a "constant"
      m_variable = Variable(subString);      
    } else
    {
      // Arg is a variable
      m_variable = Variable(subString);
    }
    
    pos += len;
    lastPos = pos;
  }
  QString remainder = vs.right( vs.size() - lastPos);
  if (remainder.size() > 0)
  {
    m_error = TagSyntaxError;
  }
}

int FilterExpression::error()
{
  return m_error;
}


FilterExpression::FilterExpression()
{
//   m_variable = Variable(token.content);
}

Variable FilterExpression::variable()
{
  return m_variable;
}

QVariant FilterExpression::resolve(Context *c)
{
  QVariant var = m_variable.resolve(c);
  foreach(ArgFilter argfilter, m_filters)
  {
    var = argfilter.first->doFilter(var, argfilter.second.resolve(c).toString());
  }
  return var;
}

// void FilterExpression::begin(Context *c, int reversed)
// {
//   m_position = 0;
//   QVariant var = resolve(c);
//   if (!var.isValid())
//     return;
//
//   if (var.type() == QVariant::String)
//   {
//
//   } else if (var.userType() > 0)
//   {
//     // A registered user type. Signal error.
// //     void *obj = QMetaType::construct(var.userType(), (void *)var );
//
//     // Instead of all this messing, I can simply require that an object can't be iterated over.
//     // Iterate over a property instead.
//     // Or I can allow iterating over an objects children(), which is a QList<QObject>
//
// //     QMetaType::destroy(var.userType(), obj);
//     //   TODO: see if I can use QMetaType::construct for user defined types.
//   }
//   else
//   {
//     m_variantList.append(var);
//   }
// }

QVariantList FilterExpression::toList(Context *c)
{
  QVariant var = resolve(c);
  if (!var.isValid())
    return QVariantList();

  if (var.type() == QVariant::List)
  {
    return var.toList();
  }
  if (var.type() == QVariant::String)
  {
    QString s = var.toString();

    QString::iterator i;
    QVariantList list;
    for (i = s.begin(); i != s.end(); ++i)
    {
      list << *i;
    }
    return list;
  } else if (var.userType() == QMetaType::QObjectStar)
  {
    // A registered user type. Signal error.
//     void *obj = QMetaType::construct(var.userType(), (void *)var );

    // Instead of all this messing, I can simply require that an object can't be iterated over.
    // Iterate over a property instead.
    // Or I can allow iterating over an objects children(), which is a QList<QObject *>

//     QMetaType::destroy(var.userType(), obj);
    //   TODO: see if I can use QMetaType::construct for user defined types.
  }
  else
  {
//     QVariantList list;
//     list << var;
//     return list;
    return QVariantList() << var;
//     m_variantList.append(var);
  }

}

bool FilterExpression::isTrue(Context *c)
{
  QVariant variant = resolve(c);

  if (!variant.isValid())
    return false;

  switch (variant.type())
  {
  case QVariant::Bool:
  {
    return variant.toBool();
    break;
  }
  case QVariant::String:
  {
    return !variant.toString().isEmpty();
    break;
  }
  case QVariant::Int:
  {
    return (variant.toInt() > 0);
  }
  default:
    break;
  }

//     if it contains QObject pointer
//     {
//       if pointer is null:
//         return false;
//       return true;
//     }

//     if (it contains number)
//     {
//       //       QString varString = var.toString();
//       if (number == 0)
//         return false;
//       return true;
//     }
//     if (it contains boolean)
//     {
//       return boolean;
//     }
//     if (it contains collection)
//     {
//       return collection.size() > 0;
//     }
//
//     if (it contains string)
//     {
//       QString varString = var.toString();
//       if (varString.isEmpty())
//         return false;
//       return true;
//     }
// etc for QPoint etc.

  return false;
  
}

// QVariant FilterExpression::next()
// {
//   return m_variantList[m_position++];
//
// //   if (!var.isValid())
// //     return QVariant();
// //
// //   m_iterVariant = var;
// }

