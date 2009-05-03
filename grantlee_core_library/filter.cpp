/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#include "filter.h"
#include "parser.h"
#include "variable.h"

#include <QDebug>


static const QString FILTER_SEPARATOR = "|";
static const QString FILTER_ARGUMENT_SEPARATOR = ":";
static const QString VARIABLE_ATTRIBUTE_SEPARATOR = ".";
static const QString ALLOWED_VARIABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.";

FilterExpression::FilterExpression(const QString &varString, Parser *parser)
{
  m_variable = Variable(varString);
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
  // filter it...
//   foreach(Filter filter, m_filters)
//   {
//
//   }
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

QVariantList FilterExpression::toList(Context *c, int reversed)
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

// QVariant FilterExpression::next()
// {
//   return m_variantList[m_position++];
//
// //   if (!var.isValid())
// //     return QVariant();
// //
// //   m_iterVariant = var;
// }

