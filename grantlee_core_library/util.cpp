
#include "util_p.h"

Util::Util()
{

}

QString Util::unescapeStringLiteral(const QString &input)
{
  return input.mid(1, input.size() - 2 )
              .replace("\\\'", "'")
              .replace("\\\"", "\"")
              .replace("\\\\", "\\");
}

bool Util::variantIsTrue(const QVariant &variant)
{

  if (!variant.isValid())
    return false;

  switch (variant.userType())
  {
  case QVariant::Bool:
  {
    return variant.toBool();
  }
  case QVariant::String:
  {
    return !variant.toString().isEmpty();
  }
  case QVariant::Int:
  {
    return (variant.toInt() > 0);
  }
  case QVariant::Double:
  {
    return (variant.toDouble() > 0);
  }
  case QMetaType::QObjectStar:
  {
    QObject *obj = variant.value<QObject *>();
    if (!obj)
      return false;

    if (obj->property("__true__").isValid())
    {
      return obj->property("__true__").toBool();
    }
    return true;
  }
  case QVariant::List:
  {
    return (variant.toList().size() > 0);
  }
  case QVariant::Map:
  {
    return (variant.toMap().size() > 0);
  }
  default:
    return false;
  }
}

QVariantList Util::variantToList(const QVariant &var)
{

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
    QObject *obj = var.value<QObject*>();
    if (obj->property("__list__").isValid())
    {
      return obj->property("__list__").toList();
    }
  }
  else
  {
    return QVariantList() << var;
  }
}




